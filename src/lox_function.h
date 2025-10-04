#pragma once

#include "lox_callable.h"
#include "stmt.h"
#include <memory>
class LoxFunction : public LoxCallable
{
    std::shared_ptr<Function> declaration;
public:
    LoxFunction(std::shared_ptr<Function> _declaration) : declaration(_declaration) {} 
    Object call(Interpreter* interpreter, const std::vector<Object>& arguments) override;
    int arity() override;
    std::string to_string() override;
};