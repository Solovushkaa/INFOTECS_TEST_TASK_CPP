#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <string>
#include <functional>

/**
 * @brief A class for storing commands for execution with their corresponding names.
 */
class Commands
{
    // --- Public Methods ---
public:
    /**
     * @brief Adds a command to the command vector.
     */
    void add(std::string &&name, std::function<void()> &&command) { m_commands.emplace_back(std::move(name), std::move(command)); }
    /**
     * @brief Provides an iterator to the beginning of the vector.
     */
    auto begin() { return m_commands.begin(); }
    /**
     * @brief Provides an iterator to the end of the vector.
     */
    auto end() { return m_commands.end(); }
    
    /**
     * @brief Sets the information block.
     */
    void setInfo(std::string &&text) { m_infoBlock = std::move(text); }
    /**
     * @brief Provides an information block.
     */
    std::string_view getInfo() const { return m_infoBlock; }

    /**
     * @brief Activates the selected command.
     */
    void activateEvent(int commandIndex) { m_commands[commandIndex].second(); }

    /**
     * @brief Provides the size of the instruction vector.
     */
    int size() const { return m_commands.size(); }

    // --- Members ---
private:
    std::vector<std::pair<std::string, std::function<void()>>> m_commands;  ///< Vector of command
    std::string m_infoBlock;                                                ///< Information block for executable commands
};

#endif // COMMANDS_HPP