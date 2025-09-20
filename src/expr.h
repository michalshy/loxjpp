#pragma once
#include "utils/tokens.hpp"
class Expr{
};
class Binary  : public Expr {
    Expr left;
    Token op;
    Expr right;
};
class Grouping  : public Expr {
    Expr expression;
};
class Literal  : public Expr {
    Object value;
};
class Unary  : public Expr {
    Token op;
    Expr right;
};
