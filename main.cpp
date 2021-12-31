
#include <iostream>
#include "program.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Bad Format! Cringe!\n";
        return -1;
    }
    Program* p = new Program(argv[1]);
    try {
        std::cout << "Exit Code: " << p->execute() << "\n";
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }
    delete p;
}