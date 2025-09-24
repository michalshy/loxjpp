#include "parser.h"
#include <memory>
#include "utils/errors.hpp"

std::shared_ptr<Expr> Parser::parse()
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

std::shared_ptr<Expr> Parser::expression()
{
    return equality();
}

std::shared_ptr<Expr> Parser::equality()
{
    std::shared_ptr<Expr> expr = comparision();

    while(match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = comparision();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::comparision()
{
    std::shared_ptr<Expr> expr = term();

    while(match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::term()
{
    std::shared_ptr<Expr> expr = factor();

    while(match(TokenType::MINUS, TokenType::PLUS))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::factor()
{
    std::shared_ptr<Expr> expr = unary();

    while(match(TokenType::SLASH, TokenType::STAR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
    if(match(TokenType::BANG, TokenType::MINUS))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(op, right);
    }

    return primary();
}

std::shared_ptr<Expr> Parser::primary()
{
    if(match(TokenType::FALSE)) return std::make_shared<Literal>(Object(false));
    if(match(TokenType::TRUE)) return std::make_shared<Literal>(Object(true));
    if(match(TokenType::NIL)) return std::make_shared<Literal>(Object());

    if(match(TokenType::NUMBER, TokenType::STRING))
    {
        if (std::holds_alternative<Object>(previous().m_Literal)) {
            return std::make_shared<Literal>(std::get<Object>(previous().m_Literal));
        } else if (std::holds_alternative<std::monostate>(previous().m_Literal)) {
            return std::make_shared<Literal>(Object());
        }
    }

    if (match(TokenType::LEFT_PAREN)) 
    {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return std::make_shared<Grouping>(expr);
    }

    throw parse_error(tokens[current], "Expect expression");
}

Token Parser::consume(TokenType type, std::string message)
{
    if(check(type)) return advance();
    throw parse_error(peek(), message);
}

ParseError Parser::parse_error(Token token, std::string message)
{
    error(token, message);
    return ParseError();
}

void Parser::synchronize()
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

template<typename... Args>
bool Parser::match(Args... types)
{
    for (TokenType type : {types...}) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) 
{
    if(isAtEnd()) return false;
    return peek().m_Type == type;
}

Token Parser::advance()
{
    if(!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd()
{
    return peek().m_Type == TokenType::END_OF_FILE;
}

Token Parser::peek()
{
    return tokens[current];
}

Token Parser::previous()
{
    return tokens[current - 1];
}