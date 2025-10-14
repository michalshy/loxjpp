#pragma once

#include "lox_class.h"
#include "utils/object.h"
#include <memory>
#include <string>
#include <unordered_map>
class LoxInstance {
    std::shared_ptr<LoxClass> klass;
    std::unordered_map<std::string, Object> fields;
public:
    LoxInstance(std::shared_ptr<LoxClass> _klass) : klass(_klass){}
    LoxInstance(LoxClass* _klass) : klass(_klass){}

    Object get(Token name);

    std::string to_string()
    {
        return klass->to_string() + " instance";
    }
};