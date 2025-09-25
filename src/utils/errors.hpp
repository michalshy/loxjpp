#pragma once

#include <string_view>
#include <iostream>
#include "tokens.hpp"
#include "../lox.h"

class RuntimeError : std::exception
{
    std::string full_message;
public:
    Token token;
    RuntimeError(Token _token, std::string _message)
    {
        token = _token;
        full_message = "Runtime error: " + _message + " at '" + _token.m_Lexeme + "' on line " + std::to_string(_token.m_Line);
    }
    const char* what()
    {
        return full_message.c_str();
    }
};


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

static void runtimeError(RuntimeError error)
{
    std::cout << error.what() << "\n[line " << error.token.m_Line << "]";
    Lox::s_HadRuntimeError = true;
}


