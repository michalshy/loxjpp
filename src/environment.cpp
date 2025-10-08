#include "environment.h"
#include "utils/errors.hpp"
#include "utils/object.h"
#include "utils/tokens.hpp"
#include <memory>

void Environment::define(std::string name, Object value)
{
    values[name] = value;
}

Object Environment::get(Token name)
{
    
    if(values.find(name.m_Lexeme) != values.end())
    {
        return values[name.m_Lexeme];
    }

    if(enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable '" + name.m_Lexeme + "'"); // errors here
}

void Environment::assign(Token name, Object value)
{
    if(values.find(name.m_Lexeme) != values.end())
    {
        values[name.m_Lexeme] = value;
        return;
    }

    if(enclosing != nullptr)
    {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.m_Lexeme + "'.");
}

Object Environment::getAt(int distance, std::string name)
{
    return ancestor(distance)->values[name];
}

void Environment::assignAt(int distance, Token name, Object value)
{
    ancestor(distance)->values[name.m_Lexeme] = value;
}

Environment* Environment::ancestor(int distance)
{
    Environment* env = this;
    for(int i = 0; i < distance; i++)
    {
        env = env->enclosing.get();
    }
    return env;
}
