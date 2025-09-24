#pragma once
#include "utils/tokens.hpp"
#include <memory>
class Binary;
class Grouping;
class Literal;
class Unary;
class Visitor
{
public:
    virtual void visitBinaryExpr(Binary* expr) = 0;
    virtual void visitGroupingExpr(Grouping* expr) = 0;
    virtual void visitLiteralExpr(Literal* expr) = 0;
    virtual void visitUnaryExpr(Unary* expr) = 0;
};
class Expr{
public:
    virtual ~Expr() = default;
    virtual void accept(Visitor* visitor) = 0;
};
class Binary : public Expr {
public:
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;
    Binary( std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) :
       left(left),op(op),right(right){}
    void accept(Visitor* visitor) override {
        return visitor->visitBinaryExpr(this);
    };
};
class Grouping : public Expr {
public:
    std::shared_ptr<Expr> expression;
    Grouping( std::shared_ptr<Expr> expression) :
       expression(expression){}
    void accept(Visitor* visitor) override {
        return visitor->visitGroupingExpr(this);
    };
};
class Literal : public Expr {
public:
    Object value;
    Literal( Object value) :
       value(value){}
    void accept(Visitor* visitor) override {
        return visitor->visitLiteralExpr(this);
    };
};
class Unary : public Expr {
public:
    Token op;
    std::shared_ptr<Expr> right;
    Unary( Token op, std::shared_ptr<Expr> right) :
       op(op),right(right){}
    void accept(Visitor* visitor) override {
        return visitor->visitUnaryExpr(this);
    };
};
