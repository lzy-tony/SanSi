#include "sansi.h"

#include <iostream>
#include <string>
#include <cstring>

const int MAX_TURN = 1 << 28;

std::string hash(std::string data) {
    static char buffer[20000];
    static uint64_t *ptr = (uint64_t *)buffer;
    static Sansi crypto;
    strcpy(buffer, data.c_str());
    int length = Sansi::pad(buffer, data.length()) >> 3;
    crypto.reset();
    for (int i = 0; i < length; i += BlockSize) {
        crypto.add_block(ptr + i);
    }
    return crypto.hash();
}

int main() {
    std::string trigger("trigger");
    std::string fast(trigger);
    std::string slow(trigger);
    int cnt = 0;
    do {
        if(cnt % 100000 == 0) {
            std::cerr << "calculating " << cnt << " th round (" << float(cnt) * 100 / MAX_TURN << "%)" << std::endl;
        }
        fast = hash(hash(fast));
        slow = hash(slow);
        ++cnt;
    } while (fast != slow && cnt < MAX_TURN);
    std::cout << std::endl;
    if (fast != slow) {
        std::cout << "No collisions found" << std::endl;
    } else {
        std::cout << "Found a collision!" << std::endl;
        std::cout << "Collision point: " << fast << std::endl;
        slow = trigger;
        std::string fast_kage, slow_kage;
        cnt = 0;
        while (fast != slow) {
            if(cnt % 100000 == 0) {
                std::cerr << "calculating " << cnt << " th round" << std::endl;
            }
            fast_kage = fast;
            slow_kage = slow;
            fast = hash(fast);
            slow = hash(slow);
            ++cnt;
        }
        std::cout << slow_kage << " " << fast_kage << std::endl;
        std::cout << "Hash: " << fast << std::endl;
    }
    return 0;
}