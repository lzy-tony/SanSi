#include "sansi.h"

#include <iostream>

// const int Round = 24;
const int Round = 2;

// const uint64_t XorMasks[Round] =
// {
//     0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
//     0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
//     0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
//     0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
//     0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
//     0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
//     0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
//     0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
// };

const uint64_t XorMasks[Round] =
{
    0x0000000000000001ULL, 0x0000000000008082ULL
};

inline uint64_t rotateLeft(uint64_t x, uint8_t numBits) {
    return (x << numBits) | (x >> (64 - numBits));
}


HashTensor::HashTensor() {
    pos = 0;
    for(int i = 0; i < StateSize; i++) {
        m_hash[i] = 0;
    }
}

void HashTensor::reset() {
    for(int i = 0; i < StateSize; i++) {
        m_hash[i] = 0;
    }
}

void HashTensor::calc_index() {
    h_index = pos;
    for(int i = 0; i < StateSize; i++) {
        h_index ^= m_hash[i];
    }
}

void HashTensor::set_ones(uint64_t _pos) {
    pos = _pos;
    for(int i = 0; i < StateSize; i++) {
        m_hash[i] = -1;
    }
    HashTensor::calc_index();
}

void HashTensor::set_zeros(uint64_t _pos) {
    pos = _pos;
    for(int i = 0; i < StateSize; i++) {
        m_hash[i] = 0;
    }
    HashTensor::calc_index();
}

void HashTensor::set_pos(uint64_t _pos) {
    pos = _pos;
}

void HashTensor::set_hash(uint64_t _pos, uint64_t val) {
    m_hash[_pos] = val;
}

void HashTensor::embed(uint64_t *data) {
    for(int i = 0; i < BlockSize; i++) {
        m_hash[i] ^= data[i];
    }
}

uint64_t HashTensor::get_mask() {
    uint64_t mask = (1 << 4) - 1;
    return m_hash[0] & mask;
}

unsigned int mod5(unsigned int x)
{
    if (x < 5)
        return x;
    return x - 5;
}

void HashTensor::f_function() {
    // re-compute state
    for (unsigned int round = 0; round < Round; round++) {
        // Theta
        uint64_t coefficients[5];
        for (unsigned int i = 0; i < 5; i++)
            coefficients[i] = m_hash[i] ^ m_hash[i + 5] ^ m_hash[i + 10] ^ m_hash[i + 15] ^ m_hash[i + 20];

        for (unsigned int i = 0; i < 5; i++) {
            uint64_t one = coefficients[mod5(i + 4)] ^ rotateLeft(coefficients[mod5(i + 1)], 1);
            m_hash[i     ] ^= one;
            m_hash[i +  5] ^= one;
            m_hash[i + 10] ^= one;
            m_hash[i + 15] ^= one;
            m_hash[i + 20] ^= one;
        }

        // temporary
        uint64_t one;

        // Rho Pi
        uint64_t last = m_hash[1];
        one = m_hash[10]; m_hash[10] = rotateLeft(last,  1); last = one;
        one = m_hash[ 7]; m_hash[ 7] = rotateLeft(last,  3); last = one;
        one = m_hash[11]; m_hash[11] = rotateLeft(last,  6); last = one;
        one = m_hash[17]; m_hash[17] = rotateLeft(last, 10); last = one;
        one = m_hash[18]; m_hash[18] = rotateLeft(last, 15); last = one;
        one = m_hash[ 3]; m_hash[ 3] = rotateLeft(last, 21); last = one;
        one = m_hash[ 5]; m_hash[ 5] = rotateLeft(last, 28); last = one;
        one = m_hash[16]; m_hash[16] = rotateLeft(last, 36); last = one;
        one = m_hash[ 8]; m_hash[ 8] = rotateLeft(last, 45); last = one;
        one = m_hash[21]; m_hash[21] = rotateLeft(last, 55); last = one;
        one = m_hash[24]; m_hash[24] = rotateLeft(last,  2); last = one;
        one = m_hash[ 4]; m_hash[ 4] = rotateLeft(last, 14); last = one;
        one = m_hash[15]; m_hash[15] = rotateLeft(last, 27); last = one;
        one = m_hash[23]; m_hash[23] = rotateLeft(last, 41); last = one;
        one = m_hash[19]; m_hash[19] = rotateLeft(last, 56); last = one;
        one = m_hash[13]; m_hash[13] = rotateLeft(last,  8); last = one;
        one = m_hash[12]; m_hash[12] = rotateLeft(last, 25); last = one;
        one = m_hash[ 2]; m_hash[ 2] = rotateLeft(last, 43); last = one;
        one = m_hash[20]; m_hash[20] = rotateLeft(last, 62); last = one;
        one = m_hash[14]; m_hash[14] = rotateLeft(last, 18); last = one;
        one = m_hash[22]; m_hash[22] = rotateLeft(last, 39); last = one;
        one = m_hash[ 9]; m_hash[ 9] = rotateLeft(last, 61); last = one;
        one = m_hash[ 6]; m_hash[ 6] = rotateLeft(last, 20); last = one;
                        m_hash[ 1] = rotateLeft(last, 44);

        // Chi
        for (unsigned int j = 0; j < StateSize; j += 5) {
        // temporaries
        uint64_t one = m_hash[j];
        uint64_t two = m_hash[j + 1];

        m_hash[j]     ^= m_hash[j + 2] & ~two;
        m_hash[j + 1] ^= m_hash[j + 3] & ~m_hash[j + 2];
        m_hash[j + 2] ^= m_hash[j + 4] & ~m_hash[j + 3];
        m_hash[j + 3] ^=      one      & ~m_hash[j + 4];
        m_hash[j + 4] ^=      two      & ~one;
        }

        // Iota
        m_hash[0] ^= XorMasks[round];
    }
}

Sansi::Sansi() {
    cnt = 0;
    while(!q.empty()) {
        q.pop();
    }
}

void Sansi::reset() {
    cnt = 0;
    while(!q.empty())
        q.pop();
}

void Sansi::add_block(uint64_t *data) { //assume block is 1024 bits long
    // process block
    cnt++;
    HashTensor hashTensor;
    (cnt & 1) ? hashTensor.set_zeros(cnt) : hashTensor.set_ones(cnt);
    hashTensor.embed(data);

    // update using existing three blocks 
    q.push(hashTensor);
    if(q.size() == 3) {
        merge2();
    }
}

void Sansi::merge2() {
    HashTensor tensor1 = q.top();
    q.pop();
    HashTensor tensor2 = q.top();
    q.pop();
    tensor1 = tensor1 + tensor2;
    tensor1.f_function();
    tensor1.calc_index();
    q.push(tensor1);
}

std::string Sansi::hash() {
    static const char dec2hex[16 + 1] = "0123456789abcdef";
    while(q.size() > 1) {
        merge2();
    }
    std::string result;
    HashTensor hashTensor = q.top();
    for(int i = 0; i < 20; i++) {
        // std::cerr << hashTensor.get_mask() << " ";
        result += dec2hex[hashTensor.get_mask()];
        hashTensor.f_function();
    }
    return result;
}