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

uint64_t* genPad() {
    uint8_t *ptr = new uint8_t[BlockCharSize];
    memset(ptr, 0, BlockCharSize);
    ptr[0] = 0x80;
    ptr[BlockCharSize - 1] = 0x1;
    return (uint64_t *)ptr;
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
    uint64_t *tpad = genPad();
    Sansi crypto;
    for (int i=0;i<MAX_TURN;i++) {
        if(i % 100000 == 0) {
            cerr << "calculating " << i << " th round (" << float(i) * 100 / MAX_TURN << "%)" << endl;
        }
        uint64_t *tsrc = genRandBytes();
        crypto.add_block(tsrc);
        crypto.add_block(tpad);
        string tret = crypto.hash();
        crypto.reset();
        if(birth_table.count(tret) && !memcmp(tsrc, birth_table[tret], BlockCharSize)) {
            outBytes(tsrc, BlockCharSize), outBytes(birth_table[tret], BlockCharSize);
            cout << tret << endl;
            break;
        }
        birth_table[tret] = tsrc;
    }
    for (auto pair: birth_table) {
        delete[] pair.second;
    }
    delete[] tpad;
    return 0;
}