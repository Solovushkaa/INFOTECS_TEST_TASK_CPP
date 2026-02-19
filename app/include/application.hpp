#ifndef APPLICATIONCORE_HPP
#define APPLICATIONCORE_HPP

#include <functional>
#include <vector>
#include "logger.hpp"
#include "renderer.hpp"
#include "worker.hpp"

/**
 * @brief The 'Application' class in which all application logic is implemented.
 */
class Application
{
    // --- Constructors ---
public:
    Application(std::pair<std::string, LogLevel> &loggerParams, char exitSymbol);

    // --- Public Methods ---
public:
    /**
     * @brief Starts the main loop of the application.
     */
    int exec();

    // --- Private Methods ---
private:
    /**
     * @brief Initializes the main menu.
     */
    void initialize();

    /**
     * @brief Create a log.
     */
    void createLog();

    /**
     * @brief Changes the default logging level.
     */
    void changeDefaultLogLevel();
    /**
     * @brief Calls up a menu for selecting the logging level.
     */
    LogLevel selectLogLevel();

    /**
     * @brief Clears the log file.
     */
    void clearLogFile();

    /**
     * @brief Converts the input number from character type to integer type.
     */
    int getInputIndex(char cIndex);
    
    /**
     * @brief Function for execution main menu tasks.
     */
    void executionBlock(int inputIndex);
    
    /**
     * @brief Skips characters that interfere with input.
     */
    void setCinIgnore();

    // --- Members ---
private:
    Commands m_mainMenu; ///< The main menu of the application
    Renderer m_renderer; ///< Application renderer

    Logger m_logger;        ///< Application logging system
    const char m_exitSymbol;  ///< Exit symbol

    Worker m_worker; ///< Application command handler
};

#endif // APPLICATIONCORE_HPP