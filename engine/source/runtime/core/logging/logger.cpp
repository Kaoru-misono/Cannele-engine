#include "logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace cannele::core::log
{
    auto Logger::instance() -> Logger&
    {
        static Logger global_logger{};
        return global_logger;
    }

    Logger::Logger()
    {
        auto sink_console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sink_console->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%l] %v%$");
        sink_console->set_level(spdlog::level::trace);

        auto sink_file = std::make_shared<spdlog::sinks::basic_file_sink_mt>("cannele.log", true);
        sink_file->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%l] %v%$");


        const spdlog::sinks_init_list sink_list = {sink_console, sink_file};

        logger = std::make_shared<spdlog::logger>("cannele_logger", sink_list.begin(), sink_list.end());
        logger->set_level(spdlog::level::trace);

        spdlog::register_logger(logger);
    }
}
