#include "lox_instance.h"
#include "lox_function.h"
#include "utils/errors.hpp"
#include "utils/object.h"
#include <memory>

Object LoxInstance::get(Token name)
{
    if(fields.find(name.m_Lexeme) != fields.end())
    {
        return fields[name.m_Lexeme];
    }

    std::shared_ptr<LoxFunction> method = klass->findMethod(name.m_Lexeme);
    if(method != nullptr) return Object(method->bind(this));
    
    throw RuntimeError(name, "Undefined property '" + name.m_Lexeme + "'.");
}

void LoxInstance::set(Token name, Object value)
{
    fields[name.m_Lexeme] = value;
}
