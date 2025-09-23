#pragma once

#include "expr.h"
#include "utils/tokens.hpp"
#include <memory>
#include <variant>

class Parser
{
    Tokens tokens;
    int current = 0;
public:
    Parser(const Tokens& _tokens) {
        tokens = _tokens;
    }
private:
    template<typename T>
    std::unique_ptr<Expr<T>> expression()
    {
        return equality<T>();
    }

    template<typename T>
    std::unique_ptr<Expr<T>> equality()
    {
        Expr<T> expr = comparision<T>();

        while(match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL))
        {
            Token op = previous();
            Expr<T> right = comparision<T>();
            expr = std::make_unique(Binary<T>(expr, op, right));
        }

        return expr;
    }

    template<typename T>
    std::unique_ptr<Expr<T>> comparision()
    {
        Expr<T> expr = term<T>();

        while(match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
        {
            Token op = previous();
            Expr<T> right = term<T>();
            expr = std::make_unique(Binary<T>(expr, op, right));
        }

        return expr;
    }

    template<typename T>
    std::unique_ptr<Expr<T>> term()
    {
        Expr<T> expr = factor<T>();

        while(match(TokenType::MINUS, TokenType::PLUS))
        {
            Token op = previous();
            Expr<T> right = factor<T>();
            expr = std::make_unique(Binary<T>(expr, op, right));
        }

        return expr;
    }

    template<typename T>
    std::unique_ptr<Expr<T>> factor()
    {
        Expr<T> expr = unary<T>();

        while(match(TokenType::SLASH, TokenType::STAR))
        {
            Token op = previous();
            Expr<T> right = unary<T>();
            expr = std::make_unique(Binary<T>(expr, op, right));
        }

        return expr;
    }

    template<typename T>
    std::unique_ptr<Expr<T>> unary()
    {
        if(match(TokenType::BANG, TokenType::MINUS))
        {
            Token op = previous();
            Expr<T> right = unary<T>();
            return std::make_unique(Unary<T>(op, right));
        }

        return primary();
    }

    template<typename T>
    std::unique_ptr<Expr<T>> primary()
    {
        if(match(TokenType::FALSE)) return Literal<T>(false);
        if(match(TokenType::TRUE)) return Literal<T>(true);
        if(match(TokenType::NIL)) return Literal<T>();

        if(match(TokenType::NUMBER, TokenType::STRING))
        {
            return Literal<T>(previous().m_Literal);
        }

        if (match(TokenType::LEFT_PAREN)) 
        {
            Expr<T> expr = expression<T>();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
            return Grouping<T>(expr);
        }
    }

    template<typename... Args>
    bool match(Args... types)
    {
        for (TokenType type : {types...}) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    bool check(TokenType type) 
    {
        if(isAtEnd()) return false;
        return peek().m_Type == type;
    }

    Token advance()
    {
        if(!isAtEnd()) current++;
        return previous();
    }

    bool isAtEnd()
    {
        return peek().m_Type == TokenType::END_OF_FILE;
    }

    Token peek()
    {
        return tokens[current];
    }

    Token previous()
    {
        return tokens[current - 1];
    }
};