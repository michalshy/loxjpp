#include <iostream>
#include "lox.h"


int main(int argc, char** argv) {

    Lox lox_client = Lox();
        
    if(argc > 2) {
        std::cout << "Usage: loxjpp [script]" << std::endl;
    } else if (argc == 2) {
        lox_client.run_file(argv[1]);
    } else {
        lox_client.run_prompt();
    }

    if(Lox::had_error)
        return 65;
    
    return 0;
}