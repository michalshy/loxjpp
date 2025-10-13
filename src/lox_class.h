#pragma once

#include <memory>
#include <string>
#include "lox_callable.h"

class LoxClass : public LoxCallable, std::enable_shared_from_this<LoxClass>
{
    std::string name;

public:
    LoxClass(std::string _name) : name(_name){}

    Object call(Interpreter *interpreter, const std::vector<Object> &arguments) override;
    int arity() override;

    std::string to_string() override { return name; }
};