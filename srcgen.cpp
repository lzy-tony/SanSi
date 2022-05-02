#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int msg_len = 256, msg_num = 2048;

const string maps = "0123456789abcdefghijklmnopqrstuvwxyz,./<>?;':\"{}\\|!@#$%^&*()";
int main(int argc, char **argv) {
    if (argc < 2)
        return 0;
    
    std::ofstream fout(argv[1]);
    if(argc > 2) {
        msg_len = atoi(argv[2]);
        if (argc > 3)
            msg_num = atoi(argv[3]);
    }

    int *msg = new int[msg_len];
    memset(msg, 0, sizeof(int) * msg_len);
    srand(time(0));

    for (int msg_cnt = 0; msg_cnt < msg_num; msg_cnt++) {
        int delta = rand() % ((int)maps.length()) + 1;

        // msg[0] += delta;
        for (int i=0;i<msg_len;i+=(rand()%(msg_len-i-1) + 1)) {
            msg[i] += delta;
        }

        for (int i=0;i<msg_len-1;i++) {
            msg[i+1] += msg[i] / ((int)maps.length());
            msg[i] %= ((int)maps.length());
        }
        msg[msg_len-1] %= ((int)maps.length());

        for (int i=0;i<msg_len;i++) {
            fout << maps[msg[i]];
        }
        fout << "\n";
    }
    fout.close();
    delete[] msg;
    return 0;
}