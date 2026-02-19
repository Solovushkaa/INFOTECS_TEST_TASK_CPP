#ifndef FILEVERIFICATION_H
#define FILEVERIFICATION_H

#include <fstream>

/**
 * @brief Class for checking work with the file system.
 */
class FileVerification 
{
    // --- Public Methods ---
public:
    /**
     * @brief Checks access to the selected file.
     */
    void isAccess(const std::string &fileName, std::ofstream &file);
    /**
     * @brief Checks whether writing to a file was successful.
     */
    void isWrite(std::ofstream &file);
    
    // --- Private Methods ---
private:
    /**
     * @brief Checks for the existence of a directory.
     */
    void isDirExists(const std::string &fileName);
    /**
     * @brief Checks if a file is open.
     */
    void isOpen(std::ofstream &file);
};

#endif // FILEVERIFICATION_H