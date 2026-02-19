#include <filesystem>
#include "fileverification.hpp"

void FileVerification::isAccess(const std::string &fileName, std::ofstream &file)
{
    isDirExists(fileName);
    isOpen(file);
}

void FileVerification::isWrite(std::ofstream &file){
    if (file.fail()) {
        file.clear();
        throw std::runtime_error("Error writing data to file");
    }
}

void FileVerification::isDirExists(const std::string &fileName)
{
    namespace fs = std::filesystem;

    fs::path filePath(fileName);
            
    fs::path dir = filePath.parent_path();
    if (!dir.empty() && !fs::exists(dir)) {
        throw std::runtime_error("Directory does not exist");
    }
}

void FileVerification::isOpen(std::ofstream& file){
    if (!file) {
        throw std::runtime_error("Cannot open file");
    }
}