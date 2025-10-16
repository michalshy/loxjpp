#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include "lox_callable.h"
#include "lox_function.h"
#include "utils/object.h"

class LoxClass : public LoxCallable, std::enable_shared_from_this<LoxClass>
{
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
    std::shared_ptr<LoxClass> superclass;
public:
    LoxClass(std::string _name, Object _superclass, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> _methods) : name(_name), methods(_methods) 
    {
        if(std::holds_alternative<std::shared_ptr<LoxClass>>(_superclass.literal))
        {
            superclass = std::get<std::shared_ptr<LoxClass>>(_superclass.literal);
        }
        else 
        {
            superclass = nullptr;
        }
    }

    Object call(Interpreter *interpreter, const std::vector<Object> &arguments) override;
    int arity() override;

    std::shared_ptr<LoxFunction> findMethod(std::string name);

    std::string to_string() override { return name; }
};