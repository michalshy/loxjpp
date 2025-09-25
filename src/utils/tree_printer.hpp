#pragma once

#include "../expr.h"
#include <string>
#include <variant>
#include <iostream>

class TreePrinter : public Visitor<void>
{
    template<typename... Args>
    void paranthesize(const std::string& name, Args*... args)
    {
        std::cout <<  "(" << name;
        for (auto arg : {args...}) {
            std::cout << " ";
            arg->accept(this);
        }
        std::cout << ")";
    }

public:
    void visitLiteralExpr(Literal<void>* expr) override {
        if(std::holds_alternative<std::monostate>(expr->value.literal))
        {
            std::cout << "nil";
        } else if (std::holds_alternative<double>(expr->value.literal)) {
            std::cout << std::to_string(std::get<double>(expr->value.literal));
        } else {
            std::cout << std::get<std::string>(expr->value.literal);
        }
    }
    void visitBinaryExpr(Binary<void>* expr) override {
        paranthesize(
            expr->op.m_Lexeme,
            expr->left.get(),
            expr->right.get()
        );
    }
    void visitGroupingExpr(Grouping<void>* expr) override {
        paranthesize("group", expr->expression.get());
    }
    void visitUnaryExpr(Unary<void>* expr) override {
        paranthesize(expr->op.m_Lexeme, expr->right.get());
    }

    void print(Expr<void>& expr)
    {
        expr.accept(this);
        std::cout<<std::endl;
    }
};