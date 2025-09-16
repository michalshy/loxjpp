#pragma once
#include <string>

class Lox
{
    public:
    void run_file(const std::string& file_name);
    void run_prompt();
    
    static bool s_HadError;
private:
    void run(const std::string&);
};
