#pragma once

#include "environment.h"
#include "lox_callable.h"
#include "stmt.h"
#include <memory>
class LoxFunction : public LoxCallable
{
    std::shared_ptr<Function> declaration;
    std::shared_ptr<Environment> closure;
public:
    LoxFunction(std::shared_ptr<Function> _declaration, std::shared_ptr<Environment> _closure) : declaration(_declaration), closure(_closure) {} 
    Object call(Interpreter* interpreter, const std::vector<Object>& arguments) override;
    int arity() override;
    std::string to_string() override;
};