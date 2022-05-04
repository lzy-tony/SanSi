#include "sansi.h"

#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char **argv) {
    std::ifstream in(argv[1]);
    std::ofstream out(argv[2]);
    char *buffer = new char[200000];
    uint64_t *hash_buffer = new uint64_t[200000];
    while(in >> buffer) {
        int len = strlen(buffer);
        while (len % BlockCharSize != 0) {
			buffer[len++] = '\0';
		}
        Sansi crypto;
        memcpy(hash_buffer, buffer, len);
        for(int i = 0; i < (len >> 8); i += BlockSize) {
            crypto.add_block(hash_buffer + i);
        }
        out << crypto.hash() << std::endl;
    }
}