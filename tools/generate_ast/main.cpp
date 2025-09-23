#include <cstdlib>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

void define_visitor(std::ofstream& stream, std::string base_name, std::vector<std::string> types)
{
    stream << "template<class T>" << std::endl << "class Visitor" << std::endl << "{" << std::endl;
    std::string base_name_lower = base_name;
    std::transform(base_name_lower.begin(), base_name_lower.end(), base_name_lower.begin(),
        [](unsigned char c){ return std::tolower(c); });
        
    stream << "public:" << std::endl;
    for(const auto& type : types)
    {
        size_t colon = type.find(':');
        std::string class_name = type.substr(0, colon-1);
        stream << "    virtual T visit" + class_name + base_name + "(" + class_name + "<T>* " + base_name_lower + ") = 0;" << std::endl; 
    } 
    stream << "};" << std::endl;
}

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
    out << "#include <memory>" << std::endl;
    
    out << "template<class T>" << std::endl << "class Visitor;" << std::endl;

    out << "template<class T>" << std::endl;
    out << "class " << base_name << "{" << std::endl;
    out << "public:" << std::endl;
    out << "    virtual ~" << base_name << "() = default;" << std::endl;
    out << "    virtual T accept(Visitor<T>* visitor) = 0;" << std::endl;
    out << "};" << std::endl;


    for (const auto& type : types)
    {
        size_t colon = type.find(':');
        std::string class_name = type.substr(0, colon-1);
        std::string fields_concat = type.substr(colon + 1);
        
        out << "template<class T>" << std::endl;
        out << "class " << class_name << " : public " << base_name << "<T>" << " {" << std::endl;
        out << "public:" << std::endl;
        
        std::string constructor = "    " + class_name + "(" + fields_concat + ") :\n       ";

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

            // extract name of field
            size_t last_space = field.rfind(' ');
            if (last_space != std::string::npos) {
                std::string field_name = field.substr(last_space + 1);
                constructor += field_name + "(" + field_name + "),";
            }
        }
        constructor.pop_back();
        constructor += "{}";
        for (const auto& field : fields) {
            out << "    " << field << ";" << std::endl;
        }

        out << constructor << std::endl;

        out << "    T accept(Visitor<T>* visitor) override {" << std::endl;
        out << "        return visitor->visit" + class_name + base_name + "(this);" << std::endl;
        out << "    };" << std::endl;

        out << "};" << std::endl;
    }
    define_visitor(out, base_name, types);
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
        "Binary : std::unique_ptr<Expr<T>> left, Token op, std::unique_ptr<Expr<T>> right",
        "Grouping : std::unique_ptr<Expr<T>> expression",
        "Literal : Object value",
        "Unary : Token op, std::unique_ptr<Expr<T>> right"
    });
}