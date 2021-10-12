#include "duckpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace DuckCSG {

    std::shared_ptr<spdlog::logger> Log::s_logger;

    void Log::init()
    {
        spdlog::set_pattern("%^[%T] %v%$");
        s_logger = spdlog::stdout_color_mt("DuckCSG");
        s_logger->set_level(spdlog::level::trace);
    }
}
