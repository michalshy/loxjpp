#include "lox.h"
#include "scanner.h"
#include <cstddef>
#include <fstream>
#include <ios>
#include <list>
#include <vector>
#include <iostream>

void Lox::run_file(std::string file_name)
{
    std::ifstream input_file(file_name, std::ios_base::binary);
    input_file.seekg(0, std::ios_base::end);
    auto length = input_file.tellg();
    input_file.seekg(0, std::ios_base::beg);

    std::vector<std::byte> buffer(length);
    input_file.read(reinterpret_cast<char*>(buffer.data()), length);
    input_file.close();

    std::string str_buffer(reinterpret_cast<const char*>(buffer.data()), buffer.size());
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