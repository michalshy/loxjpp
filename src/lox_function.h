#pragma once

#include "lox_callable.h"
class LoxFunction : public LoxCallable
{
public:
    Object call(Interpreter* interpreter, const std::vector<Object>& arguments) override;
};