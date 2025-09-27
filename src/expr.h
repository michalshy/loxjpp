#pragma once
#include "utils/tokens.hpp"
#include <memory>
class Assign;
class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;
class VisitorExpr
{
public:
    virtual Object visitAssignExpr(Assign* expr) = 0;
    virtual Object visitBinaryExpr(Binary* expr) = 0;
    virtual Object visitGroupingExpr(Grouping* expr) = 0;
    virtual Object visitLiteralExpr(Literal* expr) = 0;
    virtual Object visitUnaryExpr(Unary* expr) = 0;
    virtual Object visitVariableExpr(Variable* expr) = 0;
};
class Expr{
public:
    virtual ~Expr() = default;
    virtual Object accept(VisitorExpr* visitor) = 0;
};
class Assign : public Expr {
public:
    Token name;
    std::shared_ptr<Expr> value;
    Assign( Token name, std::shared_ptr<Expr> value) :
       name(name),value(value){}
    Object accept(VisitorExpr* visitor) override {
        return visitor->visitAssignExpr(this);
    };
};
class Binary : public Expr {
public:
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;
    Binary( std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) :
       left(left),op(op),right(right){}
    Object accept(VisitorExpr* visitor) override {
        return visitor->visitBinaryExpr(this);
    };
};
class Grouping : public Expr {
public:
    std::shared_ptr<Expr> expression;
    Grouping( std::shared_ptr<Expr> expression) :
       expression(expression){}
    Object accept(VisitorExpr* visitor) override {
        return visitor->visitGroupingExpr(this);
    };
};
class Literal : public Expr {
public:
    Object value;
    Literal( Object value) :
       value(value){}
    Object accept(VisitorExpr* visitor) override {
        return visitor->visitLiteralExpr(this);
    };
};
class Unary : public Expr {
public:
    Token op;
    std::shared_ptr<Expr> right;
    Unary( Token op, std::shared_ptr<Expr> right) :
       op(op),right(right){}
    Object accept(VisitorExpr* visitor) override {
        return visitor->visitUnaryExpr(this);
    };
};
class Variable : public Expr {
public:
    Token name;
    Variable( Token name) :
       name(name){}
    Object accept(VisitorExpr* visitor) override {
        return visitor->visitVariableExpr(this);
    };
};
