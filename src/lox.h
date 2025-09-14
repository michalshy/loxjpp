#pragma once
#include <cstddef>
#include <string>
#include <vector>

class Lox
{
public:
    void run_file(std::string file_name);
    void run_prompt();

private:
    void run(std::string&);
};