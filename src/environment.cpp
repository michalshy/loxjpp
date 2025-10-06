#include "environment.h"
#include "utils/errors.hpp"
#include "utils/object.h"
#include "utils/tokens.hpp"

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