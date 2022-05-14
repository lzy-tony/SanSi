#include "sansi.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>

using namespace std;

const int MAX_BLOCKS = 10000000;

uint64_t* genRandBytes() {
    uint64_t *ret = new uint64_t[BlockSize];
    unsigned char *tmp = new unsigned char[BlockCharSize];
    for (int i = 0; i < BlockCharSize; i++) {
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

int main() {
    uint64_t *tsrc = genRandBytes();
    uint64_t *tpad = genPad();
    Sansi crypto;
    for(int i = 0; i < 2; i++) { // warmup
        crypto.add_block(tsrc);
    }
    int start = clock();
    for(int i = 0; i < MAX_BLOCKS; i++) {
        crypto.add_block(tsrc);
    }
    crypto.add_block(tpad);
    crypto.hash();
    int end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "computation time: " << duration << " s" << endl;
    cout << "performance: " << (double)MAX_BLOCKS * 1024 / duration / 1000000 << " Mbps" << endl;
    delete[] tsrc, tpad;
    return 0;
}