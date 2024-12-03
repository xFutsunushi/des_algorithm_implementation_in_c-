#include <iostream>
#include <vector>
#include <bitset>
#include <cstdint>

using namespace std;

// Constant tables for DES
const int IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

const int FP[] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

const int E[] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

const int P[] = {
    16,  7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};

const int S[8][4][16] = {
    {
        {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
        { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
        { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
        {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
    },
    // Remaining 7 S-Boxes...
};

// Permutation function
uint64_t permute(uint64_t input, const int *table, int size) {
    uint64_t output = 0;
    for (int i = 0; i < size; ++i) {
        if (input & (1ULL << (64 - table[i]))) {
            output |= (1ULL << (size - i - 1));
        }
    }
    return output;
}

// Initial Permutation (IP)
uint64_t initial_permutation(uint64_t block) {
    return permute(block, IP, 64);
}

// Final Permutation (FP)
uint64_t final_permutation(uint64_t block) {
    return permute(block, FP, 64);
}

// Splitting the block into two halves
void split_block(uint64_t block, uint32_t &left, uint32_t &right) {
    left = (block >> 32) & 0xFFFFFFFF;
    right = block & 0xFFFFFFFF;
}

// Expansion function (E)
uint64_t expand(uint32_t half_block) {
    return permute(half_block, E, 48);
}

// XOR function
uint64_t xor_blocks(uint64_t a, uint64_t b) {
    return a ^ b;
}

// S-Box substitution
uint32_t sbox_substitution(uint64_t input) {
    uint32_t output = 0;
    for (int i = 0; i < 8; ++i) {
        int row = ((input >> (42 - i * 6)) & 0x20) | ((input >> (42 - i * 6)) & 0x1);
        int col = (input >> (43 - i * 6)) & 0xF;
        output |= S[i][row][col] << (28 - i * 4);
    }
    return output;
}

// Feistel function
uint32_t feistel(uint32_t right, uint64_t round_key) {
    uint64_t expanded = expand(right);
    uint64_t xored = xor_blocks(expanded, round_key);
    uint32_t substituted = sbox_substitution(xored);
    return permute(substituted, P, 32);
}

int main() {
    // Test with a 64-bit block
    uint64_t block = 0x0123456789ABCDEF;
    uint64_t key = 0x133457799BBCDFF1;

    cout << "Original Block: " << hex << block << endl;

    uint64_t ip_block = initial_permutation(block);
    cout << "After Initial Permutation: " << hex << ip_block << endl;

    uint32_t left, right;
    split_block(ip_block, left, right);
    cout << "Left Half: " << hex << left << ", Right Half: " << right << endl;

    uint64_t expanded = expand(right);
    cout << "Expanded Right Half: " << hex << expanded << endl;

    return 0;
}
