#include "interpreter.h"
#include "expr.h"
#include "utils/object.h"
#include "utils/tokens.hpp"
#include <exception>
#include <memory>
#include <string>
#include <variant>
#include <iostream>
#include "utils/errors.hpp"


void Interpreter::interpret(std::shared_ptr<Expr<Object>> expr)
{
    try {
        Object value = evaluate(expr);
        std::cout << stringify(value) << std::endl;
    } catch (RuntimeError error) {
        runtimeError(error);
    }
}

Object Interpreter::visitUnaryExpr(Unary<Object> *expr) 
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
Object Interpreter::visitBinaryExpr(Binary<Object> *expr) 
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
Object Interpreter::visitGroupingExpr(Grouping<Object> *expr) 
{
    return evaluate(expr->expression);
}
Object Interpreter::visitLiteralExpr(Literal<Object> *expr) 
{
    return expr->value;
}

Object Interpreter::evaluate(std::shared_ptr<Expr<Object>> expr)
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