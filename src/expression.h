#pragma once

#include "utils/tokens.hpp"

class Expr 
{

};

class BinaryExpr
{
    Expr left;
    Expr right;
    Token op;
};