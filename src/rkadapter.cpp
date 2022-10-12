#include <redis++.h>
#include <iostream>
#include <cstdio>
#include <csignal>
#include <cstdlib>
#include <rkadapter.h>
#include <rdkafkacpp.h>
#include <json.hpp>

using json = nlohmann::json;
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

static volatile sig_atomic_t run = 1;
using namespace sw::redis;
using namespace std;
using nlohmann::to_json;

struct RequestIdMap
{
	int m_number;
	string m_request_id;
	RequestIdMap() : m_number(0), m_request_id("Unknown")
	{}
	RequestIdMap(string name, int number) :
		m_number(number), m_request_id(name)
	{}
};

static RequestIdMap IdMap[] =
{
	RequestIdMap("EventCallCreated",EventCallCreated)

};

static int
GetRequestEnumType(string request_id)
{
	for (int i = 0; i < sizeof(IdMap) / sizeof(RequestIdMap); i++)
	{
		if (IdMap[i].m_request_id == request_id)
		{
			return IdMap[i].m_number;
		}
	}
}


class kafkaProducer {
public:
	RdKafka::Producer *producer;
	string brokers = "127.0.0.1:9092";
	//string topic = "voicetest";
	string call_topic = "voice-tserver-call";
	string dn_topic = "voice-tserver-dn";
	TMessageType msgType;
	kafkaProducer() {
		RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

		/* Set bootstrap broker(s) as a comma-separated list of
		* host or host:port (default port 9092).
		* librdkafka will use the bootstrap brokers to acquire the full
		* set of brokers from the cluster. */
		string errstr;
		if (conf->set("bootstrap.servers", brokers, errstr) !=
			RdKafka::Conf::CONF_OK) {
			std::cerr << errstr << std::endl;
			exit(1);
		}

		/*
		* Create producer instance.
		*/
		producer = RdKafka::Producer::create(conf, errstr);
		if (!producer) {
			std::cerr << "Failed to create producer: " << errstr << std::endl;
			exit(1);
		}
	}

	bool is_call_message(int type) {
		switch (type) {
		case EventRouteRequest:
		case EventRouteUsed:
		case EventCallCreated:
		case EventCallDeleted:
		case EventAgentReserved:
			return true;
		default:
			return false;
		}
	}

	bool is_dn_message(int type) {
		return !(is_call_message(type));
	}

	void produce_event(string request_id, string msg) {

		int req_id = GetRequestEnumType(request_id);
		string topic;
		if (is_dn_message(req_id)) {
			topic = dn_topic;
		}
		else {
			topic = call_topic;
		}
		string line = "test";
		RdKafka::ErrorCode err = producer->produce(
			/* Topic name */
			topic,
			/* Any Partition: the builtin partitioner will be
			* used to assign the message to a topic based
			* on the message key, or random partition if
			* the key is not set. */
			RdKafka::Topic::PARTITION_UA,
			/* Make a copy of the value */
			RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
			/* Value */
			const_cast<char *>(msg.c_str()), msg.size(),
			/* Key */
			NULL, 0,
			/* Timestamp (defaults to current time) */
			0,
			/* Message headers, if any */
			NULL,
			/* Per-message opaque value passed to
			* delivery report */
			NULL);

		if (err != RdKafka::ERR_NO_ERROR) {
			std::cerr << "% Failed to produce to topic " << topic << ": "
				<< RdKafka::err2str(err) << std::endl;
		}
		else {
			std::cerr << "% Enqueued message to topic " << topic << "(" << line.size() << " bytes) "
				<< "for topic " << topic << std::endl;
		}
		producer->poll(0);
	}
};

class RedisWrapper {
public:
	//RedisCluster *redis_cluster;
	RedisWrapper() {
		//*redis_cluster = RedisCluster("tcp://127.0.0.1:7100");
		int last_id = 0;
	}
	string parse_request_id(string msg) {
		json j = "{ \"id\": \"EventCallCreated\", \"EventSequenceNumber\" : \"0000000000000017\"}"_json;
		json q = json::parse(msg);
		return q["id"];
		/*for (json::iterator it = j.begin(); it != j.end(); ++it) {
			std::cout << it.key() << " : " << it.value() << "\n";
		}*/
	}
};

int main(int argc, char **argv) {
	try {

		string brokers = "localhost";
		string topic = "testtopic";
		// Create a RedisCluster object, which is movable but NOT copyable.
		RedisWrapper redisObj;
		kafkaProducer kafka;


		// Add an item into the stream. This method returns the auto generated id.

		//auto id = redis_cluster.xadd("TestStreamNew", "*", attrs.begin(), attrs.end());

		string redis_address = "tcp://127.0.0.1:7100";
		auto redis_cluster = RedisCluster(redis_address);

		/*using Item = std::pair<std::string, std::unordered_map<std::string, std::string>>;
		using Result = std::unordered_map<std::string, std::vector<Item>>;
		Result result;*/
		string last_id = "0-0";

		while (1) {
			using Item = std::pair<std::string, std::unordered_map<std::string, std::string>>;
			using Result = std::unordered_map<std::string, std::vector<Item>>;
			Result result;
			redis_cluster.xread("TenantStream:2c5ea4c0-4067-11e9-8bad-9b1deb4d3b7d", last_id, 100, std::inserter(result, result.end()));
			if (result.size() > 0) {
				auto v1 = result["TenantStream:2c5ea4c0-4067-11e9-8bad-9b1deb4d3b7d"].begin();

				auto size = result["TenantStream:2c5ea4c0-4067-11e9-8bad-9b1deb4d3b7d"].size();
				cout << "size is" << size << endl;

				for (auto it = 0; it < size; it++)
				{
					auto tempnew = result["TenantStream:2c5ea4c0-4067-11e9-8bad-9b1deb4d3b7d"][it].second;
					cout << "xread event is" << tempnew["tevent"] << endl << endl;
					auto id = tempnew["tevent"];
					string request_id = redisObj.parse_request_id(id);

					kafka.produce_event(request_id, id);
					cout << request_id << endl;
					if (size - it == 1) {
						last_id = result["TenantStream:2c5ea4c0-4067-11e9-8bad-9b1deb4d3b7d"][it].first;
					}
				}
				cout << "last id set is" << last_id << endl;
				result.empty();
			}
			Sleep(1000);
		}

	}
	catch (const Error &e) {
		std::cout << "error in redis" << std::endl;
	}
	return 0;
}