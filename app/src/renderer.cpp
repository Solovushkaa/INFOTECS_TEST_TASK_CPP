#include "renderer.hpp"
#include <iostream>

Renderer::Renderer(Commands *applicationMainMenu)
    : m_applicationMainMenu(applicationMainMenu)
{}

void Renderer::drawMainLayer()
{
    clearLayer();

    drawInfoBlock(m_applicationMainMenu->getInfo());
    drawSubmenus();
    drawContinue();
}

void Renderer::drawLogTextInput()
{
    clearLayer();
    std::cout << "Enter message: ";
}

void Renderer::drawLogLevels(LogLevel byDefault)
{
    clearLayer();
    std::cout << "Select log level:\n";
    std::cout << "1.INFO\n";
    std::cout << "2.WARNING\n";
    std::cout << "3.ERROR\n";
    std::cout << "4.Keep selected (" << LogLevelAsString(byDefault) <<  ")\n";
    drawContinue();
}

void Renderer::drawErrorText(std::string_view text)
{
    clearLayer();
    std::cerr << "Error: " << text << "\n";
    std::cerr << "Enter to continue...\n";
}

void Renderer::drawConfrimClearLogFile()
{
    clearLayer();
    std::cout << "To clear the logging file, enter (y/Y) or enter (n/N) to cancel: ";
}

void Renderer::clearLayer()
{
    std::cout << "\033[2J\033[H"; // ASCII code for cleaning the console
}

void Renderer::drawInfoBlock(std::string_view info)
{
    int length = info.size();
    drawLine(length);
    std::cout << "\n| " << info << " |\n";
    drawLine(length);
    std::cout << '\n';
}

void Renderer::drawLine(int length)
{
    std::cout << '+';
    const int boxLength = length + 2;
    for(int i = 0; i < boxLength; ++i) std::cout << '-';
    std::cout << '+';
}

void Renderer::drawSubmenus()
{
    int index = 1;
    for(auto& [name,_] : *m_applicationMainMenu)
    {
        std::cout << index << '.' << name << '\n';
        ++index;
    }
}

void Renderer::drawContinue()
{
    std::cout << "Enter to continue...\n";
}