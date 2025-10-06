#include "logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace towercpp {

std::shared_ptr<spdlog::logger> p_core_logger;

void Logger::init() {
    std::vector<spdlog::sink_ptr> log_sinks;
    log_sinks.emplace_back(
        std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
    log_sinks.emplace_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("Chipy.log", true));

    log_sinks[0]->set_pattern("%^[%T] %n: %v%$");
    log_sinks[1]->set_pattern("[%T] [%l] %n: %v");

    p_core_logger = std::make_shared<spdlog::logger>("CHIPY", begin(log_sinks),
                                                     end(log_sinks));
    spdlog::register_logger(p_core_logger);
    p_core_logger->set_level(spdlog::level::trace);
    p_core_logger->flush_on(spdlog::level::trace);
}

spdlog::logger& Logger::get() {
    assert(p_core_logger);
    return *p_core_logger;
}

void Logger::shutdown() { spdlog::shutdown(); }

}  // namespace towercpp
