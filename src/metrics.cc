#include "metrics.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>


void Metrics::increment_request() { 
    ++request_count_; 
}

double Metrics::get_cpu_usage() {
    std::ifstream proc_stat("/proc/stat");
    std::string line;
    std::getline(proc_stat, line);
    std::istringstream iss(line.substr(5));

    unsigned long user, nice, system, idle;
    iss >> user >> nice >> system >> idle;

    unsigned long total = user + nice + system + idle;
    static unsigned long prev_total = 0, prev_idle = idle;

    unsigned long diff_total = total - prev_total;
    unsigned long diff_idle = idle - prev_idle;

    prev_total = total;
    prev_idle = idle;

    double core_count = 6;

    return (diff_total > 0) ? (core_count *(diff_total-diff_idle)/diff_total) : 0.0;
}

void Metrics::log_metrics() {
    while (running_) {
        int rps = request_count_.exchange(0);
        double cpu_usage = get_cpu_usage();

        {
            std::lock_guard<std::mutex> lock(metrics_mutex_);
            metrics_file_ << std::fixed << std::setprecision(2);
            metrics_file_ << get_timestamp() << " " << "CPU: " << cpu_usage << ", HTTP REQUESTS RPS: " << rps << std::endl;
            metrics_file_.flush();
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

std::string Metrics::get_timestamp() {
    auto now = std::chrono::system_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
    now.time_since_epoch()) % 1000;

    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S")
       << "." << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}