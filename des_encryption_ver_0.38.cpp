#include <iostream>
#include <random>
#include <cstdint>
#include <fstream>
#include <bitset>
#include <string>
#include <iomanip>
#include <sstream>


using namespace std;

// TABLICE PERMUTACJI 
// Permutacja początkowa  ( Initial permutation )
const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// Permutacja końcowa  ( Final permutation )
const int FP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// Rozszerzenie ( Expansion Table - E )
const int E[48] = {
    32, 1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9,  10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

// Permutacja P
const int P[32] = {
    16, 7,  20, 21,
    29, 12, 28, 17,
    1,  15, 23, 26,
    5,  18, 31, 10,
    2,  8,  24, 14,
    32, 27, 3,  9,
    19, 13, 30, 6,
    22, 11, 4,  25
};

// Tablice S-Box (Substitution Boxes)
const int S[8][4][16] = {
    // S1
    {
        {14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7},
        {0,  15, 7,  4,  14, 2,  13, 1,  10, 6,  12, 11, 9,  5,  3,  8},
        {4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0},
        {15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0,  6,  13}
    },
    // S2
    {
        {15, 1,  8,  14, 6,  11, 3,  4,  9,  7,  2,  13, 12, 0,  5,  10},
        {3,  13, 4,  7,  15, 2,  8,  14, 12, 0,  1,  10, 6,  9,  11, 5},
        {0,  14, 7,  11, 10, 4,  13, 1,  5,  8,  12, 6,  9,  3,  2,  15},
        {13, 8,  10, 1,  3,  15, 4,  2,  11, 6,  7,  12, 0,  5,  14, 9}
    },
    // S3
    {
        {10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8},
        {13, 7,  0,  9,  3,  4,  6,  10, 2,  8,  5,  14, 12, 11, 15, 1},
        {13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7},
        {1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5,  2,  12}
    },
    // S4
    {
        {7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15},
        {13, 8,  11, 5,  6,  15, 0,  3,  4,  7,  2,  12, 1,  10, 14, 9},
        {10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4},
        {3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7,  2,  14}
    },
    // S5
    {
        {2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9},
        {14, 11, 2,  12, 4,  7,  13, 1,  5,  0,  15, 10, 3,  9,  8,  6},
        {4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14},
        {11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4,  5,  3}
    },
    // S6
    {
        {12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11},
        {10, 15, 4,  2,  7,  12, 9,  5,  6,  1,  13, 14, 0,  11, 3,  8},
        {9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6},
        {4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0,  8,  13}
    },
    // S7
    {
        {4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1},
        {13, 0,  11, 7,  4,  9,  1,  10, 14, 3,  5,  12, 2,  15, 8,  6},
        {1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2},
        {6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2,  3,  12}
    },
    // S8
    {
        {13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7},
        {1,  15, 13, 8,  10, 3,  7,  4,  12, 5,  6,  11, 0,  14, 9,  2},
        {7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8},
        {2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5,  6,  11}
    }
};

// Permutacja klucza (PC1)
const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1,  58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27,
    19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29,
    21, 13, 5,  28, 20, 12, 4
};

// Permutacja klucza (PC2)
const int PC2[48] = {
    14, 17, 11, 24, 1,  5,
    3,  28, 15, 6,  21, 10,
    23, 19, 12, 4,  26, 8,
    16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Przesunięcie klucza (Key Shifts)
const int SHIFT_SCHEDULE[16] = {
    1, 1, 2, 2,
    2, 2, 2, 2,
    1, 2, 2, 2,
    2, 2, 2, 1
};


string generate_64bit_key() {
    // Używamy generatora liczb losowych
    random_device rd;
    mt19937_64 gen(rd());  // Generator liczb losowych 64-bitowy
    uniform_int_distribution<unsigned long long> dis(0, 0xFFFFFFFFFFFFFFFF);  // Zakres 64 bitów

    // Generujemy losowy 64-bitowy klucz
    unsigned long long key = dis(gen);

    // Konwertujemy na format hex (16 znaków)
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << key;
    return ss.str();
}

uint64_t text_to_hex(const std::string &text, size_t block_index) {
    uint64_t block = 0;
    size_t index = block_index * 8;

    // Sprawdzamy, czy mamy wystarczająco znaków
    for (size_t i = 0; i < 8; ++i) {
        if (index + i < text.size()) {
            // Znak na pozycji 'index + i', rzutujemy na 8-bitową liczbę
            // i przesuwamy odpowiednio do 64-bitowego bloku
            block |= static_cast<uint64_t>(text[index + i]) << (56 - 8 * i);
        }
    }
    return block;
}

void print_hex(uint64_t block) {
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << block;
    cout << ss.str() << endl;
}


// Funkcja wykonująca permutację IP (Initial permutation)
uint64_t apply_initial_permutation(uint64_t block) {
    uint64_t permuted_block = 0;
    for (int i = 0; i < 64; ++i) {
        int bit = (block >> (64 - IP[i])) & 1; // & 1 operacja AND i przesunięcie bitowe w prawo
        permuted_block |= (bit << (63 - i));  // |= bitowe OR i przesunięcie bitów w lewo
    }
    return permuted_block;
}

int main() {
    string input; // zmienna do trzymania wejścia z klawiatury
    cout << "Enter plain text: " << endl; 
    getline(cin, input);

    string key = generate_64bit_key();
    cout << "Generated 64-bit key: " << key << endl;
    // Save to file fo decrypt on remote host
    ofstream file;
    file.open("generated_key.txt");
    file << key;
    file.close();

    cout << "Entry text: " << input << endl;
    cout << "Find blocks:" << endl;

    for (size_t i = 0; i < (input.size() + 7) / 8; ++i) {
        uint64_t block = text_to_hex(input, i);
        cout << "Block " << i << ": ";
        print_hex(block);
        cout << "Block before and after initial permutation" << endl;
        cout << bitset<64>(block) << endl; 

        uint64_t perm = apply_initial_permutation(block);
        cout << bitset<64>(perm) << endl;
        cout << "---------------------------------------" << endl;

    }

// TEST

    //uint64_t block = 0x416c61206d61206b;
    //uint64_t permuted_block = apply_initial_permutation(block);

    //cout << "Blok wejściowy: " << bitset<64>(block) << endl;
    //cout << "Blok po Initial Permutation: " << bitset<64>(permuted_block) << endl;

    return 0;
}
