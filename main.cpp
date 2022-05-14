#include "sansi.h"

#include <iostream>
#include <fstream>
#include <cstring>

void consoleStringHashDemo() {
    Sansi crypto;
    std::string input;
    std::cout << "> ";
    while (std::cin >> input) {
        std::cout << crypto.stringHash(input) << std::endl << "> ";
    }
}

void stringHashDemo(const char *src, const char *dest) {
    std::ifstream in(src);
    std::ofstream out(dest);
    std::string line;
    Sansi crypto;
    while (std::getline(in, line)) {
        out << crypto.stringHash(line) << std::endl;
    }
    in.close();
    out.close();
}

void fileHashDemo(const char *path) {
    Sansi crypto;
    std::cout << crypto.fileHash(path) << std::endl;
}

int main(int argc, char **argv) {
    switch (argc) {
        case 1:
            consoleStringHashDemo();
            break;
        case 2:
            fileHashDemo(argv[1]);
            break;
        case 3:
            stringHashDemo(argv[1], argv[2]);
            break;
        default:
            std::cerr << "Error" << std::endl;
            break;
    }
}