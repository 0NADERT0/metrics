#include "httplib.h"
#include <iostream>


void make_requests(int count) {
    httplib::Client cli("http://localhost:8080");
    
    for (int i = 0; i < count; i++) {
        auto res = cli.Get("/");
        if (res && res->status == 200) {
            std::cout << "Request " << i+1 << ": " << res->body << "\n";
        }
        else {
            std::cerr << "Request failed\n";
        }
    }
}

int main() {
    make_requests(1000000);
    return 0;
}