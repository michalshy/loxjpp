#pragma once
#include "utils/tokens.hpp"
#include "expr.h"
#include <memory>
class Block;
class Expression;
class Print;
class Var;
class VisitorStmt
{
public:
    virtual void visitBlockStmt(Block* stmt) = 0;
    virtual void visitExpressionStmt(Expression* stmt) = 0;
    virtual void visitPrintStmt(Print* stmt) = 0;
    virtual void visitVarStmt(Var* stmt) = 0;
};
class Stmt{
public:
    virtual ~Stmt() = default;
    virtual void accept(VisitorStmt* visitor) = 0;
};
class Block : public Stmt {
public:
    std::vector<std::shared_ptr<Stmt>> statements;
    Block( std::vector<std::shared_ptr<Stmt>> statements) :
       statements(statements){}
    void accept(VisitorStmt* visitor) override {
        return visitor->visitBlockStmt(this);
    };
};
class Expression : public Stmt {
public:
    std::shared_ptr<Expr> expression;
    Expression( std::shared_ptr<Expr> expression) :
       expression(expression){}
    void accept(VisitorStmt* visitor) override {
        return visitor->visitExpressionStmt(this);
    };
};
class Print : public Stmt {
public:
    std::shared_ptr<Expr> expression;
    Print( std::shared_ptr<Expr> expression) :
       expression(expression){}
    void accept(VisitorStmt* visitor) override {
        return visitor->visitPrintStmt(this);
    };
};
class Var : public Stmt {
public:
    Token name;
    std::shared_ptr<Expr> initializer;
    Var( Token name, std::shared_ptr<Expr> initializer) :
       name(name),initializer(initializer){}
    void accept(VisitorStmt* visitor) override {
        return visitor->visitVarStmt(this);
    };
};
