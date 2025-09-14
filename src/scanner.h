#pragma once

#include <list>
#include <string>
#include <ostream>

struct Token
{

    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "temp";
        return os;
    }
};

class Scanner
{
public:
    Scanner(std::string&);
    std::list<Token> scan_tokens();
};