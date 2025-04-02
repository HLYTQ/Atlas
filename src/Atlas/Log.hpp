#pragma once

#include "spdlog/spdlog.h"

namespace Atlas {

class Log {
public:
    static void init();

    static std::shared_ptr<spdlog::logger>& get_core_logger() {
        return s_CoreLogger;
    }
    static std::shared_ptr<spdlog::logger>& get_client_logger() {
        return s_ClientLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace Atlas

// core log macros
#define AT_CORE_TRACE(...)    ::Atlas::Log::get_core_logger()->trace(__VA_ARGS__);
#define AT_CORE_INFO(...)     ::Atlas::Log::get_core_logger()->info(__VA_ARGS__);
#define AT_CORE_WARN(...)     ::Atlas::Log::get_core_logger()->warn(__VA_ARGS__);
#define AT_CORE_ERROR(...)    ::Atlas::Log::get_core_logger()->error(__VA_ARGS__);
#define AT_CORE_CRITICAL(...) ::Atlas::Log::get_core_logger()->critical(__VA_ARGS__);

// client log macros
#define AT_TRACE(...)    ::Atlas::Log::get_client_logger()->trace(__VA_ARGS__);
#define AT_INFO(...)     ::Atlas::Log::get_client_logger()->info(__VA_ARGS__);
#define AT_WARN(...)     ::Atlas::Log::get_client_logger()->warn(__VA_ARGS__);
#define AT_ERROR(...)    ::Atlas::Log::get_client_logger()->error(__VA_ARGS__);
#define AT_CRITICAL(...) ::Atlas::Log::get_client_logger()->critical(__VA_ARGS__);
