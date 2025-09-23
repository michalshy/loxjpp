#pragma once

#include <cstdio>
#include "utils/tokens.hpp"
#include <vector>
#include <string>

class Scanner
{
public:
    Scanner(const std::string&);
    Tokens& scan_tokens();
private:
    bool is_at_end();
    void scan_token();
    char advance();
    void add_token(TokenType type);
    void add_token(TokenType type, Object literal);
    bool match(char c);
    char peek();
    void string();
    bool is_digit(char c);
    void number();
    char peek_next();
    bool is_alpha(char c);
    void identifier();
    bool is_alpha_numeric(char c);

    int m_Start;
    int m_Current;
    int m_Line;

    std::string m_Source;
    Tokens m_Tokens;
};