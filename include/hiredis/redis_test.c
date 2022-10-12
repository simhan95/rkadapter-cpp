#include "hiredis.h"

redisContext *c = redisConnect("127.0.0.1", 7100);
if (c != NULL && c->err) {
    printf("Error: %s\n", c->errstr);
    // handle error
} else {
    printf("Connected to Redis\n");
}

