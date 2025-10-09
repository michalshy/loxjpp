#pragma once
#include "environment.h"
#include "expr.h"
#include "stmt.h"
#include "utils/object.h"
#include <memory>
#include <unordered_map>


class Globals
{
    std::shared_ptr<Environment> global_env;
    static Globals* _globals;
    Globals()
    {
        global_env = std::make_shared<Environment>();
    }
public:

    static Globals* GetInstance()
    {
        if(_globals == nullptr)
            _globals = new Globals();
        return _globals;
    }

    std::shared_ptr<Environment> get_env()
    {
        return global_env;
    }
};

class Interpreter : public VisitorExpr, VisitorStmt
{
    std::shared_ptr<Environment> env;
    std::unordered_map<Expr*, int> locals;
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
    void visitReturnStmt(Return *stmt) override;
    void visitVarStmt(Var *stmt) override;
    void visitWhileStmt(While *stmt) override;
    void executeBlock(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> env);
    void resolve(Expr* expr, int depth);
    std::shared_ptr<Environment> get_env() { return env; }
private:
    void execute(std::shared_ptr<Stmt> statements);
    Object evaluate(std::shared_ptr<Expr> expr);
    bool isTruthy(Object object);
    bool isEqual(Object left, Object right);
    std::string stringify(Object object);
    Object lookUpVariable(Token name, Expr* expr);
};