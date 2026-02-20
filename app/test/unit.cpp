#include <vector>
#include <sstream>
#include <numeric>
#include <filesystem>
#include "minitestframework.hpp"
#include "argsvalidator.hpp"
#include "application.hpp"

using namespace std::chrono_literals;
namespace tt = std::this_thread;

// constants
constexpr int mainMenuCommandCreateLog = 1;
constexpr int mainMenuCommandChangeLogLevel = 2;
constexpr int mainMenuCommandClearLog = 3;
constexpr int logLevel = static_cast<int>(LogLevel::INFO)+1;
constexpr char exitSymbol = 'q';
constexpr char acceptSymbol = 'y';

const std::string logText = "Text";
std::string pathToUnitTestApp;
std::string fileName;

std::pair<std::string, LogLevel> loggerParams;
// ---------

class File
{
public:
    void clear() {
        m_file.clear();
        m_file.open(fileName, std::ios::out | std::ios::trunc);
        m_file.close();
    }
    void input(std::string &newLogFile) {
        m_file.clear();
        m_file.open(fileName, std::ios::in);
        std::getline(m_file, newLogFile);
        m_file.close();
    }
private:
    std::fstream m_file;
} file;


class StreamRedirect
{
public:
    StreamRedirect()
    {
        cinOld = std::cin.rdbuf(fakeCinBuffer.rdbuf());
        coutOld = std::cout.rdbuf(fakeCoutBuffer.rdbuf());
    }
    void setFakeCinBufferForCreateLog() 
    {
        fakeCinBuffer << mainMenuCommandCreateLog << "\n";
        fakeCinBuffer << logText << "\n";
        fakeCinBuffer << logLevel << "\n";
        fakeCinBuffer << exitSymbol << "\n";
    }
    void setFakeCinBufferForChangeLogLevel()
    {
        fakeCinBuffer << mainMenuCommandChangeLogLevel << "\n";
        fakeCinBuffer << 1+logLevel << "\n"; 
        fakeCinBuffer << exitSymbol << "\n"; 
    }
    void setFakeCinBufferForClearLog()
    {
        fakeCinBuffer << mainMenuCommandClearLog << "\n";
        fakeCinBuffer << acceptSymbol << "\n"; 
        fakeCinBuffer << exitSymbol << "\n"; 
    }
    void setFakeBufferForExit(char exitSymbol)
    {
        fakeCinBuffer << exitSymbol << "\n";
    }
    std::string getStrFromCoutBuffer()
    {
        std::string buffer = fakeCoutBuffer.str();
        return std::string(buffer.begin(), buffer.end());
    }
    void backBuffersToOrigin()
    {
        std::cin.rdbuf(cinOld);
        std::cout.rdbuf(coutOld);
        std::cin.clear();
        std::cout.clear();
    }
    ~StreamRedirect()
    {
        backBuffersToOrigin();
    }

private:
    std::streambuf* cinOld;
    std::streambuf* coutOld;
    std::stringstream fakeCinBuffer;
    std::stringstream fakeCoutBuffer;
};

// --- Worker tests ---

constexpr int numIteration = 1'000;

TEST(testWorkerTaskQueue)
{
    int value;
    auto fakeFun = [&value](int i){ value = i; };
    Worker worker;

    for(int i = 0; i < numIteration; ++i){
        worker.execute(std::function<void()>([i, &fakeFun](){
            fakeFun(i);
        }));
    }

    ASSERT_TRUE(numIteration != value+1);
}

TEST(testWorkerSynchronizationExecutor)
{   
    std::vector<int> valuesFromWorker;
    valuesFromWorker.reserve(numIteration);

    std::vector<int> correctValuesOrderFromWorker(numIteration);
    std::iota(correctValuesOrderFromWorker.begin(), correctValuesOrderFromWorker.end(), 0);
    
    auto fakeFun = [&valuesFromWorker](int i){ valuesFromWorker.push_back(i); };
    Worker worker;

    for(int i = 0; i < numIteration; ++i){
        worker.execute(std::function<void()>([i, &fakeFun](){
            fakeFun(i);
        }));
    }

    tt::sleep_for(100ms); // Waitiing to the worker thread to complete

    for(int i = 0; i < numIteration; ++i){
        ASSERT_TRUE(valuesFromWorker[i] == correctValuesOrderFromWorker[i]);
    }
}

// --- ArgsValidator tests ---

constexpr const int argc = 3;
char const *argv[] = {
    "./program",
    "file.txt",
    "INFO"
};

TEST(testArgsValidatorIsValid)
{
    ArgsValidator argsValidator(argc, argv);

    ASSERT_TRUE(argsValidator.is_valid());
}

TEST(testArgsValidatorGetArgs)
{
    std::string fileName = "file.txt";
    std::string stringLogLevel = "INFO";
    
    ArgsValidator argsValidator(argc, argv);

    auto args = argsValidator.getArgs();
    ASSERT_EQ(args.first, fileName);
    ASSERT_EQ(args.second, LogLevelFromString(stringLogLevel))
}

// --- Commands tests ---
TEST(testCommandsAdd)
{
    Commands commands;

    std::string commandName;
    int value;
    std::function<void()> command([&value](){
        value = 1234;
    });
    commands.add(std::move(commandName), std::move(command));

    ASSERT_TRUE(commands.size() == 1);
}

