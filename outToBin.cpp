#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstring>

using namespace std;

int main(int argc, char **argv) {
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    char *buffer = new char[200000];
    uint64_t *hash_buffer = new uint64_t[200000];

    unordered_map<char, string> mp;
    mp['0'] = "0000", mp['1'] = "0001", mp['2'] = "0010", mp['3'] = "0011";
    mp['4'] = "0100", mp['5'] = "0101", mp['6'] = "0110", mp['7'] = "0111";
    mp['8'] = "1000", mp['9'] = "1001", mp['a'] = "1010", mp['b'] = "1011";
    mp['c'] = "1100", mp['d'] = "1101", mp['e'] = "1110", mp['f'] = "1111";

    while(in >> buffer) {
        int len = strlen(buffer);
        for (int i=0;i<len;i++) {
            out << mp[buffer[i]];
        }
        out << endl;
    }
}