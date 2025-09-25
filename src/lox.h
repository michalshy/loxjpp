#pragma once
#include <string>
#include "interpreter.h"

class Lox
{
    public:
    void run_file(const std::string& file_name);
    void run_prompt();
    
    static bool s_HadError;
    static bool s_HadRuntimeError;
private:
    static Interpreter interpreter;
    void run(const std::string&);
};
