#include "lox_function.h"
#include "environment.h"
#include "interpreter.h"
#include "utils/errors.hpp"
#include "utils/object.h"
#include <memory>
#include "lox_instance.h"

Object LoxFunction::call(Interpreter* interpreter, const std::vector<Object>& arguments)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    for(int i = 0; i < declaration->params.size(); i++)
    {
        environment->define(declaration->params[i].m_Lexeme, arguments[i]);
    }

    try {
        interpreter->executeBlock(declaration->body, environment);
    } catch (ReturnVal returnVal) {
        if(isInitializer) return closure->getAt(0, "this");

        return returnVal.get();
    }

    if(isInitializer) return closure->getAt(0, "this");
    return Object();
}

int LoxFunction::arity()
{
    return declaration->params.size();
}

std::string LoxFunction::to_string()
{
    return "<fn " + declaration->name.m_Lexeme + ">";
}

std::shared_ptr<LoxFunction> LoxFunction::bind(LoxInstance* instance)
{
    std::shared_ptr<Environment> new_env = std::make_shared<Environment>(closure);
    new_env->define("this", Object(std::make_shared<LoxInstance>(*instance)));
    return std::make_shared<LoxFunction>(declaration, new_env, isInitializer);
}
