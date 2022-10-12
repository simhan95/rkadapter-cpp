/**************************************************************************
   Copyright (c) 2020 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#include "tls.h"
#include "errors.h"

typedef struct {
    const char *cacert_filename;
    const char *capath;
    const char *cert_filename;
    const char *private_key_filename;
    const char *server_name;
    int verify_mode;
} redisSSLOptions;

namespace sw {

namespace redis {

namespace tls {

bool& auto_init() {
    static bool init = true;

    return init;
}

void disable_auto_init() {
    auto_init() = false;
}

TlsInit::TlsInit() {
    if (auto_init()) {
        redisInitOpenSSL();
    }
}

TlsContextUPtr secure_connection(redisContext &ctx, const TlsOptions &opts) {
    static TlsInit tls_init;

    auto c_str = [](const std::string &s) {
        return s.empty() ? nullptr : s.c_str();
    };

    redisSSLContextError err;
    redisSSLOptions options = {
        .cacert_filename = c_str(opts.cacert),
        .capath = c_str(opts.cacertdir),
        .cert_filename = c_str(opts.cert),
        .private_key_filename = c_str(opts.key),
        .server_name = c_str(opts.sni),
        .verify_mode = REDIS_SSL_VERIFY_NONE,
    };
    auto tls_ctx = TlsContextUPtr(redisCreateSSLContextWithOptions(&options, &err));
    if (!tls_ctx) {
        throw Error(std::string("failed to create TLS context: ")
                    + redisSSLContextGetError(err));
    }

    if (redisInitiateSSLWithContext(&ctx, tls_ctx.get()) != REDIS_OK) {
        throw_error(ctx, "Failed to initialize TLS connection");
    }

    return tls_ctx;
}

}

}

}
