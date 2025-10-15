#include "interpreter.h"
#include "environment.h"
#include "expr.h"
#include "lox_function.h"
#include "stmt.h"
#include "utils/object.h"
#include "utils/tokens.hpp"
#include <chrono>
#include <memory>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <variant>
#include <iostream>
#include <vector>
#include "utils/errors.hpp"
#include "lox_callable.h"
#include <functional>
#include "lox_class.h"
#include "lox_instance.h"
Globals* Globals::_globals = nullptr;

class NativeCallable : public LoxCallable {
    std::function<Object(Interpreter*, const std::vector<Object>&)> function;
    int arity_val;
public:
    NativeCallable(std::function<Object(Interpreter*, const std::vector<Object>&)> func, int arity)
        : function(func), arity_val(arity) {}
    Object call(Interpreter* interpreter, const std::vector<Object>& arguments) override
    {
        return function(interpreter, arguments);
    }

    int arity() override
    {
        return arity_val;
    }

    std::string to_string() override { return "<native fn>"; }
};

Interpreter::Interpreter()
{
    env = Globals::GetInstance()->get_env();

    auto clock_lambda = [](Interpreter* interpreter, const std::vector<Object>& arguments) -> Object {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0;
        return Object(seconds);
    };
    Globals::GetInstance()->get_env()->define("clock", Object(std::make_shared<NativeCallable>(clock_lambda, 0)));
}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> statements)
{
    try {
        for (const auto& statement : statements)
        {
            execute(statement);
        }
    } catch (RuntimeError error) {
        runtimeError(error);
    }
}

Object Interpreter::visitUnaryExpr(Unary *expr) 
{
    Object right = evaluate(expr->right);

    switch (expr->op.m_Type) {
        case TokenType::BANG:
            return !isTruthy(right);
        case TokenType::MINUS:
            if(std::holds_alternative<double>(right.literal))
                return -1 * std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operand must be a number");
        default:
            return Object();
    }
}
Object Interpreter::visitBinaryExpr(Binary *expr) 
{
    Object left = evaluate(expr->left);
    Object right = evaluate(expr->right);

    switch (expr->op.m_Type) {
        case TokenType::MINUS:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) - std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::SLASH:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) / std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::STAR:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) * std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::PLUS:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
            {
                return std::get<double>(left.literal) + std::get<double>(right.literal);
            }
            if(std::holds_alternative<std::string>(right.literal) && std::holds_alternative<std::string>(left.literal) ) 
                return std::get<std::string>(left.literal) + std::get<std::string>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a numbers or strings");
        case TokenType::GREATER:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) > std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::GREATER_EQUAL:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) >= std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::LESS:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) < std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::LESS_EQUAL:
            if(std::holds_alternative<double>(right.literal) && std::holds_alternative<double>(left.literal))
                return std::get<double>(left.literal) <= std::get<double>(right.literal);
            else
                throw RuntimeError(expr->op, "Operands must be a number");
        case TokenType::BANG_EQUAL: return !isEqual(left, right);
        case TokenType::EQUAL_EQUAL: return isEqual(left, right);
        default:
            return Object();
    }
    return Object();
}

Object Interpreter::visitCallExpr(Call* expr)
{
    Object callee = evaluate(expr->callee);

    std::vector<Object> arguments;
    for (const auto& argument : expr->arguments)
    {
        arguments.push_back(evaluate(argument));
    }

    try {
        std::shared_ptr<LoxCallable> function = std::get<std::shared_ptr<LoxCallable>>(callee.literal);

        if(arguments.size() != function->arity())
        {
            throw RuntimeError(expr->paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()) + ".");
        }

        return function->call(this, arguments);
    } catch (const std::bad_variant_access&) {
        throw RuntimeError(expr->paren, "Can only call functions and classes.");
    }
}

Object Interpreter::visitGetExpr(Get *expr)
{
    Object object = evaluate(expr->object);
    try {
        std::shared_ptr<LoxInstance> instance = std::get<std::shared_ptr<LoxInstance>>(object.literal);
        return instance->get(expr->name);
    } catch (const std::bad_variant_access&) {
        throw RuntimeError(expr->name, "Only instances can have properties.");
    }
}

Object Interpreter::visitGroupingExpr(Grouping *expr) 
{
    return evaluate(expr->expression);
}
Object Interpreter::visitLiteralExpr(Literal *expr) 
{
    return expr->value;
}

Object Interpreter::visitLogicalExpr(Logical *expr)
{
    Object left = evaluate(expr->left);

    if(expr->op.m_Type == TokenType::OR)
    {
        if(isTruthy(left)) return left;
    } else {
        if(!isTruthy(left)) return left;
    }

    return evaluate(expr->right);
}

Object Interpreter::visitSetExpr(Set *expr)
{
    Object object = evaluate(expr->object);
    try {
        std::shared_ptr<LoxInstance> instance = std::get<std::shared_ptr<LoxInstance>>(object.literal);

        Object value = evaluate(expr->value);
        instance->set(expr->name, value);
    } catch (const std::bad_variant_access&) {
        throw RuntimeError(expr->name, "Only instances can have properties.");
    }
    return Object();
}


