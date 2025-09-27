#pragma once

#include "expr.h"
#include "utils/object.h"
#include "utils/tokens.hpp"
#include "utils/errors.hpp"
#include "stmt.h"
#include <memory>
#include <vector>

class ParseError : public std::exception{};

class Parser
{
    Tokens tokens;
    int current = 0;
public:
    Parser(const Tokens& _tokens) {
        tokens = _tokens;
    }
    std::vector<std::shared_ptr<Stmt>> parse()
    {
        std::vector<std::shared_ptr<Stmt>> statements;
        while(!isAtEnd()) {
            statements.push_back(declaration());
        }

        return statements;
    }
private:

    std::shared_ptr<Stmt> declaration()
    {
        try {
         if(match(TokenType::VAR)) return varDeclaration();
         return statement();
        } catch (ParseError error) {
            synchronize();
            return nullptr;
        }
    }

    std::shared_ptr<Stmt> varDeclaration()
    {
        Token name = consume(TokenType::IDENTIFIER, "Expect var name.");

        std::shared_ptr<Expr> initializer = nullptr;
        if(match(TokenType::EQUAL))
        {
            initializer = expression();
        }

        consume(TokenType::SEMICOLON, "Expect ; after var declaration.");
        return std::make_shared<Var>(Var(name, initializer));
    }

    std::shared_ptr<Stmt> statement()
    {
        if(match(TokenType::PRINT)) return printStatement();
        if(match(TokenType::LEFT_BRACE)) return std::make_shared<Block>(block());

        return expressionStatement();
    }

    std::shared_ptr<Stmt> printStatement()
    {
        std::shared_ptr<Expr> val = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after value.");
        return std::make_shared<Print>(Print(val));
    }

    std::shared_ptr<Stmt> expressionStatement()
    {
        std::shared_ptr<Expr> val = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after value.");
        return std::make_shared<Expression>(Expression(val));
    }

    std::vector<std::shared_ptr<Stmt>> block()
    {
        std::vector<std::shared_ptr<Stmt>> statements;

        while(!check(TokenType::RIGHT_BRACE) && !isAtEnd())
        {
            statements.push_back(declaration());
        }

        consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
        return statements;
    }

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

    std::shared_ptr<Expr> expression()
    {
        return assignment();
    }

    std::shared_ptr<Expr> assignment()
    {
        std::shared_ptr<Expr> expr = equality();

        if(match(TokenType::EQUAL))
        {
            Token equals = previous();
            std::shared_ptr<Expr> value = assignment();
            
            Variable* var_expr = dynamic_cast<Variable*>(expr.get());
            if(var_expr)
            {
                Token name = var_expr->name; 
                return std::make_shared<Assign>(name, value);
            }

            error(equals, "Invalid assignment target.");
        }
        return expr;
    }

    std::shared_ptr<Expr> equality()
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

    std::shared_ptr<Expr> comparision()
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

    std::shared_ptr<Expr> term()
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

    std::shared_ptr<Expr> factor()
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

    std::shared_ptr<Expr> unary()
    {
        if(match(TokenType::BANG, TokenType::MINUS))
        {
            Token op = previous();
            std::shared_ptr<Expr> right = unary();
            return std::make_shared<Unary>(op, right);
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
    
    std::shared_ptr<Expr> primary()
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

        if(match(TokenType::IDENTIFIER))
        {
            return std::make_shared<Variable>(previous());
        }

        if (match(TokenType::LEFT_PAREN)) 
        {
            std::shared_ptr<Expr> expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
            return std::make_shared<Grouping>(expr);
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