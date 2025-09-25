#pragma once
#include <variant>
#include <string>

struct Object
{
    std::variant<std::monostate, std::string, double, bool> literal;
public:
    Object() : literal(std::monostate{}) {}
    Object(double d) : literal(d) {}
    Object(const std::string& s) : literal(s) {}
    Object(bool b) : literal(b) {}
};