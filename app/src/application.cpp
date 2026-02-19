#include "application.hpp"
#include <iostream>

Application::Application(std::pair<std::string, LogLevel> &loggerParams, char exitSymbol) 
    : m_mainMenu()
    , m_renderer(&m_mainMenu)
    , m_logger(loggerParams.first, loggerParams.second)
    , m_exitSymbol(exitSymbol)
{
    initialize();
}

void Application::initialize()
{
    m_mainMenu.add("Сreate a new log message", std::function<void()>([this](){ this->createLog(); }));
    m_mainMenu.add("Change default log level", std::function<void()>([this](){ this->changeDefaultLogLevel(); }));
    m_mainMenu.add("Clear log file", std::function<void()>([this](){ this->clearLogFile(); }));
    m_mainMenu.setInfo(R"(To exit, press 'q')");
}

void Application::createLog()
{
    m_renderer.drawLogTextInput();

    std::string message;
    setCinIgnore();
    std::getline(std::cin, message);

    LogLevel logLevel = selectLogLevel();

    std::function<void()> func = 
        [this, message, logLevel]() 
        { 
            m_logger.createLog(message, logLevel); 
            m_logger.flushBuffer();
        };

    m_worker.execute(std::move(func));
}

void Application::changeDefaultLogLevel()
{
    LogLevel logLevel = selectLogLevel();
    std::function<void()> func = [this, logLevel]() { m_logger.setDefaultLogLevel(logLevel); };

    m_worker.execute(std::move(func));
}

LogLevel Application::selectLogLevel()
{
    LogLevel logLevel = m_logger.getDefaultLogLevel();

    
    char input;
    while(true)
    {
        m_renderer.drawLogLevels(logLevel);

        std::cin >> input;
        int index = getInputIndex(input);

        if(index >= 0 && index < static_cast<int>(LogLevel::COUNT)) {
            logLevel = static_cast<LogLevel>(index);
            break;
        } else if(index == static_cast<int>(LogLevel::COUNT)) {
            break;
        }
    }

    return logLevel;
}

void Application::clearLogFile()
{
    char input;
    
    while(true)
    {
        m_renderer.drawConfrimClearLogFile();

        std::cin >> input;
        
        if(input == 'y' || input == 'Y'){
            m_logger.clearLog();
            break;
        } else if(input == 'n' || input == 'N'){
            break;
        }
    }
}

int Application::getInputIndex(char cIndex)
{
    return static_cast<int>(cIndex-'1');
}

void Application::executionBlock(int inputIndex)
{
    try {
        m_mainMenu.activateEvent(inputIndex);
    } 
    catch (std::exception& err) {
        m_renderer.drawErrorText(err.what());
    }
}

void Application::setCinIgnore()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int Application::exec()
{
    char input = ' ';
    while(true)
    {
        m_renderer.drawMainLayer();

        std::cin >> input;
        if(input == m_exitSymbol){
            break;
        }
        int index = getInputIndex(input);
        if(index >= 0 && index <= m_mainMenu.size()){
            executionBlock(index);
        }
    }

    return 0;
}