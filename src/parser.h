#pragma once

#include "expr.h"
#include "utils/tokens.hpp"
#include "utils/errors.hpp"

class ParseError : public std::exception{};

template<class T>
class Parser
{
    Tokens tokens;
    int current = 0;
public:
    Parser(const Tokens& _tokens) {
        tokens = _tokens;
    }
    std::shared_ptr<Expr<T>> parse()
    {
        try 
        {
            return expression();
        } 
        catch (ParseError& error)
        {
            return nullptr;
        }
    }
private:

    Token previous()
    {
        return tokens[current - 1];
    }

    Token peek()
    {
        return tokens[current];
    }

    bool isAtEnd()
    {
        return peek().m_Type == TokenType::END_OF_FILE;
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

    std::shared_ptr<Expr<T>> expression()
    {
        return equality();
    }

    std::shared_ptr<Expr<T>> equality()
    {
        std::shared_ptr<Expr<T>> expr = comparision();

        while(match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL))
        {
            Token op = previous();
            std::shared_ptr<Expr<T>> right = comparision();
            expr = std::make_shared<Binary<T>>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<Expr<T>> comparision()
    {
        std::shared_ptr<Expr<T>> expr = term();

        while(match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
        {
            Token op = previous();
            std::shared_ptr<Expr<T>> right = term();
            expr = std::make_shared<Binary<T>>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<Expr<T>> term()
    {
        std::shared_ptr<Expr<T>> expr = factor();

        while(match(TokenType::MINUS, TokenType::PLUS))
        {
            Token op = previous();
            std::shared_ptr<Expr<T>> right = factor();
            expr = std::make_shared<Binary<T>>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<Expr<T>> factor()
    {
        std::shared_ptr<Expr<T>> expr = unary();

        while(match(TokenType::SLASH, TokenType::STAR))
        {
            Token op = previous();
            std::shared_ptr<Expr<T>> right = unary();
            expr = std::make_shared<Binary<T>>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<Expr<T>> unary()
    {
        if(match(TokenType::BANG, TokenType::MINUS))
        {
            Token op = previous();
            std::shared_ptr<Expr<T>> right = unary();
            return std::make_shared<Unary<T>>(op, right);
        }

        return primary();
    }

    Token consume(TokenType type, std::string message)
    {
        if(check(type)) return advance();
        throw parse_error(peek(), message);
    }

    ParseError parse_error(Token token, std::string message)
    {
        error(token, message);
        return ParseError();
    }
    
    std::shared_ptr<Expr<T>> primary()
    {
        if(match(TokenType::FALSE)) return std::make_shared<Literal<T>>(Object(false));
        if(match(TokenType::TRUE)) return std::make_shared<Literal<T>>(Object(true));
        if(match(TokenType::NIL)) return std::make_shared<Literal<T>>(Object());

        if(match(TokenType::NUMBER, TokenType::STRING))
        {
            if (std::holds_alternative<Object>(previous().m_Literal)) {
                return std::make_shared<Literal<T>>(std::get<Object>(previous().m_Literal));
            } else if (std::holds_alternative<std::monostate>(previous().m_Literal)) {
                return std::make_shared<Literal<T>>(Object());
            }
        }

        if (match(TokenType::LEFT_PAREN)) 
        {
            std::shared_ptr<Expr<T>> expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
            return std::make_shared<Grouping<T>>(expr);
        }

        throw parse_error(tokens[current], "Expect expression");
    }
    
    void synchronize()
    {
        advance();

        while(isAtEnd())
        {
            if(previous().m_Type == TokenType::SEMICOLON) return;
            switch (peek().m_Type) {
                case TokenType::CLASS:
                case TokenType::FUN:
                case TokenType::VAR:
                case TokenType::FOR:
                case TokenType::IF:
                case TokenType::WHILE:
                case TokenType::PRINT:
                case TokenType::RETURN:
                    return;
                default:
                    break;
            }
            advance();
        }
    }
};