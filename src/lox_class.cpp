#include "lox_class.h"
#include "lox_function.h"
#include "lox_instance.h"
#include <memory>

Object LoxClass::call(Interpreter *interpreter, const std::vector<Object> &arguments)
{
    LoxInstance instance(this);
    std::shared_ptr<LoxFunction> initializer = findMethod("init");
    if(initializer != nullptr)
    {
        initializer->bind(&instance)->call(interpreter, arguments);
    }
    return Object(std::make_shared<LoxInstance>(instance));
}

int LoxClass::arity()
{
    LoxInstance instance(this);
    std::shared_ptr<LoxFunction> initializer = findMethod("init");
    if(initializer == nullptr) return 0;
    return initializer->arity();
}

std::shared_ptr<LoxFunction> LoxClass::findMethod(std::string name)
{
    if(methods.find(name) != methods.end())
    {
        return methods[name];
    }
    return nullptr;
}
