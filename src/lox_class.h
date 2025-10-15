#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "lox_callable.h"
#include "lox_function.h"

class LoxClass : public LoxCallable, std::enable_shared_from_this<LoxClass>
{
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
public:
    LoxClass(std::string _name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> _methods) : name(_name), methods(_methods) {}

    Object call(Interpreter *interpreter, const std::vector<Object> &arguments) override;
    int arity() override;

    std::shared_ptr<LoxFunction> findMethod(std::string name);

    std::string to_string() override { return name; }
};