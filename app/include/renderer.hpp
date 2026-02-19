#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "commands.hpp"
#include "loglevel.hpp"

/**
 * @brief Class for rendering console graphics.
 */
class Renderer
{
    // --- Constructors ---
public:
    Renderer(Commands *applicationMainMenu);

    // --- Public Methods ---
public:
    /**
     * @brief Renders the main menu.
     */
    void drawMainLayer();
    /**
     * @brief Renders text input for the log.
     */
    void drawLogTextInput();
    /**
     * @brief Renders available logging levels.
     */
    void drawLogLevels(LogLevel byDefault);
    /**
     * @brief Renders an error that occurs during program execution.
     */
    void drawErrorText(std::string_view text);
    /**
     * @brief Renders a confirmation to clear the log file.
     */
    void drawConfrimClearLogFile();

    // --- Private Methods ---
private:
    /**
     * @brief Clears the rendering layer.
     */
    void clearLayer();
    /**
     * @brief Renders the application information block.
     */
    void drawInfoBlock(std::string_view info);
    /**
     * @brief Draws a straight line of the form '+---+'.
     */
    void drawLine(int length);
    /**
     * @brief Draws text to continue.
     */
    void drawContinue();
    /**
     * @brief Renders all main menu items.
     */
    void drawSubmenus();

    // --- Members ---
private:
    Commands *m_applicationMainMenu; ///< Pointer to the main application menu
};

#endif // RENDERER_HPP