#ifndef SANSI_H
#define SANSI_H

#include <queue>
#include <string>

typedef unsigned long uint64_t;

const int StateSize = 1600 / (8 * 8);
const int BlockSize = 1024 / (8 * 8);

class HashTensor {
private:
    uint64_t pos;
    uint64_t m_hash[StateSize];
public:
    uint64_t h_index;

    inline void reset();
    inline void set_ones(uint64_t _pos);
    inline void set_zeros(uint64_t _pos);
    inline void set_pos(uint64_t _pos);
    inline void set_hash(uint64_t _pos, uint64_t val);
    inline void calc_index();
    inline void f_function();
    inline void embed(const uint64_t *data);
    inline uint64_t get_mask();

    bool operator < (const HashTensor &t) const {
        return h_index < t.h_index;
    }

    HashTensor operator + (const HashTensor &t) const {
        HashTensor r;
        for(int i = 0; i < StateSize; i++) {
            r.set_hash(i, m_hash[i] ^ t.m_hash[i]);
        }
        r.set_pos((pos + t.pos) >> 1);
        r.calc_index();
        return r;
    }
};

class Sansi {
private:
    int cnt;
    std::priority_queue <HashTensor> q;
public:
    Sansi();
    void reset();
    void add_block(const uint64_t *data);
    HashTensor merge2();
    std::string hash();
};

#endif