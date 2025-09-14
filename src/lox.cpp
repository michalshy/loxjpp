#include "lox.h"
#include "scanner.h"
#include <cstddef>
#include <fstream>
#include <ios>
#include <list>
#include <vector>
#include <iostream>

bool Lox::had_error = false;

void Lox::run_file(std::string file_name)
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

        Lox::had_error = false;
    }
}

void Lox::run(std::string& source)
{
    Scanner scanner = Scanner(source);
    std::list<Token> tokens = scanner.scan_tokens();

    for (auto& token: tokens) {
        std::cout << token;
    }
}