#include "gui_shell/logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace gui_shell {

std::shared_ptr<spdlog::logger> Logger::s_logger;

void Logger::Init(const std::string& name, spdlog::level::level_enum level) {
    if (s_logger) {
        return;
    }
    s_logger = spdlog::stdout_color_mt(name);
    s_logger->set_level(level);
    s_logger->set_pattern("%^[%T] %n [%l]: %v%$");
}

std::shared_ptr<spdlog::logger>& Logger::Get() {
    if (!s_logger) {
        Init();
    }
    return s_logger;
}

} // namespace gui_shell
