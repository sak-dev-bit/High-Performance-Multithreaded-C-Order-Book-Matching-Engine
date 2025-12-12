#include <iostream>
#include <mutex>
#include <sstream>
#include <chrono>

static std::mutex log_mtx;

void log_info(const std::string &msg) {
    std::lock_guard<std::mutex> lk(log_mtx);
    auto now = std::chrono::system_clock::now();
    auto t_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t_c);
#else
    localtime_r(&t_c, &tm);
#endif
    char buf[64];
    std::strftime(buf, sizeof(buf), "%F %T", &tm);
    std::cout << "[" << buf << "] " << msg << std::endl;
}
