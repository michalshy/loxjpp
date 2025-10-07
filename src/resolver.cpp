#include "resolver.h"
#include "expr.h"
#include "stmt.h"
#include "utils/errors.hpp"
#include "utils/object.h"
#include <cerrno>
#include <memory>
#include <unordered_map>

void Resolver::visitBlockStmt(Block *stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
}

void Resolver::resolve(std::vector<std::shared_ptr<Stmt>> statements)
{
    for(const auto& statement : statements)
    {
        resolve(statement);
    }
}


void Resolver::resolve(std::shared_ptr<Stmt> statement)
{
    statement->accept(this);
}

void Resolver::resolve(std::shared_ptr<Expr> statement)
{
    statement->accept(this);
}


void Resolver::beginScope()
{
    scopes.insert(scopes.begin(), std::unordered_map<std::string, bool>());
}

void Resolver::endScope()
{
    scopes.pop_back();
}

void Resolver::visitExpressionStmt(Expression *stmt) {}
void Resolver::visitFunctionStmt(Function *stmt) {}
void Resolver::visitIfStmt(If *stmt) {}
void Resolver::visitPrintStmt(Print *stmt) {}
void Resolver::visitReturnStmt(Return *stmt) {}

void Resolver::visitVarStmt(Var *stmt) {
    declare(stmt->name);
    if(stmt->initializer != nullptr)
    {
        resolve(stmt->initializer);
    }
    define(stmt->name);
}

void Resolver::declare(Token name)
{
    if(scopes.empty()) return;

    scopes.front()[name.m_Lexeme] = false;
}

void Resolver::define(Token name)
{
    if(scopes.empty()) return;

    scopes.front()[name.m_Lexeme] = true;
}

void Resolver::visitWhileStmt(While *stmt) {}

Object Resolver::visitAssignExpr(Assign *expr) { return Object(); }
Object Resolver::visitBinaryExpr(Binary *expr) { return Object(); }
Object Resolver::visitCallExpr(Call *expr) { return Object(); }
Object Resolver::visitGroupingExpr(Grouping *expr) { return Object(); }
Object Resolver::visitLiteralExpr(Literal *expr) { return Object(); }
Object Resolver::visitLogicalExpr(Logical *expr) { return Object(); }
Object Resolver::visitUnaryExpr(Unary *expr) { return Object(); }

Object Resolver::visitVariableExpr(Variable *expr) { 
    if(!scopes.empty() && scopes.front()[expr->name.m_Lexeme] == false)
    {
        error(expr->name, "Can't reead local variable in its own initializer");
    }

    resolveLocal(std::make_shared<Variable>(expr->name), expr->name);

    return Object(); //TODO: find better way to implement visitor
}

void Resolver::resolveLocal(std::shared_ptr<Expr> expr, Token name)
{
    for(int i = scopes.size() - 1;  i >= 0; i--)
    {
        if(scopes[i].find(name.m_Lexeme) != scopes[i].end())
        {
            interpreter.resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}
