#include "sansi.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <string>
#include <cstring>

using namespace std;

const int MAX_TURN = (1 << 28);

unordered_map<string, uint64_t*> birth_table;

uint64_t* genRandBytes() {
    uint64_t *ret = new uint64_t[BlockSize];
    unsigned char *tmp = new unsigned char[BlockCharSize];
    for (int i=0;i<BlockCharSize;i++) {
        tmp[i] = (unsigned char)(rand() % 256);
    }
    memcpy(ret, tmp, BlockCharSize);
    delete[] tmp;
    return ret;
}

void outBytes(const void *src, const int length) {
    unsigned char *tmp = (unsigned char *)src;
    for (int i=0;i<length;i++) {
        cout << hex << ((int)tmp[i]);
    }
    cout << endl;
}

int main() {
    srand(time(0));
    for (int i=0;i<MAX_TURN;i++) {
        uint64_t *tsrc = genRandBytes();
        Sansi crypto;
        crypto.add_block(tsrc);
        string tret = crypto.hash();
        if(birth_table.count(tret) && !memcmp(tsrc, birth_table[tret], BlockCharSize)) {
            outBytes(tsrc, BlockCharSize), outBytes(birth_table[tret], BlockCharSize);
            cout << tret << endl;
            break;
        }
        birth_table[tret] = tsrc;
    }
    return 0;
}