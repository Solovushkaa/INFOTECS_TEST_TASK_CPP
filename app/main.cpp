#include "application.hpp"
#include "argsvalidator.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    ArgsValidator validator(argc, argv);
    if(!validator.is_valid()){
        std::cout << "Invalid arguments!\n";
        return 1;
    }
    auto loggerParams = validator.getArgs();

    try {
        const char exitSymbol = 'q';
        Application app(loggerParams, exitSymbol);
        return app.exec();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
