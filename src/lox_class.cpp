#include "lox_class.h"
#include "lox_instance.h"
#include <memory>

Object LoxClass::call(Interpreter *interpreter, const std::vector<Object> &arguments)
{
    std::shared_ptr<LoxInstance> instance = std::make_shared<LoxInstance>(this);
    return Object(instance);
}

int LoxClass::arity()
{
    return 0;
}