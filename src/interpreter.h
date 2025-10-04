#pragma once
#include "environment.h"
#include "expr.h"
#include "stmt.h"
#include "utils/object.h"
#include <memory>

static Environment globals = Environment();

class Interpreter : public VisitorExpr, VisitorStmt
{
    Environment env = globals;
public:
    Interpreter();

    void interpret(std::vector<std::shared_ptr<Stmt>> statements);

    Object visitUnaryExpr(Unary* expr) override;
    Object visitBinaryExpr(Binary* expr) override;
    Object visitCallExpr(Call* expr) override;
    Object visitGroupingExpr(Grouping* expr) override;
    Object visitLiteralExpr(Literal* expr) override;
    Object visitLogicalExpr(Logical *expr) override;
    Object visitVariableExpr(Variable *expr) override;
    Object visitAssignExpr(Assign *expr) override;
    
    void visitBlockStmt(Block *stmt) override;
    void visitPrintStmt(Print* stmt) override;
    void visitIfStmt(If *stmt) override;
    void visitExpressionStmt(Expression* stmt) override;
    void visitFunctionStmt(Function *stmt) override;
    void visitVarStmt(Var *stmt) override;
    void visitWhileStmt(While *stmt) override;
    void executeBlock(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> env);
private:
    void execute(std::shared_ptr<Stmt> statements);
    Object evaluate(std::shared_ptr<Expr> expr);
    bool isTruthy(Object object);
    bool isEqual(Object left, Object right);
    std::string stringify(Object object);
};