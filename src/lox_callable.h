#pragma once

#include "expr.h"
#include "interpreter.h"
#include "utils/object.h"
#include <memory>
#include <vector>
class LoxCallable
{
public:
    virtual Object call(Interpreter* interpreter, std::vector<Object> arguments) = 0;
};