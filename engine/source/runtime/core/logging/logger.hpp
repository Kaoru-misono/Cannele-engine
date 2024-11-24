#pragma once

#include <spdlog/spdlog.h>

namespace cannele::core::log
{
    class Logger final
    {
    public:
        static auto instance() -> Logger&;

    #define CN_LOG_METHORD(level) \
        template <typename... Args> \
        auto level(fmt::format_string<Args...> fmt, Args &&... args) -> void { \
            logger->level(fmt, std::forward<Args>(args)...); \
        }

        CN_LOG_METHORD(info)
        CN_LOG_METHORD(debug)
        CN_LOG_METHORD(warn)
        CN_LOG_METHORD(error)
        CN_LOG_METHORD(critical)

    private:
        Logger();

        std::shared_ptr<spdlog::logger> logger;
    };

#define CN_DEBUG(fmt, ...)   ::cannele::core::log::Logger::instance().debug(fmt, ##__VA_ARGS__)
#define CN_INFO(fmt, ...)    ::cannele::core::log::Logger::instance().info(fmt, ##__VA_ARGS__)
#define CN_WARN(fmt, ...)    ::cannele::core::log::Logger::instance().warn(fmt, ##__VA_ARGS__)
#define CN_ERROR(fmt, ...)   ::cannele::core::log::Logger::instance().error(fmt, ##__VA_ARGS__)
#define CN_CRTICAL(fmt, ...) ::cannele::core::log::Logger::instance().critical(fmt, ##__VA_ARGS__)

#define CN_ASSERT(expr) do { \
    if (!(expr)) { \
        CN_CRTICAL("assertion failed in file '{}' line {}, expression: '{}'", \
            __FILE__, __LINE__, #expr); \
    } \
} while (false)
#define CANNELE_ASSERT_MSG(expr, msg) do { \
    if (!(expr)) { \
        CN_CRTICAL("{} (file '{}' line {})", msg, __FILE__, __LINE__); \
    } \
} while (false)
}
