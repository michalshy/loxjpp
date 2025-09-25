#include "lox.h"
#include "expr.h"
#include "interpreter.h"
#include "parser.h"
#include "scanner.h"
#include "utils/object.h"
#include "utils/tree_printer.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>

Interpreter Lox::interpreter = Interpreter();
bool Lox::s_HadError = false;
bool Lox::s_HadRuntimeError = false;

void Lox::run_file(const std::string& file_name)
{
    std::ifstream input_file(file_name, std::ios::in | std::ios::binary);
    if (!input_file) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        return;
    }
    std::string str_buffer((std::istreambuf_iterator<char>(input_file)),
                            std::istreambuf_iterator<char>());
    input_file.close();
    run(str_buffer);
}

void Lox::run_prompt()
{
    std::string line;
    std::cout << "> ";

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        run(line);
        std::cout << "> ";

        Lox::s_HadError = false;
    }
}

void Lox::run(const std::string& source)
{
    Scanner scanner = Scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();

    Parser parser = Parser<Object>(tokens);
    std::shared_ptr<Expr<Object>> expression = parser.parse();

    if(s_HadError) return;

    //TreePrinter().print(*expression);
    Lox::interpreter.interpret(expression);
}