TEST(testCommandsSetGetInfo)
{
    Commands commands;

    const std::string info = "Inforamation";
    std::string moveInfo = info;
    commands.setInfo(std::move(moveInfo));

    ASSERT_EQ(commands.getInfo(), info);
}

TEST(testCommandsActivateElement)
{
    Commands commands;

    std::string commandName;
    const int value = 1234;
    int changesValue = 0;
    std::function<void()> command([&changesValue, &value](){
        changesValue = value;
    });
    commands.add(std::move(commandName), std::move(command));
    commands.begin()->second();

    ASSERT_EQ(changesValue, value);
}

// --- Application tests ---
TEST(testApplicationExecCreateNewLog)
{
    // file.clear();

    Application app(loggerParams, exitSymbol);

    StreamRedirect streamRedirect;
    streamRedirect.setFakeCinBufferForCreateLog();

    app.exec();

    tt::sleep_for(30ms); // Waitiing to the worker thread to complete
    
    std::string newLogFile;
    file.input(newLogFile);

    bool isLogFileMatchInputLog = (newLogFile.find(logText) != std::string::npos)
        && (newLogFile.find(LogLevelAsString(static_cast<LogLevel>(logLevel-1))) != std::string::npos);
    ASSERT_TRUE(isLogFileMatchInputLog);
}

TEST(testApplicationExecChangeDefaultLogLevel)
{
    file.clear();

    Application app(loggerParams, exitSymbol);

    // Changing LogLevel
    StreamRedirect streamRedirect;
    streamRedirect.setFakeCinBufferForChangeLogLevel();

    app.exec();

    tt::sleep_for(30ms); // Waitiing to the worker thread to complete
    // ----------------

    // Try to create log
    streamRedirect.setFakeCinBufferForCreateLog();

    app.exec();

    tt::sleep_for(30ms); // Waitiing to the worker thread to complete
    // ----------------
    
    std::string newLogFile;
    file.input(newLogFile);

    std::filesystem::remove(fileName);

    ASSERT_EQ(newLogFile.size(), 0);
}

TEST(testApplicationExecClearLogFile)
{
    file.clear();

    Application app(loggerParams, exitSymbol);

    // Create log
    StreamRedirect streamRedirect;
    streamRedirect.setFakeCinBufferForCreateLog();

    app.exec();

    tt::sleep_for(30ms); // Waitiing to the worker thread to complete
    // ----------------

    // Clear log
    streamRedirect.setFakeCinBufferForClearLog();

    app.exec();

    tt::sleep_for(30ms); // Waitiing to the worker thread to complete
    // ----------------

    std::string newLogFile;
    file.input(newLogFile);

    std::filesystem::remove(fileName);

    ASSERT_EQ(newLogFile.size(), 0);
}

// --- Renders tests ---

const std::string mainOutput = {
    "\033[2J\033[H+--------------------+\n"
    "| To exit, press 'q' |\n"
    "+--------------------+\n"
    "1.Сreate a new log message\n"
    "2.Change default log level\n"
    "3.Clear log file\n"
    "Enter to continue...\n"
};

const std::string logMessageOutput = {
    "\033[2J\033[HEnter message: "
};

const std::string logLevelOutput = {
    "\033[2J\033[HSelect log level:\n"
    "1.INFO\n"
    "2.WARNING\n"
    "3.ERROR\n"
    "4.Keep selected (INFO)\n"
    "Enter to continue...\n"
};

const std::string logClearOutput = {
    "\033[2J\033[HTo clear the logging file, enter (y/Y) or enter (n/N) to cancel: "
};

TEST(testRendererDrawMainLayer)
{
    StreamRedirect streamRedirect;

    Application app(loggerParams, exitSymbol);

    streamRedirect.setFakeBufferForExit(exitSymbol);

    app.exec();

    std::string realOutput = streamRedirect.getStrFromCoutBuffer();
    std::string originOutput = mainOutput;

    ASSERT_EQ(originOutput, realOutput);
}

TEST(testRendererDrawCreateLog)
{
    StreamRedirect streamRedirect;
    
    streamRedirect.setFakeCinBufferForCreateLog();

    Application app(loggerParams, exitSymbol);

    app.exec();

    std::string realOutput = streamRedirect.getStrFromCoutBuffer();
    std::string originOutput = mainOutput + logMessageOutput + logLevelOutput + mainOutput;

    ASSERT_EQ(originOutput, realOutput);
}

TEST(testRendererDrawConfrim)
{
    StreamRedirect streamRedirect;
    
    streamRedirect.setFakeCinBufferForClearLog();

    Application app(loggerParams, exitSymbol);

    app.exec();

    std::string realOutput = streamRedirect.getStrFromCoutBuffer();
    std::string originOutput = mainOutput + logClearOutput + mainOutput;

    ASSERT_EQ(originOutput, realOutput);
}

int main(int argc, char **argv)
{
    if(argc < 2) {
        setRedText();
        std::cout << "Invalid arguments!\n";
        setDefaultText();
        return 1;
    }

    pathToUnitTestApp = argv[1];
    fileName = pathToUnitTestApp+"file.txt";
    loggerParams = {fileName, LogLevel::INFO};

    run_all_tests("Start 'logger' app tests:");

    std::filesystem::remove(fileName);
}