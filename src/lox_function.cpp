#include "lox_function.h"
#include "environment.h"
#include "interpreter.h"
#include "utils/object.h"
#include <memory>

Object LoxFunction::call(Interpreter* interpreter, const std::vector<Object>& arguments)
{
    Environment env = Environment(globals);
    for(int i = 0; i < declaration->params.size(); i++)
    {
        env.define(declaration->params[i].m_Lexeme, arguments[i]);
    }

    interpreter->executeBlock(declaration->body, std::make_shared<Environment>(env));
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