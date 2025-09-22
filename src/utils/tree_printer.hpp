#pragma once

#include "../expr.h"
#include <string>
#include <variant>

class TreePrinter : public Visitor<std::string>
{
    template<typename... Args>
    std::string paranthesize(const std::string& name, Args*... args)
    {
        std::string result;
        result += "(" + name;
        for (auto arg : {args...}) {
            result += " " + arg->accept(this);
        }
        result += ")";
        return result;
    }

public:
    std::string visitLiteralExpr(Literal<std::string>* expr) override {
        if(std::holds_alternative<std::monostate>(expr->value.literal))
        {
            return "nil";
        } else if (std::holds_alternative<double>(expr->value.literal)) {
            return std::to_string(std::get<double>(expr->value.literal));
        } else {
            return std::get<std::string>(expr->value.literal);
        }
    }
    std::string visitBinaryExpr(Binary<std::string>* expr) override {
        return paranthesize(
            expr->op.m_Lexeme,
            expr->left,
            expr->right
        );
    }
    std::string visitGroupingExpr(Grouping<std::string>* expr) override {
        return paranthesize("group", expr->expression);
    }
    std::string visitUnaryExpr(Unary<std::string>* expr) override {
        return paranthesize(expr->op.m_Lexeme, expr->right);
    }

    std::string print(Expr<std::string>& expr)
    {
        return expr.accept(this);
    }
};