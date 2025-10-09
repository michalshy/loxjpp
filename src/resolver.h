#pragma once

#include "expr.h"
#include "interpreter.h"
#include "stmt.h"
#include <memory>
#include <string>
#include <unordered_map>

enum class FunctionType
{
    NONE,
    FUNCTION
};

class Resolver : public VisitorExpr, public VisitorStmt
{
    Interpreter interpreter;
    std::vector<std::unordered_map<std::string, bool>> scopes;
    FunctionType current = FunctionType::NONE;
public:
    Resolver(Interpreter _interpreter) : interpreter(_interpreter){}

    void visitBlockStmt(Block *stmt) override;
    void visitExpressionStmt(Expression *stmt) override;
    void visitFunctionStmt(Function *stmt) override;
    void visitIfStmt(If *stmt) override;
    void visitPrintStmt(Print *stmt) override;
    void visitReturnStmt(Return *stmt) override;
    void visitVarStmt(Var *stmt) override;
    void visitWhileStmt(While *stmt) override;
    Object visitAssignExpr(Assign *expr) override;
    Object visitBinaryExpr(Binary *expr) override;
    Object visitCallExpr(Call *expr) override;
    Object visitGroupingExpr(Grouping *expr) override;
    Object visitLiteralExpr(Literal *expr) override;
    Object visitLogicalExpr(Logical *expr) override;
    Object visitUnaryExpr(Unary *expr) override;
    Object visitVariableExpr(Variable *expr) override;

    void resolve(std::vector<std::shared_ptr<Stmt>> statements);
private:
    void resolve(std::shared_ptr<Stmt> statement);
    void resolve(std::shared_ptr<Expr> statement);
    void beginScope();
    void endScope();
    void declare(Token name);
    void define(Token name);
    void resolveLocal(Expr* expr, Token name);
    void resolveFunction(Function* function, FunctionType type);
};