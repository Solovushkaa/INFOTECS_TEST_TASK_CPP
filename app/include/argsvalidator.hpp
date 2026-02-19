#ifndef ARGSVALIDATOR_HPP
#define ARGSVALIDATOR_HPP

#include "loglevel.hpp"
#include <string>
#include <utility>

/**
 * @brief Class for validating application parameters.
 */
class ArgsValidator
{
    // --- Constructors ---
public:
    ArgsValidator(int argc, char const *argv[]);

    // --- Public Methods ---
public:
    /**
     * @brief Indicates that the parameters are correct.
     */
    bool is_valid() { return m_argc > 1; }
    /**
     * @brief Passes application parameters in the required form.
     */
    auto getArgs() -> std::pair<std::string, LogLevel>;

    // --- Members ---
private:
    int m_argc;     ///< Number of application parameters
    char const **m_argv;  ///< Application parameters
};

#endif // ARGSVALIDATOR_HPP