Object Interpreter::visitVariableExpr(Variable* expr)
{
    return lookUpVariable(expr->name, expr);
}

Object Interpreter::visitAssignExpr(Assign *expr)
{
    Object value = evaluate(expr->value);

    if(locals.find(expr) != locals.end())
    {
        int distance = locals[expr];
        env->assignAt(distance, expr->name, value);
    }
    else {
        Globals::GetInstance()->get_env()->assign(expr->name, value);
    }
    return value;
}

void Interpreter::visitBlockStmt(Block* stmt)
{
    executeBlock(stmt->statements, std::make_shared<Environment>(env));
}

void Interpreter::visitClassStmt(Class* stmt)
{
    env->define(stmt->name.m_Lexeme, Object());

    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
    for(auto& method : stmt->methods)
    {
        std::shared_ptr<LoxFunction> function = std::make_shared<LoxFunction>(method, env);
        methods[method->name.m_Lexeme] = function;
    } 

    std::shared_ptr<LoxClass> klass = std::make_shared<LoxClass>(stmt->name.m_Lexeme, methods);
    env->assign(stmt->name, Object(klass));
}

void Interpreter::visitPrintStmt(Print* stmt)
{
    Object value = evaluate(stmt->expression);
    std::cout << stringify(value) << std::endl;
}

void Interpreter::visitIfStmt(If* stmt)
{
    if(isTruthy(evaluate(stmt->condition)))
    {
        execute(stmt->thenBranch);
    } 
    else if (stmt->elseBranch != nullptr)
    {
        execute(stmt->elseBranch);
    }
}

void Interpreter::visitExpressionStmt(Expression* stmt)
{
    evaluate(stmt->expression);
}

void Interpreter::visitFunctionStmt(Function* stmt)
{
    LoxFunction function = LoxFunction(std::make_shared<Function>(*stmt), env);
    env->define(stmt->name.m_Lexeme, Object(std::make_shared<LoxFunction>(function)));
}

void Interpreter::visitReturnStmt(Return* stmt)
{
    Object value = Object();
    
    if(stmt->value != nullptr)
    {
        value = evaluate(stmt->value);
    } 
    throw ReturnVal(value);
}

void Interpreter::visitVarStmt(Var *stmt)
{
    Object value = Object();
    if(stmt->initializer != nullptr)
    {
        value = evaluate(stmt->initializer);
    }

    env->define(stmt->name.m_Lexeme, value);
}

void Interpreter::visitWhileStmt(While *stmt)
{
    while(isTruthy(evaluate(stmt->condition)))
    {
        execute(stmt->body);
    }
}

void Interpreter::executeBlock(std::vector<std::shared_ptr<Stmt>> statements, std::shared_ptr<Environment> environment)
{
    std::shared_ptr<Environment> previous = this->env;
    try
    {
        this->env = environment;
    
        for(const auto& statement : statements)
        {
            execute(statement);
        }
    }
    catch(...)
    {
        this->env = previous;
        throw;
    }
    this->env = previous;
}

void Interpreter::resolve(Expr* expr, int depth)
{
    locals[expr] = depth;
}

void Interpreter::execute(std::shared_ptr<Stmt> statement)
{
    statement->accept(this);
}

Object Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    return expr->accept(this);
}

bool Interpreter::isTruthy(Object object)
{
    if(std::holds_alternative<std::monostate>(object.literal))
        return false;
    if(std::holds_alternative<bool>(object.literal))
    {
        return std::get<bool>(object.literal);
    }
    return true;
}

bool Interpreter::isEqual(Object left, Object right)
{
    if(std::holds_alternative<std::monostate>(left.literal) && std::holds_alternative<std::monostate>(right.literal)) return true;
    if(std::holds_alternative<std::monostate>(left.literal)) return false;

    return left.literal == right.literal;
}

std::string Interpreter::stringify(Object object)
{
    if (std::holds_alternative<std::monostate>(object.literal))
    {
        return "nil";
    }

    if (std::holds_alternative<double>(object.literal))
    {
        std::string text = std::to_string(std::get<double>(object.literal));
        text.erase ( text.find_last_not_of('0') + 1, std::string::npos );
        text.erase ( text.find_last_not_of('.') + 1, std::string::npos );
        return text;
    }

    if (std::holds_alternative<bool>(object.literal))
    {
        return std::get<bool>(object.literal) ? "true" : "false";
    }

    if(std::holds_alternative<std::shared_ptr<LoxInstance>>(object.literal))
    {
        return std::get<std::shared_ptr<LoxInstance>>(object.literal)->to_string();
    }

    if(std::holds_alternative<std::shared_ptr<LoxCallable>>(object.literal))
    {
        return std::get<std::shared_ptr<LoxCallable>>(object.literal)->to_string();
    }

    return std::get<std::string>(object.literal);
}

Object Interpreter::lookUpVariable(Token name, Expr* expr)
{
    if(locals.find(expr) != locals.end())
    {
        int distance = locals[expr];
        return env->getAt(distance, name.m_Lexeme);
    }

    return Globals::GetInstance()->get_env()->get(name);
}



