#include "loglevel.hpp"
#include <stdexcept>

std::string LogLevelAsString(LogLevel logLevel)
{
    switch (logLevel)
    {
    case LogLevel::INFO:
        return constants::INFO;
    case LogLevel::WARNING:
        return constants::WARNING;
    case LogLevel::ERROR:
        return constants::ERROR;
    case LogLevel::COUNT:
        throw std::runtime_error("'COUNT' is a size of 'LogLevel'");
    }
    throw std::runtime_error("Unknown type of 'LogLevel'");
}

LogLevel LogLevelFromString(std::string_view logLevel)
{
    if (logLevel == constants::INFO) {
        return LogLevel::INFO;
    } else if (logLevel == constants::WARNING) {
        return LogLevel::WARNING;
    } else if (logLevel == constants::ERROR) {
        return LogLevel::ERROR;
    }
    throw std::runtime_error("Unknown type of 'LogLevel'");
}