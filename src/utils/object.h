#pragma once
#include <memory>
#include <variant>
#include <string>

class LoxCallable;
class LoxInstance;

class Object
{
public:
    std::variant<std::monostate, std::string, double, bool, std::shared_ptr<LoxCallable>, std::shared_ptr<LoxInstance>, LoxInstance*> literal;
    Object() : literal(std::monostate{}) {}
    Object(double d) : literal(d) {}
    Object(const std::string& s) : literal(s) {}
    Object(bool b) : literal(b) {}
    Object(std::shared_ptr<LoxCallable> v) : literal(v) {}
    Object(std::shared_ptr<LoxInstance> i) : literal(i) {}
};