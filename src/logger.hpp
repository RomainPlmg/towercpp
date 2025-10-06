#pragma once

#include <spdlog/spdlog.h>

namespace towercpp {

class Logger {
   public:
    static void init();
    static spdlog::logger& get();
    static void shutdown();
};

}  // namespace towercpp

#define LOG_TRACE(...) ::towercpp::Logger::get().trace(__VA_ARGS__)
#define LOG_DEBUG(...) ::towercpp::Logger::get().debug(__VA_ARGS__)
#define LOG_INFO(...) ::towercpp::Logger::get().info(__VA_ARGS__)
#define LOG_WARN(...) ::towercpp::Logger::get().warn(__VA_ARGS__)
#define LOG_ERROR(...) ::towercpp::Logger::get().error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::towercpp::Logger::get().critical(__VA_ARGS__)