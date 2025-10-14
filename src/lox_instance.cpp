#include "lox_instance.h"
#include "utils/errors.hpp"

Object LoxInstance::get(Token name)
{
    if(fields.find(name.m_Lexeme) != fields.end())
    {
        return fields[name.m_Lexeme];
    }
    
    throw RuntimeError(name, "Undefined property '" + name.m_Lexeme + "'.");
}