#pragma once

#include "utils/object.h"
#include <memory>
#include <unordered_map>
#include "utils/tokens.hpp"
#include <string>

class Environment
{
    std::unordered_map<std::string, Object> values;
    std::shared_ptr<Environment> enclosing;
public:
    Environment() = default;
    Environment(std::shared_ptr<Environment> _enclosing) : enclosing(_enclosing){}
    void define(std::string name, Object value);
    Object get(Token name);
    void assign(Token name, Object value);
    int size(){ return values.size(); }
};