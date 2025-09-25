#pragma once
#include "utils/tokens.hpp"
#include <memory>
template<class T>
class Binary;
template<class T>
class Grouping;
template<class T>
class Literal;
template<class T>
class Unary;
template<class T>
class Visitor
{
public:
    virtual T visitBinaryExpr(Binary<T>* expr) = 0;
    virtual T visitGroupingExpr(Grouping<T>* expr) = 0;
    virtual T visitLiteralExpr(Literal<T>* expr) = 0;
    virtual T visitUnaryExpr(Unary<T>* expr) = 0;
};
template<class T>
class Expr{
public:
    virtual ~Expr() = default;
    virtual void accept(Visitor<T>* visitor) = 0;
};
template<class T>
class Binary : public Expr<T>, std::enable_shared_from_this<T> {
public:
    std::shared_ptr<Expr<T>> left;
    Token op;
    std::shared_ptr<Expr<T>> right;
    Binary( std::shared_ptr<Expr<T>> left, Token op, std::shared_ptr<Expr<T>> right) :
       left(left),op(op),right(right){}
    void accept(Visitor<T>* visitor) override {
        return visitor->visitBinaryExpr(this);
    };
};
template<class T>
class Grouping : public Expr<T>, std::enable_shared_from_this<T> {
public:
    std::shared_ptr<Expr<T>> expression;
    Grouping( std::shared_ptr<Expr<T>> expression) :
       expression(expression){}
    void accept(Visitor<T>* visitor) override {
        return visitor->visitGroupingExpr(this);
    };
};
template<class T>
class Literal : public Expr<T>, std::enable_shared_from_this<T> {
public:
    Object value;
    Literal( Object value) :
       value(value){}
    void accept(Visitor<T>* visitor) override {
        return visitor->visitLiteralExpr(this);
    };
};
template<class T>
class Unary : public Expr<T>, std::enable_shared_from_this<T> {
public:
    Token op;
    std::shared_ptr<Expr<T>> right;
    Unary( Token op, std::shared_ptr<Expr<T>> right) :
       op(op),right(right){}
    void accept(Visitor<T>* visitor) override {
        return visitor->visitUnaryExpr(this);
    };
};
