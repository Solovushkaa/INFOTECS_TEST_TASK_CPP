#include "argsvalidator.hpp"

ArgsValidator::ArgsValidator(int argc, char const *argv[])
    : m_argc(argc)
    , m_argv(argv)
{}

auto ArgsValidator::getArgs() -> std::pair<std::string, LogLevel>
{
    std::pair<std::string, LogLevel> params;
    params.first = m_argv[1]; // Name of the log file
    if (m_argc == 2) {
        params.second = LogLevel::INFO;
    } else {
        std::string logLevel = m_argv[2];
        params.second = LogLevelFromString(logLevel);
    }

    return params;
}
