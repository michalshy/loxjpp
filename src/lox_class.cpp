#include "lox_class.h"
#include "lox_instance.h"
#include <memory>

Object LoxClass::call(Interpreter *interpreter, const std::vector<Object> &arguments)
{
    LoxInstance instance(this);
    return Object(std::make_shared<LoxInstance>(instance));
}

int LoxClass::arity()
{
    return 0;
}

std::shared_ptr<LoxFunction> LoxClass::findMethod(std::string name)
{
    if(methods.find(name) != methods.end())
    {
        return methods[name];
    }
    return nullptr;
}
