#include "lox_function.h"
#include "environment.h"
#include "interpreter.h"
#include "utils/errors.hpp"
#include "utils/object.h"
#include <memory>

Object LoxFunction::call(Interpreter* interpreter, const std::vector<Object>& arguments)
{
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(Globals::GetInstance()->get_env());
    for(int i = 0; i < declaration->params.size(); i++)
    {
        environment->define(declaration->params[i].m_Lexeme, arguments[i]);
    }

    try {
        interpreter->executeBlock(declaration->body, environment);
    } catch (ReturnVal returnVal) {
        return returnVal.get();
    }
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