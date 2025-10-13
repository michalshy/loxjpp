#pragma once

#include "lox_class.h"
#include <memory>
#include <string>
class LoxInstance {
    std::shared_ptr<LoxClass> klass;

public:
    LoxInstance(std::shared_ptr<LoxClass> _klass) : klass(_klass){}
    LoxInstance(LoxClass* _klass) : klass(_klass){}

    std::string to_string()
    {
        return klass->to_string() + " instance";
    }
};