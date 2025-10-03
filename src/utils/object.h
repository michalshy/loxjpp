#pragma once
#include <memory>
#include <utility>
#include <variant>
#include <functional>
#include <string>

class LoxCallable;

class Object
{
public:
    std::variant<std::monostate, std::string, double, bool, std::shared_ptr<LoxCallable>> literal;
    Object() : literal(std::monostate{}) {}
    Object(double d) : literal(d) {}
    Object(const std::string& s) : literal(s) {}
    Object(bool b) : literal(b) {}
    Object(std::shared_ptr<LoxCallable> v) : literal(v) {}
};