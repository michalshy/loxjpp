#pragma once

#include <string_view>
#include <iostream>
#include "../lox.h"

static void report(int line, std::string_view where, std::string_view message)
{
    std::cout << "[line " << line << "] Error" << where << ": " << message << std::endl;
    Lox::s_HadError = true;
}

static void error(int line, std::string_view message)
{
    report(line, "", message);
}
