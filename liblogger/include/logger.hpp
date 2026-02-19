#ifndef LOG_H
#define LOG_H

#include "fileverification.hpp"
#include "loglevel.hpp"
#include <string>

/**
 * @brief Class for creating log files.
 */
class Logger
{
    // --- Constructors ---
public:
    Logger(const std::string &logFileName, LogLevel defaultLogLevel = LogLevel::INFO);

    // --- Public Methods ---
public:
    /**
     * @brief Creates a log from a text message. LogLevel is used by default.
     */
    void createLog(const std::string &text);
    /**
     * @brief Creates a log from a text message and LogLevel.
     */
    void createLog(const std::string &text, LogLevel logLevel);

    /**
     * @brief Clears the log file.
     */
    void clearLog();

    /**
     * @brief Sets the default LogLevel.
     */
    void setDefaultLogLevel(LogLevel logLevel) { m_defaultLogLevel = logLevel; }
    /**
     * @brief Provides a default LogLevel.
     */
    LogLevel getDefaultLogLevel() const { return m_defaultLogLevel; }

    /**
     * @brief Flush the file buffer/
     */
    void flushBuffer() { m_logFile.flush(); }

    // --- Private Methods ---
private:
    /**
     * @brief Output the current time to the log file.
     */
    void outputCurrentTime();

    // --- Members ---
private:
    std::string m_logFileName;           ///< Log file name
    LogLevel m_defaultLogLevel;          ///< Logging level
    FileVerification m_fileVerification; ///< File handling verifier

    std::ofstream m_logFile; ///< Log file
};

#endif // LOG_H