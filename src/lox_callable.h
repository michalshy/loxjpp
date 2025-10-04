#pragma once

#include "interpreter.h"
#include "utils/object.h"
#include <vector>

class LoxCallable
{
public:
    virtual Object call(Interpreter* interpreter, const std::vector<Object>& arguments) = 0;
    virtual int arity() = 0;
    virtual std::string to_string() = 0;
};