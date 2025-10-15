#pragma once

#include "lox_class.h"
#include "utils/object.h"
#include <string>
#include <unordered_map>
class LoxInstance {
    LoxClass* klass;
    std::unordered_map<std::string, Object> fields;
public:
    LoxInstance(LoxClass* _klass) : klass(_klass){}

    Object get(Token name);
    void set(Token name, Object value);

    std::string to_string()
    {
        return klass->to_string() + " instance";
    }
};