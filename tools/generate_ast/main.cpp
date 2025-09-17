#include <cstdlib>
#include <iostream>
#include <vector>

void define_ast(std::string output, std::string base_name, std::vector<std::string>)
{
    
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: generate_ast <output>";
        exit(1);
    }
    std::string output = argv[1];
    define_ast(output, "Expr", std::vector<std::string>{
        "Binary : Expr left, Token operator, Expr right",
        "Grouping : Expr expression",
        "Literal : Object value",
        "Unary : Token operator, Expr right"
    });
}