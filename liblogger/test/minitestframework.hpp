#include <functional>
#include <vector>
#include <iostream>

struct TestResult 
{
    std::string name;
    std::string message;
    bool passed;
};

std::vector<std::pair<std::string, std::function<void()>>> tests; // Vector of tests
std::vector<TestResult> results;

#define TEST(name) \
    void name(); \
    struct Register##name { \
        Register##name() { tests.push_back({#name, name}); } \
    } register_##name; \
    void name()

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition); \
    }

#define ASSERT_EQ(a, b) \
    if ((a) != (b)) { \
        throw std::runtime_error("Assertion failed: " #a " != " #b); \
    }

void setGreenText()
{
    std::cout << "\033[32m";
}

void setRedText() 
{
    std::cout << "\033[31m";
}

void setDefaultText() 
{
    std::cout << "\033[0m";
}

void run_all_tests(const std::string &header) 
{
    int passed = 0;
    int failed = 0;

    std::cout << header << std::endl;
    std::cout << "========================\n\n";

    for (const auto& [name, func] : tests) {
        try {
            func();
            setGreenText();
            std::cout << "[PASS] ";
            setDefaultText();
            std::cout << name << std::endl;
            ++passed;
        } catch (const std::exception& e) {
            setRedText();
            std::cout << "[FAIL] ";
            setDefaultText();
            std::cout << name << " -> " << e.what() << std::endl;
            ++failed;
        } catch (...) {
            setRedText();
            std::cout << "[FAIL] ";
            setDefaultText();
            std::cout << name << " -> Unknown error" << std::endl;
            ++failed;
        }
    }

    std::cout << "\n========================\n";
    std::cout << "[ALL]: " << (passed + failed) << " tests\n";
    setGreenText();
    std::cout << "[PASS]: " << passed << "\n";
    setRedText();
    std::cout << "[FAIL]: " << failed << std::endl;
    setDefaultText();
}