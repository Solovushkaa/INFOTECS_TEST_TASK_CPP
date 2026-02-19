#include <vector>
#include <sstream>
#include <numeric>
#include <filesystem>
#include "minitestframework.hpp"
#include "argsvalidator.hpp"
#include "application.hpp"

// --- Worker tests ---
TEST(testWorkerTaskQueue)
{
    const int numIteration = 1'000;
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
    const int numIteration = 1'000;
    
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

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    for(int i = 0; i < numIteration; ++i){
        ASSERT_TRUE(valuesFromWorker[i] == correctValuesOrderFromWorker[i]);
    }
}

// --- ArgsValidator tests ---
TEST(testArgsValidatorIsValid)
{
    int argc = 3;
    char const *argv[] = {
        "./program",
        "file.txt",
        "INFO"
    };
    ArgsValidator argsValidator(argc, argv);

    ASSERT_TRUE(argsValidator.is_valid());
}

TEST(testArgsValidatorGetArgs)
{
    int argc = 3;
    std::string fileName = "file.txt";
    std::string logLevel = "INFO";
    char const *argv[] = {
        "./program",
        "file.txt",
        "INFO"
    };
    ArgsValidator argsValidator(argc, argv);

    auto args = argsValidator.getArgs();
    ASSERT_EQ(args.first, fileName);
    ASSERT_EQ(args.second, LogLevelFromString(logLevel))
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

    const std::string info = "Inforamation";
    std::string moveInfo = info;
    commands.setInfo(std::move(moveInfo));

    ASSERT_EQ(commands.getInfo(), info);
}

// --- Renders tests ---
// TEST(testRendererDrawMainLayer)
// {

// }

// TEST(testRendererDrawLogTextInput)
// {
    
// }

// TEST(testRendererDrswLogLevels)
// {
    
// }

// TEST(testRendererDrawErrorText)
// {
    
// }

// TEST(testRendererDrawConfrim)
// {
    
// }

// --- Application tests ---
class StreamAppRedirect
{
public:
    StreamAppRedirect()
    {
        cinOld = std::cin.rdbuf(fakeCinBuffer.rdbuf());
        coutOld = std::cout.rdbuf(nullptr);
    }
    void setFakeCinBufferForCreateLog(int mainMenuCommand, std::string &logText, int logLevel, char exitSymbol) 
    { 
        fakeCinBuffer << mainMenuCommand << "\n"; 
        fakeCinBuffer << logText << "\n"; 
        fakeCinBuffer << logLevel << "\n"; 
        fakeCinBuffer << exitSymbol << "\n"; 
    }
    void setFakeCinBufferForChangeLogLevel(int mainMenuCommand, int logLevel, char exitSymbol)
    {
        fakeCinBuffer << mainMenuCommand << "\n";
        fakeCinBuffer << logLevel << "\n"; 
        fakeCinBuffer << exitSymbol << "\n"; 
    }
    void setFakeCinBufferForClearLog(int mainMenuCommand, char acceptSymbol, char exitSymbol)
    {
        fakeCinBuffer << mainMenuCommand << "\n";
        fakeCinBuffer << acceptSymbol << "\n"; 
        fakeCinBuffer << exitSymbol << "\n"; 
    }
    ~StreamAppRedirect()
    {
        std::cin.rdbuf(cinOld);
        std::cout.rdbuf(coutOld);
        std::cin.clear();
        std::cout.clear();
    }

private:
    std::streambuf* cinOld;
    std::streambuf* coutOld;
    std::stringstream fakeCinBuffer;
};

std::string pathToUnitTestApp;

TEST(testApplicationExecCreateNewLog)
{
    std::string fileName = pathToUnitTestApp+"file.txt";

    std::ofstream clearFile(fileName, std::ios::out | std::ios::trunc);
    clearFile.close();

    int mainMenuCommandCreateLog = 1;
    std::string logText = "Text";
    LogLevel logLevel = LogLevel::WARNING;
    char exitSymbol = 'q';

    std::pair loggerParams = {fileName, LogLevel::INFO};
    Application app(loggerParams, exitSymbol);

    StreamAppRedirect streamRedirect;
    streamRedirect.setFakeCinBufferForCreateLog(mainMenuCommandCreateLog, logText, static_cast<int>(logLevel)+1, exitSymbol);

    app.exec();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(30ms);
    
    std::ifstream fin(fileName);
    std::string newLogFile;
    std::getline(fin, newLogFile);
    fin.close();

    std::filesystem::remove(fileName);

    bool isLogFileMatchInputLog = (newLogFile.find(logText) != std::string::npos)
        && (newLogFile.find(LogLevelAsString(logLevel)) != std::string::npos);
    ASSERT_TRUE(isLogFileMatchInputLog);
}

TEST(testApplicationExecChangeDefaultLogLevel)
{
    std::string fileName = pathToUnitTestApp+"file.txt";

    std::ofstream clearFile(fileName, std::ios::out | std::ios::trunc);
    clearFile.close();

    int mainMenuCommandCreateLog = 1;
    int mainMenuCommandChangeLogLevel = 2;
    std::string logText = "Text";
    LogLevel logLevel = LogLevel::WARNING;
    char exitSymbol = 'q';

    std::pair loggerParams = {fileName, LogLevel::INFO};
    Application app(loggerParams, exitSymbol);

    // Changing LogLevel
    StreamAppRedirect streamRedirect;
    streamRedirect.setFakeCinBufferForChangeLogLevel(mainMenuCommandChangeLogLevel, static_cast<int>(LogLevel::ERROR)+1, exitSymbol);

    app.exec();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(30ms);
    // ----------------

    // Try to create log
    streamRedirect.setFakeCinBufferForCreateLog(mainMenuCommandCreateLog, logText, static_cast<int>(logLevel)+1, exitSymbol);

    app.exec();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(30ms);
    // ----------------
    
    std::ifstream fin(fileName);
    std::string newLogFile;
    std::getline(fin, newLogFile);
    fin.close();

    std::filesystem::remove(fileName);

    ASSERT_EQ(newLogFile.size(), 0);
}

TEST(testApplicationExecClearLogFile)
{
    std::string fileName = pathToUnitTestApp+"file.txt";

    std::ofstream clearFile(fileName, std::ios::out | std::ios::trunc);
    clearFile.close();

    int mainMenuCommandCreateLog = 1;
    int mainMenuCommandClearLog = 3;
    std::string logText = "Text";
    LogLevel logLevel = LogLevel::WARNING;
    char acceptSymbol = 'y';
    char exitSymbol = 'q';

    std::pair loggerParams = {fileName, LogLevel::INFO};
    Application app(loggerParams, exitSymbol);

    // Create log
    StreamAppRedirect streamRedirect;
    streamRedirect.setFakeCinBufferForCreateLog(mainMenuCommandCreateLog, logText, static_cast<int>(logLevel)+1, exitSymbol);

    app.exec();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(30ms);
    // ----------------

    // Clear log
    streamRedirect.setFakeCinBufferForClearLog(mainMenuCommandClearLog, acceptSymbol, exitSymbol);

    app.exec();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(30ms);
    // ----------------

    std::ifstream fin(fileName);
    std::string newLogFile;
    std::getline(fin, newLogFile);
    fin.close();

    std::filesystem::remove(fileName);

    ASSERT_EQ(newLogFile.size(), 0);
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
    run_all_tests("Start 'logger' app tests:");
}