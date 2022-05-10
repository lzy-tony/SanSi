#include "sansi.h"

#include <iostream>
#include <cstring>

const int Round = 2;
const int halosize = 1;
const int local_x = 5;
const int local_y = 5;
const int local_z = 4;
const int block_group = 8;

const uint64_t XorMasks[Round] =
{
    0xd76aa478e8c7b756ULL, 0x242070dbc1bdceeeULL
};

const uint64_t bias = 0x0000000000000fafULL;

inline uint64_t rotateLeft(uint64_t x, uint8_t numBits) {
    return (x << numBits) | (x >> (64 - numBits));
}

inline int index(int x, int y, int z, int ldy, int ldz) {
    return z + ldz * (y + ldy * x);
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
    calc_index();
}

uint64_t HashTensor::get_mask() {
    uint64_t mask = (1 << 4) - 1;
    return m_hash[0] & mask;
}

void HashTensor::print() {
    std::cerr << "printing tensor" << std::endl;
    for(int i = 0; i < StateSize; ++i) {
        std::cerr << m_hash[i] << std::endl;
    }
    std::cerr << "end print" << std::endl;
}

unsigned int mod5(uint64_t x) {
    return x % 5;
}

unsigned int mod64(uint64_t x) {
    return x % 64;
}

void HashTensor::f_function() {
    // re-compute state
    static uint64_t grid[(local_x + 2 * halosize) * (local_y + 2 * halosize) * (local_z + 2 * halosize)];
    static uint64_t aux[(local_x + 2 * halosize) * (local_y + 2 * halosize) * (local_z + 2 * halosize)];
    std::memset(grid, 0, sizeof(grid));
    std::memset(aux, 0, sizeof(aux));
    uint64_t *buffer[2] = {grid, aux};
    int ldy = local_y + 2 * halosize;
    int ldz = local_z + 2 * halosize;
    int x_start = halosize, x_end = halosize + local_x;
    int y_start = halosize, y_end = halosize + local_y;
    int z_start = halosize, z_end = halosize + local_z;
    int nt = 2;
    uint64_t mask16 = ((1 << 16) - 1);

    for (unsigned int round = 0; round < Round; round++) {
        // Sigma
        // decompose uint64_t to four 16 bit
        for(int i = 0; i < local_x; i++) {
            for(int j = 0; j < local_y; j++) {
                buffer[0][index(i + x_start, j + y_start, halosize + 3, ldy, ldz)] = (m_hash[i * local_x + j] & mask16);
                buffer[0][index(i + x_start, j + y_start, halosize + 2, ldy, ldz)] = ((m_hash[i * local_x + j] >> 16) & mask16);
                buffer[0][index(i + x_start, j + y_start, halosize + 1, ldy, ldz)] = ((m_hash[i * local_x + j] >> 32) & mask16);
                buffer[0][index(i + x_start, j + y_start, halosize, ldy, ldz)] = ((m_hash[i * local_x + j] >> 48) & mask16);
            }
        }
        uint64_t *a0, *a1;
        for(int t = 0; t < nt; ++t) {
            a0 = buffer[t % 2], a1 = buffer[(t + 1) % 2];
            for(int x = x_start; x < x_end; ++x) {
                for(int y = y_start; y < y_end; ++y) {
                    for(int z = z_start; z < z_end; ++z) {
                        uint64_t tmp =
                            pos * bias
                            + a0[index(x - 1, y, z, ldy, ldz)]
                            + a0[index(x, y - 1, z, ldy, ldz)]
                            + a0[index(x, y, z - 1, ldy, ldz)]
                            + a0[index(x, y, z, ldy, ldz)]
                            + a0[index(x, y, z + 1, ldy, ldz)]
                            + a0[index(x, y + 1, z, ldy, ldz)]
                            + a0[index(x + 1, y, z, ldy, ldz)];
                        while(tmp > mask16)
                            tmp = tmp % mask16 + tmp / mask16;
                        a1[index(x, y, z, ldy, ldz)] = tmp;
                    }
                }
            }
        }
        // write back
        for(int i = 0; i < local_x; i++) {
            for(int j = 0; j < local_y; j++) {
                int idx = i * local_y + j;
                m_hash[idx] = a1[index(i + x_start, j + y_start, halosize + 3, ldy, ldz)], m_hash[idx] <<= 16;
                m_hash[idx] |= a1[index(i + x_start, j + y_start, halosize + 2, ldy, ldz)], m_hash[idx] <<= 16;
                m_hash[idx] |= a1[index(i + x_start, j + y_start, halosize + 1, ldy, ldz)], m_hash[idx] <<= 16;
                m_hash[idx] |= a1[index(i + x_start, j + y_start, halosize, ldy, ldz)];
            }
        }

        // Rho Pi
        uint64_t last = m_hash[1];
        uint64_t one;
                          m_hash[ 0] = rotateLeft(last, mod64(pos +  0));
        one = m_hash[10]; m_hash[10] = rotateLeft(last, mod64(pos +  1)); last = one;
        one = m_hash[ 7]; m_hash[ 7] = rotateLeft(last, mod64(pos +  3)); last = one;
        one = m_hash[11]; m_hash[11] = rotateLeft(last, mod64(pos +  6)); last = one;
        one = m_hash[17]; m_hash[17] = rotateLeft(last, mod64(pos + 10)); last = one;
        one = m_hash[18]; m_hash[18] = rotateLeft(last, mod64(pos + 15)); last = one;
        one = m_hash[ 3]; m_hash[ 3] = rotateLeft(last, mod64(pos + 21)); last = one;
        one = m_hash[ 5]; m_hash[ 5] = rotateLeft(last, mod64(pos + 28)); last = one;
        one = m_hash[16]; m_hash[16] = rotateLeft(last, mod64(pos + 36)); last = one;
        one = m_hash[ 8]; m_hash[ 8] = rotateLeft(last, mod64(pos + 45)); last = one;
        one = m_hash[21]; m_hash[21] = rotateLeft(last, mod64(pos + 55)); last = one;
        one = m_hash[24]; m_hash[24] = rotateLeft(last, mod64(pos +  2)); last = one;
        one = m_hash[ 4]; m_hash[ 4] = rotateLeft(last, mod64(pos + 14)); last = one;
        one = m_hash[15]; m_hash[15] = rotateLeft(last, mod64(pos + 27)); last = one;
        one = m_hash[23]; m_hash[23] = rotateLeft(last, mod64(pos + 41)); last = one;
        one = m_hash[19]; m_hash[19] = rotateLeft(last, mod64(pos + 56)); last = one;
        one = m_hash[13]; m_hash[13] = rotateLeft(last, mod64(pos +  8)); last = one;
        one = m_hash[12]; m_hash[12] = rotateLeft(last, mod64(pos + 25)); last = one;
        one = m_hash[ 2]; m_hash[ 2] = rotateLeft(last, mod64(pos + 43)); last = one;
        one = m_hash[20]; m_hash[20] = rotateLeft(last, mod64(pos + 62)); last = one;
        one = m_hash[14]; m_hash[14] = rotateLeft(last, mod64(pos + 18)); last = one;
        one = m_hash[22]; m_hash[22] = rotateLeft(last, mod64(pos + 39)); last = one;
        one = m_hash[ 9]; m_hash[ 9] = rotateLeft(last, mod64(pos + 61)); last = one;
        one = m_hash[ 6]; m_hash[ 6] = rotateLeft(last, mod64(pos + 20)); last = one;
                          m_hash[ 1] = rotateLeft(last, mod64(pos + 44));

        // Alpha
        for(unsigned int j = 0; j < StateSize; j += 5) {
            uint64_t one = m_hash[j];
            uint64_t two = m_hash[j + 1];
            uint64_t three = m_hash[j + 2];
            uint64_t four = m_hash[j + 3];

            m_hash[j]     ^= m_hash[j + 2] & ~m_hash[j + 4];
            m_hash[j + 1] ^= m_hash[j + 3] & ~one;
            m_hash[j + 2] ^= m_hash[j + 4] & ~two;
            m_hash[j + 3] ^=      one      & ~three;
            m_hash[j + 4] ^=      three    & ~four;
        }

        // Chi
        // embed round mask into state
        uint64_t sum = 0;
        for (unsigned int i = 0; i < StateSize; i++) {
            sum ^= m_hash[i];
        }
        m_hash[sum % StateSize] ^= XorMasks[round];
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
    hashTensor.f_function();
    hashTensor.calc_index();

    // update using existing three blocks 
    q.push(hashTensor);
    if(q.size() == block_group) {
        merge_n();
    }
}

void Sansi::merge_2() {
    HashTensor tensor1 = q.top();
    q.pop();
    HashTensor tensor2 = q.top();
    q.pop();
    tensor1 = tensor1 + tensor2;
    tensor1.f_function();
    tensor1.calc_index();
    q.push(tensor1);
}

void Sansi::merge_n() {
    while(q.size() > 1) {
        merge_2();
    }
}

std::string Sansi::hash() {
    static const char dec2hex[16 + 1] = "0123456789abcdef";
    merge_n();
    std::string result;
    HashTensor hashTensor = q.top();
    for(int i = 0; i < 20; i++) {
        result += dec2hex[hashTensor.get_mask()];
        hashTensor.f_function();
    }
    return result;
}