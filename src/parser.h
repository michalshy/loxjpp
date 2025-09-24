#pragma once

#include "expr.h"
#include "utils/tokens.hpp"

class ParseError : public std::exception{};

class Parser
{
    Tokens tokens;
    int current = 0;
public:
    Parser(const Tokens& _tokens) {
        tokens = _tokens;
    }

    std::shared_ptr<Expr> parse();
private:
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparision();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();
    Token consume(TokenType type, std::string message);
    ParseError parse_error(Token token, std::string message);
    void synchronize();
    template<typename... Args>
    bool match(Args... types);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
};