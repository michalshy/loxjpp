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

void Resolver::visitExpressionStmt(Expression *stmt) {
    resolve(stmt->expression);
}

void Resolver::visitFunctionStmt(Function *stmt) {
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(std::make_shared<Function>(*stmt), FunctionType::FUNCTION);
}

void Resolver::resolveFunction(std::shared_ptr<Function> function, FunctionType type)
{
    FunctionType enclosingFunction = current;
    current = type;

    beginScope();
    for(const auto& param : function->params)
    {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();

    current = enclosingFunction;
}

void Resolver::visitIfStmt(If *stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if(stmt->elseBranch != nullptr) resolve(stmt->elseBranch);
}
void Resolver::visitPrintStmt(Print *stmt) {
    resolve(stmt->expression);
}
void Resolver::visitReturnStmt(Return *stmt) {
    if (current == FunctionType::NONE) {
      error(stmt->keyword, "Can't return from top-level code.");
    }
    if(stmt->value != nullptr)
    {
        resolve(stmt->value);
    }
}

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

    if(scopes.front().find(name.m_Lexeme) != scopes.front().end())
        error(name, "Already a variable with this name in this scope.");

    scopes.front()[name.m_Lexeme] = false;
}

void Resolver::define(Token name)
{
    if(scopes.empty()) return;

    scopes.front()[name.m_Lexeme] = true;
}

void Resolver::visitWhileStmt(While *stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
}

Object Resolver::visitAssignExpr(Assign *expr) { 
    
    resolve(expr->value);
    resolveLocal(std::make_shared<Assign>(*expr), expr->name);

    return Object(); //TODO: find better way to implement visitor
}
Object Resolver::visitBinaryExpr(Binary *expr) { 
    resolve(expr->left);
    resolve(expr->right);
    return Object(); 
}

Object Resolver::visitCallExpr(Call *expr) { 
    resolve(expr->callee);
    for(const auto& param : expr->arguments)
    {
        resolve(param);
    }
    return Object(); 
}

Object Resolver::visitGroupingExpr(Grouping *expr) { 
    resolve(expr->expression);
    return Object(); 
}

Object Resolver::visitLiteralExpr(Literal *expr) { 
    return Object(); 
}

Object Resolver::visitLogicalExpr(Logical *expr) { 
    resolve(expr->left);
    resolve(expr->right);
    return Object(); 
}

Object Resolver::visitUnaryExpr(Unary *expr) { 
    resolve(expr->right);
    return Object(); 
}

Object Resolver::visitVariableExpr(Variable *expr) { 
    if(!scopes.empty() && scopes.front()[expr->name.m_Lexeme] == false)
    {
        error(expr->name, "Can't read local variable in its own initializer");
    }

    resolveLocal(std::make_shared<Variable>(*expr), expr->name);

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
