#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <sstream>

void define_visitor(std::ofstream& stream, std::string base_name, std::vector<std::string> types, std::string visitor_name, std::string visitor_type)
{
    stream << "class " << visitor_name << std::endl << "{" << std::endl;
    std::string base_name_lower = base_name;
    std::transform(base_name_lower.begin(), base_name_lower.end(), base_name_lower.begin(),
        [](unsigned char c){ return std::tolower(c); });
        
    stream << "public:" << std::endl;
    for(const auto& type : types)
    {
        size_t colon = type.find(':');
        std::string class_name = type.substr(0, colon-1);
        stream << "    virtual " << visitor_type << " visit" + class_name + base_name + "(" + class_name + "* " + base_name_lower + ") = 0;" << std::endl; 
    } 
    stream << "};" << std::endl;
}

void define_ast(std::string output, std::string base_name, std::vector<std::string> types, std::string return_type)
{
    std::string visitor_name = "Visitor" + base_name;

    std::filesystem::path dir(output + "/");

    std::string base_name_file = base_name;
    std::transform(base_name_file.begin(), base_name_file.end(), base_name_file.begin(),
        [](unsigned char c){ return std::tolower(c); });

    std::filesystem::path file( base_name_file + ".h");


    std::ofstream out((dir / file).string());

    out << "#pragma once" << std::endl;
    out << "#include \"utils/tokens.hpp\"" << std::endl;
    out << "#include <memory>" << std::endl;

    for (const auto& type : types)
    {
        size_t colon = type.find(':');
        std::string class_name = type.substr(0, colon-1);
        out << "class " << class_name << ";" << std::endl;
    }
    define_visitor(out, base_name, types, visitor_name, return_type);

    out << "class " << base_name << "{" << std::endl;
    out << "public:" << std::endl;
    out << "    virtual ~" << base_name << "() = default;" << std::endl;
    out << "    virtual " << return_type << " accept(" << visitor_name << "* visitor) = 0;" << std::endl;
    out << "};" << std::endl;


    for (const auto& type : types)
    {
        size_t colon = type.find(':');
        std::string class_name = type.substr(0, colon-1);
        std::string fields_concat = type.substr(colon + 1);
        out << "class " << class_name << " : public " << base_name << " {" << std::endl;
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

        out << "    " << return_type << " accept(" << visitor_name << "* visitor) override {" << std::endl;
        out << "        return visitor->visit" + class_name + base_name + "(this);" << std::endl;
        out << "    };" << std::endl;

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
        "Assign : Token name, std::shared_ptr<Expr> value",
        "Binary : std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right",
        "Call : std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments",
        "Grouping : std::shared_ptr<Expr> expression",
        "Literal : Object value",
        "Logical : std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right",
        "Unary : Token op, std::shared_ptr<Expr> right",
        "Variable : Token name"

    }, "Object");

    define_ast(output, "Stmt", std::vector<std::string>{
        "Block : std::vector<std::shared_ptr<Stmt>> statements",
        "Expression : std::shared_ptr<Expr> expression",
        "Function : Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body",
        "If : std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch",
        "Print : std::shared_ptr<Expr> expression",
        "Return : Token keyword, std::shared_ptr<Expr> value",
        "Var : Token name, std::shared_ptr<Expr> initializer",
        "While : std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body",
    }, "void");
}