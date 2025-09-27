#pragma once
#include "environment.h"
#include "expr.h"
#include "stmt.h"
#include "utils/object.h"
#include <memory>

class Interpreter : public VisitorExpr, VisitorStmt
{
    Environment env = Environment();
public:
    void interpret(std::vector<std::shared_ptr<Stmt>> statements);

    Object visitUnaryExpr(Unary* expr) override;
    Object visitBinaryExpr(Binary* expr) override;
    Object visitGroupingExpr(Grouping* expr) override;
    Object visitLiteralExpr(Literal* expr) override;
    Object visitVariableExpr(Variable *expr) override;
    Object visitAssignExpr(Assign *expr) override;
    
    void visitBlockStmt(Block *stmt) override;
    void visitPrintStmt(Print* stmt) override;
    void visitExpressionStmt(Expression* stmt) override;
    void visitVarStmt(Var *stmt) override;
private:
    void executeBlock(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> env);
    void execute(std::shared_ptr<Stmt> statements);
    Object evaluate(std::shared_ptr<Expr> expr);
    bool isTruthy(Object object);
    bool isEqual(Object left, Object right);
    std::string stringify(Object object);
};