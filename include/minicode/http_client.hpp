#pragma once

#include <string>

namespace minicode {

struct HttpResponse {
    bool ok = false;        // true if a response was received
    int status = 0;         // HTTP status code (0 if the request failed)
    std::string body;       // response body
    std::string error;      // human-readable error when ok == false
};

// Perform a blocking HTTP(S) GET.
//
// Accepts "http(s)://host[:port]/path"; a missing scheme defaults to http and
// a missing path defaults to "/". HTTPS is handled via OpenSSL (TLS), with
// server certificate verification enabled.
HttpResponse http_get(const std::string& url);

} // namespace minicode
