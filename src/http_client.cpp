#include "minicode/http_client.hpp"

#include <httplib.h>

namespace minicode {

namespace {

// Split "scheme://host[:port]/path" into a base ("scheme://host[:port]") that
// httplib::Client understands and the request path.
struct ParsedUrl {
    std::string base;
    std::string path;
};

ParsedUrl parse_url(const std::string& url) {
    std::string scheme = "http";
    std::string rest = url;

    auto sep = rest.find("://");
    if (sep != std::string::npos) {
        scheme = rest.substr(0, sep);
        rest = rest.substr(sep + 3);
    }

    std::string host_port;
    std::string path;
    auto slash = rest.find('/');
    if (slash == std::string::npos) {
        host_port = rest;
        path = "/";
    } else {
        host_port = rest.substr(0, slash);
        path = rest.substr(slash);
    }

    return {scheme + "://" + host_port, path};
}

} // namespace

HttpResponse http_get(const std::string& url) {
    ParsedUrl parsed = parse_url(url);

    httplib::Client client(parsed.base.c_str());
    client.set_follow_location(true);

    auto res = client.Get(parsed.path.c_str());
    if (!res) {
        HttpResponse out;
        out.ok = false;
        out.error = "request failed: " + httplib::to_string(res.error());
        return out;
    }

    HttpResponse out;
    out.ok = true;
    out.status = res->status;
    out.body = res->body;
    return out;
}

} // namespace minicode
