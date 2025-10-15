#pragma once

#include "environment.h"
#include "lox_callable.h"
#include "stmt.h"
#include <memory>
class LoxInstance;
class LoxFunction : public LoxCallable
{
    std::shared_ptr<Function> declaration;
    std::shared_ptr<Environment> closure;
    bool isInitializer;
public:
    LoxFunction(std::shared_ptr<Function> _declaration, std::shared_ptr<Environment> _closure, bool initializer) : declaration(std::move(_declaration)), closure(std::move(_closure)), isInitializer(initializer) {} 
    Object call(Interpreter* interpreter, const std::vector<Object>& arguments) override;
    int arity() override;
    std::string to_string() override;
    std::shared_ptr<LoxFunction> bind(LoxInstance* instance);
};