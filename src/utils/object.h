#pragma once
#include <variant>
#include <string>

class Object
{
public:
    std::variant<std::monostate, std::string, double, bool> literal;
    Object() : literal(std::monostate{}) {}
    Object(double d) : literal(d) {}
    Object(const std::string& s) : literal(s) {}
    Object(bool b) : literal(b) {}
};