#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "Core.h"

// Engine log macros
#define DC_CORE_TRACE(...)      ::DuckCSG::Log::getLogger()->trace(__VA_ARGS__)
#define DC_CORE_INFO(...)       ::DuckCSG::Log::getLogger()->info(__VA_ARGS__)
#define DC_CORE_WARN(...)       ::DuckCSG::Log::getLogger()->warn(__VA_ARGS__)
#define DC_CORE_ERROR(...)      ::DuckCSG::Log::getLogger()->error(__VA_ARGS__)
#define DC_CORE_CRITICAL(...)   ::DuckCSG::Log::getLogger()->critical(__VA_ARGS__)

namespace DuckCSG {

    class Log
    {
    public:
        static void init();
        inline static std::shared_ptr<spdlog::logger>& getLogger() { if (!s_logger) { abort(); }; return s_logger; }
    private:
        static std::shared_ptr<spdlog::logger> s_logger;
    };
}
