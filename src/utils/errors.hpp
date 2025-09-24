#pragma once

#include <string_view>
#include <iostream>
#include "tokens.hpp"
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

static void error(Token token, std::string message)
{
    if(token.m_Type == TokenType::END_OF_FILE) 
    {
        report(token.m_Line, " at end", message);
    }
    else 
    {
        report(token.m_Line, " at '" + token.m_Lexeme + "'", message);
    }
}



