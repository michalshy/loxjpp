#pragma once

#include <string_view>
#include <iostream>

static void report(int line, std::string_view where, std::string_view message)
{
    std::cout << "[line " << line << "] Error" << where << ": " << message << std::endl;
}

static void error(int line, std::string_view message)
{
    report(line, "", message);
}
