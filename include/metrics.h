#ifndef METRICS_H
#define METRICS_H

#include <atomic>
#include <mutex>
#include <fstream>
#include <thread>
#include <string>

class Metrics {
    public:
        Metrics(const std::string& log_file = "metrics.log") 
            : metrics_file_(log_file, std::ios::out | std::ios::trunc),
              metrics_thread_(&Metrics::log_metrics, this) {
            if (!metrics_file_.is_open()) {
                throw std::runtime_error("fail to open");
            }
        }
        
        ~Metrics() {
            running_ = false;
            if (metrics_thread_.joinable()) {
                metrics_thread_.join();
            }
            metrics_file_.close();
        }
    
        void increment_request();
    
    private:
        void log_metrics();
        double get_cpu_usage();
        std::string get_timestamp();
    
        std::atomic<int> request_count_{0};
        std::atomic<bool> running_{true};
        std::mutex metrics_mutex_;
        std::ofstream metrics_file_;
        std::thread metrics_thread_;
    };
#endif