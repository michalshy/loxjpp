#include <cstdlib>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

void define_ast(std::string output, std::string base_name, std::vector<std::string> types)
{
    std::filesystem::path dir(output + "/");

    std::string base_name_file = base_name;
    std::transform(base_name_file.begin(), base_name_file.end(), base_name_file.begin(),
        [](unsigned char c){ return std::tolower(c); });

    std::filesystem::path file( base_name_file + ".h");


    std::ofstream out((dir / file).string());
    out << "#pragma once" << std::endl;
    out << "#include \"utils/tokens.hpp\"" << std::endl;
    out << "class " << base_name << "{" << std::endl;
    out << "};" << std::endl;

    for (const auto& type : types)
    {
        size_t colon = type.find(':');
        std::string class_name = type.substr(0, colon);
        std::string fields_concat = type.substr(colon + 1);

        out << "class " << class_name << " : public " << base_name << " {" << std::endl;

        // Split fields_concat by ", "
        std::vector<std::string> fields;
        size_t start = 0;
        // Use a stringstream to split fields_concat by ", "
        std::istringstream ss(fields_concat);
        std::string field;
        while (std::getline(ss, field, ',')) {
            // Remove leading spaces
            size_t first = field.find_first_not_of(' ');
            if (first != std::string::npos)
            {
                field = field.substr(first);
            }
            fields.push_back(field);
        }
        for (const auto& field : fields) {
            out << "    " << field << ";" << std::endl;
        }
        out << "};" << std::endl;
    }
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
        "Binary : Expr left, Token op, Expr right",
        "Grouping : Expr expression",
        "Literal : Object value",
        "Unary : Token op, Expr right"
    });
}