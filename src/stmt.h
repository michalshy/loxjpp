#pragma once
#include "utils/tokens.hpp"
#include <memory>
#include "expr.h"
class Block;
class Class;
class Expression;
class Function;
class If;
class Print;
class Return;
class Var;
class While;
class VisitorStmt
{
public:
    virtual void visitBlockStmt(Block* stmt) = 0;
    virtual void visitClassStmt(Class* stmt) = 0;
    virtual void visitExpressionStmt(Expression* stmt) = 0;
    virtual void visitFunctionStmt(Function* stmt) = 0;
    virtual void visitIfStmt(If* stmt) = 0;
    virtual void visitPrintStmt(Print* stmt) = 0;
    virtual void visitReturnStmt(Return* stmt) = 0;
    virtual void visitVarStmt(Var* stmt) = 0;
    virtual void visitWhileStmt(While* stmt) = 0;
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
class Class : public Stmt {
public:
    Token name;
    std::vector<std::shared_ptr<Function>> methods;
    Class( Token name, std::vector<std::shared_ptr<Function>> methods) :
       name(name),methods(methods){}
    void accept(VisitorStmt* visitor) override {
        return visitor->visitClassStmt(this);
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
class Function : public Stmt {
public:
    Token name;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;
    Function( Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body) :
       name(name),params(params),body(body){}
    void accept(VisitorStmt* visitor) override {
        return visitor->visitFunctionStmt(this);
    };
};
class If : public Stmt {
public:
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;
    If( std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch) :
       condition(condition),thenBranch(thenBranch),elseBranch(elseBranch){}
    void accept(VisitorStmt* visitor) override {
        return visitor->visitIfStmt(this);
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
class Return : public Stmt {
public:
    Token keyword;
    std::shared_ptr<Expr> value;
    Return( Token keyword, std::shared_ptr<Expr> value) :
       keyword(keyword),value(value){}
    void accept(VisitorStmt* visitor) override {
        return visitor->visitReturnStmt(this);
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
class While : public Stmt {
public:
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;
    While( std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body) :
       condition(condition),body(body){}
    void accept(VisitorStmt* visitor) override {
        return visitor->visitWhileStmt(this);
    };
};
