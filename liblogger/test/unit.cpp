#include <fstream>
#include <filesystem>
#include "minitestframework.hpp"
#include "logger.hpp"

// constants
std::string prFile;
std::string pubFile;
// ---------

// --- Log level tests ---
TEST(testLogLevelConstants)
{
    ASSERT_EQ(constants::INFO, "INFO");
    ASSERT_EQ(constants::WARNING, "WARNING");
    ASSERT_EQ(constants::ERROR, "ERROR");
}

TEST(testLogLevelAsString)
{
    ASSERT_EQ(LogLevelAsString(LogLevel::INFO), constants::INFO);
}

TEST(testLogLevelFromString)
{
    ASSERT_EQ(LogLevelFromString(constants::INFO), LogLevel::INFO);
}

// --- File verification tests ---
TEST(testFileVerificationisAccessPrivateFile)
{
    FileVerification fileVer;

    std::ofstream file(prFile);
    bool flag = false;
    try {
        fileVer.isAccess(prFile, file);
    } catch (...) { flag = true; }
    ASSERT_TRUE(flag);
}

TEST(testFileVerificationisAccessPublicFile)
{
    FileVerification fileVer;

    std::ofstream file(pubFile);
    fileVer.isAccess(pubFile, file);
}

TEST(testFileVerificationisCanCreateFile)
{
    FileVerification fileVer;

    std::filesystem::path path = pubFile;
    std::string testFile = path.parent_path().string()+"/file.txt";

    std::ofstream file(testFile);
    fileVer.isAccess(testFile, file);

    std::ifstream fin(testFile);
    ASSERT_TRUE(fin.is_open());
}

TEST(testFileVerificationisWrite)
{
    FileVerification fileVer;

    std::ofstream fout(pubFile, std::ios::out | std::ios::trunc);
    std::string originalText = "Test text";
    fout << originalText;

    fout.flush();
    fileVer.isWrite(fout);

    fout.close();

    std::ifstream fin(pubFile);
    std::string receivedText;
    std::getline(fin, receivedText);

    ASSERT_EQ(originalText, receivedText);
}

// --- Logger tests ---
TEST(testLoggerCreateLogWithOneParam)
{
    std::ofstream fout(pubFile, std::ios::out | std::ios::trunc);
    fout.close();

    std::string logMessage = "Some message";
    Logger logger(pubFile);
    logger.createLog(logMessage);
    logger.flushBuffer();

    std::ifstream fin(pubFile);
    std::string log;
    std::getline(fin, log);

    ASSERT_TRUE(log.find(logMessage) != std::string::npos);
}

TEST(testLoggerCreateLogWithTwoParam)
{
    std::ofstream fout(pubFile, std::ios::out | std::ios::trunc);
    fout.close();

    std::string logMessage = "Some message";
    LogLevel logLevel = LogLevel::ERROR;
    Logger logger(pubFile);
    logger.createLog(logMessage, logLevel);
    logger.flushBuffer();

    std::ifstream fin(pubFile);
    std::string log;
    std::getline(fin, log);

    ASSERT_TRUE(log.find(constants::ERROR) != std::string::npos);
}

TEST(testLoggerCreateLogWithLogLevelLowerThanDefault)
{
    std::ofstream fout(pubFile, std::ios::out | std::ios::trunc);
    fout.close();

    std::string logMessage = "Some message";
    LogLevel logLevel = LogLevel::ERROR;
    Logger logger(pubFile, logLevel);
    logger.createLog(logMessage, LogLevel::INFO);
    logger.flushBuffer();

    std::ifstream fin(pubFile);
    std::string log;
    std::getline(fin, log);

    ASSERT_TRUE(log.size() == 0);
}

TEST(testLoggerChangeDefaultLogLevel)
{
    Logger logger(pubFile, LogLevel::WARNING);

    LogLevel originlLogLevel = logger.getDefaultLogLevel();
    logger.setDefaultLogLevel(LogLevel::INFO);

    ASSERT_TRUE(originlLogLevel != logger.getDefaultLogLevel());
}

int main(int argc, char **argv)
{
    if(argc < 3) {
        setRedText();
        std::cout << "Invalid arguments!\n";
        setDefaultText();
        return 1;
    }
    prFile = argv[1];
    pubFile = argv[2];
    
    run_all_tests("Start 'liblogger' library tests:");
}
