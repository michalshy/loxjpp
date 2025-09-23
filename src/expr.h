#pragma once
#include "utils/tokens.hpp"
#include <memory>
template<class T>
class Visitor;
template<class T>
class Expr{
public:
    virtual ~Expr() = default;
    virtual T accept(Visitor<T>* visitor) = 0;
};
template<class T>
class Binary : public Expr<T> {
public:
    std::unique_ptr<Expr<T>> left;
    Token op;
    std::unique_ptr<Expr<T>> right;
    Binary( std::unique_ptr<Expr<T>> left, Token op, std::unique_ptr<Expr<T>> right) :
       left(left),op(op),right(right){}
    T accept(Visitor<T>* visitor) override {
        return visitor->visitBinaryExpr(this);
    };
};
template<class T>
class Grouping : public Expr<T> {
public:
    std::unique_ptr<Expr<T>> expression;
    Grouping( std::unique_ptr<Expr<T>> expression) :
       expression(expression){}
    T accept(Visitor<T>* visitor) override {
        return visitor->visitGroupingExpr(this);
    };
};
template<class T>
class Literal : public Expr<T> {
public:
    Object value;
    Literal( Object value) :
       value(value){}
    T accept(Visitor<T>* visitor) override {
        return visitor->visitLiteralExpr(this);
    };
};
template<class T>
class Unary : public Expr<T> {
public:
    Token op;
    std::unique_ptr<Expr<T>> right;
    Unary( Token op, std::unique_ptr<Expr<T>> right) :
       op(op),right(right){}
    T accept(Visitor<T>* visitor) override {
        return visitor->visitUnaryExpr(this);
    };
};
template<class T>
class Visitor
{
public:
    virtual T visitBinaryExpr(Binary<T>* expr) = 0;
    virtual T visitGroupingExpr(Grouping<T>* expr) = 0;
    virtual T visitLiteralExpr(Literal<T>* expr) = 0;
    virtual T visitUnaryExpr(Unary<T>* expr) = 0;
};
