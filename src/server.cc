#include "metrics.h"
#include "httplib.h"
#include <iostream>

int request_count = 0;
Metrics metrics;

void handle_request(const httplib::Request& req, httplib::Response& res) {
    request_count++;
    metrics.increment_request();
    res.set_content(
        "Request count: " + std::to_string(request_count), 
        "text/plain"
    );
}

int main() {
    httplib::Server server;
    server.Get("/", handle_request);

    std::cout << "Server started at http://localhost:8080\n";
    server.listen("0.0.0.0", 8080);
    return 0;
}