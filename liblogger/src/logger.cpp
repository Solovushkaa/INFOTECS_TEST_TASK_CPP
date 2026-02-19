#include <chrono>
#include <fstream>
#include <filesystem>
#include "logger.hpp"

Logger::Logger(const std::string &logFileName, LogLevel defaultLogLevel) 
    : m_logFileName(logFileName)
    , m_defaultLogLevel(defaultLogLevel)
    , m_logFile(m_logFileName, std::ios::app)
{
    m_fileVerification.isAccess(m_logFileName, m_logFile);
}

void Logger::createLog(const std::string &text)
{
    createLog(text, m_defaultLogLevel);
}

void Logger::createLog(const std::string &text, LogLevel logLevel)
{
    if(logLevel < m_defaultLogLevel) return;

    m_fileVerification.isAccess(m_logFileName, m_logFile);

    m_logFile << '[' << LogLevelAsString(logLevel) << "][";
    outputCurrentTime();
    m_logFile << "]: " << text << ".\n";

    m_fileVerification.isWrite(m_logFile);
}

void Logger::clearLog()
{
    m_logFile.seekp(0, std::ios::beg);
    m_logFile.clear();

    std::filesystem::resize_file(m_logFileName, 0);
}

void Logger::outputCurrentTime()
{
    using std::chrono::system_clock;

    auto currentTime = system_clock::now();
    auto currentTTime = system_clock::to_time_t(currentTime);
    auto localtime = std::localtime(&currentTTime);

    m_logFile << std::put_time(localtime, "%Y-%m-%d, %H:%M:%S");
}