#pragma once

#include <spdlog/spdlog.h>

namespace cannele
{
    class Logger final
    {
    public:
        static auto instance() -> Logger&;

        template <typename... Args>
        auto debug(fmt::format_string<Args...> fmt, Args &&... args) -> void {
            logger->debug(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        auto info(fmt::format_string<Args...> fmt, Args &&... args) -> void {
            logger->info(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        auto warn(fmt::format_string<Args...> fmt, Args &&... args) -> void {
            logger->warn(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        auto error(fmt::format_string<Args...> fmt, Args &&... args) -> void {
            logger->error(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        auto critical(fmt::format_string<Args...> fmt, Args &&... args) -> void {
            logger->critical(fmt, std::forward<Args>(args)...);
            std::exit(-1);
        }
    private:
        Logger();

        std::shared_ptr<spdlog::logger> logger;
    };

#define CANNELE_DEBUG(fmt, ...)   Logger::instance().debug(fmt, ##__VA_ARGS__)
#define CANNELE_INFO(fmt, ...)    Logger::instance().info(fmt, ##__VA_ARGS__)
#define CANNELE_WARN(fmt, ...)    Logger::instance().warn(fmt, ##__VA_ARGS__)
#define CANNELE_ERROR(fmt, ...)   Logger::instance().error(fmt, ##__VA_ARGS__)
#define CANNELE_CRTICAL(fmt, ...) Logger::instance().critical(fmt, ##__VA_ARGS__)

#define CANNELE_ASSERT(expr) do { \
    if (!(expr)) { \
        CANNELE_CRTICAL("assertion failed in file '{}' line {}, expression: '{}'", \
            __FILE__, __LINE__, #expr); \
    } \
} while (false)
#define CANNELE_ASSERT_MSG(expr, msg) do { \
    if (!(expr)) { \
        CANNELE_CRTICAL("{} (file '{}' line {})", msg, __FILE__, __LINE__); \
    } \
} while (false)
}
