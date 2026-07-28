#pragma once

#include <memory>
#include <string>

#include <spdlog/spdlog.h>

namespace gui_shell
{

  /// Thin wrapper around a single named spdlog logger used throughout the
  /// module. Consumers may call Init() once at startup to control the name
  /// and verbosity; if they don't, Get() will lazily initialize sane defaults.
  class Logger
  {
  public:
    static void Init(const std::string& name = "GuiShell", spdlog::level::level_enum level = spdlog::level::info);

    static std::shared_ptr<spdlog::logger>& Get();

  private:
    static std::shared_ptr<spdlog::logger> s_logger;
  };

} // namespace gui_shell

#define GS_TRACE(...)    ::gui_shell::Logger::Get()->trace(__VA_ARGS__)
#define GS_INFO(...)     ::gui_shell::Logger::Get()->info(__VA_ARGS__)
#define GS_WARN(...)     ::gui_shell::Logger::Get()->warn(__VA_ARGS__)
#define GS_ERROR(...)    ::gui_shell::Logger::Get()->error(__VA_ARGS__)
#define GS_CRITICAL(...) ::gui_shell::Logger::Get()->critical(__VA_ARGS__)
