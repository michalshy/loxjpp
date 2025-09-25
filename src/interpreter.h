#pragma once
#include "expr.h"
#include "utils/object.h"
#include <memory>

class Interpreter : public Visitor<Object>
{
public:
    void interpret(std::shared_ptr<Expr<Object>> expr);

    Object visitUnaryExpr(Unary<Object> *expr) override;
    Object visitBinaryExpr(Binary<Object> *expr) override;
    Object visitGroupingExpr(Grouping<Object> *expr) override;
    Object visitLiteralExpr(Literal<Object> *expr) override;
private:
    Object evaluate(std::shared_ptr<Expr<Object>> expr);
    bool isTruthy(Object object);
    bool isEqual(Object left, Object right);
    std::string stringify(Object object);
};