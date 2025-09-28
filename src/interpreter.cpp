#include "interpreter.h"
#include "environment.h"
#include "expr.h"
#include "stmt.h"
#include "utils/object.h"
#include "utils/tokens.hpp"
#include <exception>
#include <memory>
#include <string>
#include <sys/types.h>
#include <variant>
#include <iostream>
#include <vector>
#include "utils/errors.hpp"


void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> statements)
{
    try {
        for (const auto& statement : statements)
        {
            execute(statement);
        }
    } catch (RuntimeError error) {
        runtimeError(error);
    }
}

Object Interpreter::visitUnaryExpr(Unary *expr) 
{
    Object right = evaluate(expr->right);

    switch (expr->op.m_Type) {
        case TokenType::BANG:
            return !isTruthy(right);
        case TokenType::MINUS:
            if(std::holds_alternative<double>(right.literal))
                return -1 * std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operand must be a number");
        default:
            return Object();
    }
}
Object Interpreter::visitBinaryExpr(Binary *expr) 
{
    Object left = evaluate(expr->left);
    Object right = evaluate(expr->right);

    switch (expr->op.m_Type) {
        case TokenType::MINUS:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) - std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::SLASH:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) / std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::STAR:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) * std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::PLUS:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) + std::get<double>(right.literal);
            if(std::holds_alternative<std::string>(right.literal) && std::holds_alternative<std::string>(left.literal) ) 
                return std::get<std::string>(left.literal) + std::get<std::string>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a numbers or strings");
        case TokenType::GREATER:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) > std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::GREATER_EQUAL:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) >= std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::LESS:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) < std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::LESS_EQUAL:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) <= std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::BANG_EQUAL: return !isEqual(left, right);
        case TokenType::EQUAL_EQUAL: return isEqual(left, right);
        default:
            return Object();
    }
    return Object();
}
Object Interpreter::visitGroupingExpr(Grouping *expr) 
{
    return evaluate(expr->expression);
}
Object Interpreter::visitLiteralExpr(Literal *expr) 
{
    return expr->value;
}

Object Interpreter::visitLogicalExpr(Logical *expr)
{
    Object left = evaluate(expr->left);

    if(expr->op.m_Type == TokenType::OR)
    {
        if(isTruthy(left)) return left;
    } else {
        if(!isTruthy(left)) return left;
    }

    return evaluate(expr->right);
}

Object Interpreter::visitVariableExpr(Variable* expr)
{
    return env.get(expr->name);
}

Object Interpreter::visitAssignExpr(Assign *expr)
{
    Object value = evaluate(expr->value);
    env.assign(expr->name, value);
    return value;
}

void Interpreter::visitBlockStmt(Block* stmt)
{
    executeBlock(stmt->statements, std::make_shared<Environment>(env));
}

void Interpreter::visitPrintStmt(Print* stmt)
{
    Object value = evaluate(stmt->expression);
    std::cout << stringify(value) << std::endl;
}

void Interpreter::visitIfStmt(If* stmt)
{
    if(isTruthy(evaluate(stmt->condition)))
    {
        execute(stmt->thenBranch);
    } 
    else if (stmt->elseBranch != nullptr)
    {
        execute(stmt->elseBranch);
    }
}

void Interpreter::visitExpressionStmt(Expression* stmt)
{
    evaluate(stmt->expression);
}

void Interpreter::visitVarStmt(Var *stmt)
{
    Object value = Object();
    if(stmt->initializer != nullptr)
    {
        value = evaluate(stmt->initializer);
    }

    env.define(stmt->name.m_Lexeme, value);
}

void Interpreter::visitWhileStmt(While *stmt)
{
    
}


void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> env)
{
    std::shared_ptr<Environment> previous = std::make_shared<Environment>(this->env);
    this->env = env;

    for(const auto& statement : statements)
    {
        execute(statement);
    }
    this->env = previous;
}

void Interpreter::execute(std::shared_ptr<Stmt> statement)
{
    statement->accept(this);
}

Object Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    return expr->accept(this);
}

bool Interpreter::isTruthy(Object object)
{
    if(std::holds_alternative<std::monostate>(object.literal))
        return false;
    if(std::holds_alternative<bool>(object.literal))
    {
        return std::get<bool>(object.literal);
    }
    return true;
}

bool Interpreter::isEqual(Object left, Object right)
{
    if(std::holds_alternative<std::monostate>(left.literal) && std::holds_alternative<std::monostate>(right.literal)) return true;
    if(std::holds_alternative<std::monostate>(left.literal)) return false;

    return left.literal == right.literal;
}

std::string Interpreter::stringify(Object object)
{
    if (std::holds_alternative<std::monostate>(object.literal))
    {
        return "nil";
    }

    if (std::holds_alternative<double>(object.literal))
    {
        std::string text = std::to_string(std::get<double>(object.literal));
        text.erase ( text.find_last_not_of('0') + 1, std::string::npos );
        text.erase ( text.find_last_not_of('.') + 1, std::string::npos );
        return text;
    }

    if (std::holds_alternative<bool>(object.literal))
    {
        return std::get<bool>(object.literal) ? "true" : "false";
    }

    return std::get<std::string>(object.literal);
}