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

void Resolver::visitClassStmt(Class* stmt)
{
    ClassType enclosingClass = currentClass;
    currentClass = ClassType::CLASS;

    declare(stmt->name);
    define(stmt->name);

    if(stmt->superclass != nullptr && stmt->name.m_Lexeme == stmt->superclass->name.m_Lexeme)
    {
        error(stmt->superclass->name, "A class can't inherit from itself");
    }

    if(stmt->superclass != nullptr)
    {
        currentClass = ClassType::SUBCLASS;
        resolve(stmt->superclass);
    }

    if(stmt->superclass != nullptr)
    {
        beginScope();
        scopes.back()["super"] = true;
    }

    beginScope();
    scopes.back()["this"] = true;

    for(auto& method: stmt->methods)
    {
        FunctionType declaration = FunctionType::METHOD;
        if(method->name.m_Lexeme == "init")
            declaration = FunctionType::INITIALIZER;
        resolveFunction(method.get(), declaration);
    }

    endScope();
    
    if(stmt->superclass != nullptr) endScope();
    
    currentClass = enclosingClass;
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
    scopes.push_back(std::unordered_map<std::string, bool>());
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

    resolveFunction(stmt, FunctionType::FUNCTION);
}

void Resolver::resolveFunction(Function* function, FunctionType type)
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
        if(current == FunctionType::INITIALIZER)
        {
            error(stmt->keyword, "Can't return value from initializer.");
        }

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

    if(scopes.back().find(name.m_Lexeme) != scopes.back().end())
        error(name, "Already a variable with this name in this scope.");

    scopes.back()[name.m_Lexeme] = false;
}

void Resolver::define(Token name)
{
    if(scopes.empty()) return;

    scopes.back()[name.m_Lexeme] = true;
}

void Resolver::visitWhileStmt(While *stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
}

Object Resolver::visitAssignExpr(Assign *expr) { 
    
    resolve(expr->value);
    resolveLocal(expr, expr->name);

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

Object Resolver::visitGetExpr(Get *expr)
{
    resolve(expr->object);
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

Object Resolver::visitSetExpr(Set *expr)
{
    resolve(expr->value);
    resolve(expr->object);
    return Object();
}

Object Resolver::visitSuperExpr(Super *expr)
{
    if (currentClass == ClassType::NONE) {
      error(expr->keyword,
          "Can't use 'super' outside of a class.");
    } else if (currentClass != ClassType::SUBCLASS) {
      error(expr->keyword,
          "Can't use 'super' in a class with no superclass.");
    }

    resolveLocal(expr, expr->keyword);
    return Object();
}

Object Resolver::visitThisExpr(This* expr)
{
    if(currentClass == ClassType::NONE)
    {
        error(expr->keyword, "Can't use this outside of class.");
    }

    resolveLocal(expr, expr->keyword);
    return Object();
}

Object Resolver::visitUnaryExpr(Unary *expr) { 
    resolve(expr->right);
    return Object(); 
}

Object Resolver::visitVariableExpr(Variable *expr) { 
    if (!scopes.empty() && scopes.back().count(expr->name.m_Lexeme) &&
        !scopes.back()[expr->name.m_Lexeme]) {
        error(expr->name, "Can't read local variable in its own initializer");
    }

    resolveLocal(expr, expr->name);

    return Object(); //TODO: find better way to implement visitor
}

void Resolver::resolveLocal(Expr* expr, Token name)
{
    for(int i = scopes.size() - 1; i >= 0; i--)
    {
        if(scopes[i].find(name.m_Lexeme) != scopes[i].end())
        {
            interpreter.resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

