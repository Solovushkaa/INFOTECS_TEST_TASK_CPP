#ifndef LOGLEVEL_H
#define LOGLEVEL_H

#include <string>

/**
 * @brief Enum class for providing log level.
 */
enum class LogLevel
{
    INFO,    ///< Just information
    WARNING, ///< Warning
    ERROR,   ///< Error
    COUNT    ///< To detect the size of the enum
};

/**
 * @brief Namespace with string constants interpreting enum values
 */
namespace constants
{

    const std::string INFO = "INFO";
    const std::string WARNING = "WARNING";
    const std::string ERROR = "ERROR";

} // constants

/**
 * @brief Converts the LogLevel to a std::string.
 */
std::string LogLevelAsString(LogLevel logLevel);

/**
 * @brief Converts a std::string with the name of a logging level to a LogLevel.
 */
LogLevel LogLevelFromString(std::string_view logLevel);

#endif // LOGLEVEL_H