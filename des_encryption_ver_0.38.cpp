/*
https://billstclair.com/grabbe/des.htm
DES Algorithm implementation in C++
Author: Łukasz Świeżak
 */

#include <iostream>
#include <random>
#include <cstdint>
#include <fstream>
#include <bitset>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cassert>
#include <array>

using namespace std;

// TABLICE PERMUTACJI 
// Permutacja początkowa  ( Initial permutation )
const array<int, 64> IP = {
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
const array<int, 64> FP = {
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
const array<int, 48> E = {
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
const array<int, 32> P = {
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
const array<int, 56> PC1 = {
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
const array<int, 48> PC2 = {
    14, 17, 11, 24, 1,  5,
    3,  28, 15, 6,  21, 10,
    23, 19, 12, 4,  26, 8,
    16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Przesunięcie w lewo (liczba bitów) dla każdej rundy
const int shiftTable[16] = {1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1};

// Funkcja do konwersji tekstu na uint64_t
vector<uint64_t> stringToUint64(const string& input) {
    vector<uint64_t> result;
    size_t len = input.length();
    
    for (size_t i = 0; i < len; i += 8) {
        uint64_t chunk = 0;
        for (size_t j = 0; j < 8 && (i + j) < len; ++j) {
            chunk |= (static_cast<uint64_t>(input[i + j]) << (56 - 8 * j));
        }
        result.push_back(chunk);
    }
    return result;
}

string generate_64bit_key() {
    // Używamy generatora liczb losowych
    random_device rd;
    mt19937_64 gen(rd());  // Generator liczb losowych 64-bitowy
    uniform_int_distribution<unsigned long long> dis(0, 0xFFFFFFFFFFFFFFFF);  // Zakres 64 bitów

    // Generujemy losowy 64-bitowy klucz
    unsigned long long masterkey = dis(gen);

    // Konwertujemy na format hex (16 znaków)
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << masterkey;
    return ss.str();
}

uint64_t text_to_hex(const string &text, size_t block_index) {
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

// Funkcja permutacji ogólnej
uint64_t initial_permute(uint64_t input, const int *permutation_table, size_t output_size) {
    uint64_t output = 0;
    //cout << "Input: " << bitset<64>(input) << " (" << hex << uppercase << input << ")" << endl;
    for (size_t i = 0; i < output_size; ++i) {
        size_t bit_position = permutation_table[i] - 1;
        uint64_t bit = (input >> (63 - bit_position)) & 1;
        output |= bit << (output_size - 1 - i);
        // cout << "Bit " << i << ": from position " << bit_position
        //           << " -> " << bit << ", Output: "
        //           << bitset<64>(output) << " (0x" << hex << uppercase << output << ")" << endl;
    }
    //cout << "Permuted Output: " << bitset<64>(output)
    //          << " (0x" << hex << uppercase << output << ")" << endl;
    return output;
}

// Funkcja pomocnicza do konwersji 64-bitowego klucza do bitów
bitset<64> string_to_bitset(const string& str) {
    bitset<64> result;
    for (size_t i = 0; i < str.size(); ++i) {
        result[i] = (str[i] == '1');
    }
    return result;
}

bitset<64> stringToBitset64(const string& input) {
    bitset<64> result;
    // Konwertowanie każdego znaku do odpowiedniego segmentu bitów
    for (size_t i = 0; i < input.length(); ++i) {
        for (size_t j = 0; j < 8; ++j) {
            result[i * 8 + j] = (input[i] >> (7 - j)) & 1;
        }
    }
    return result;
}

// Funkcja dzieląca 64-bitowy blok na lewą (L) i prawą (R) część (po 32 bity każda)
void splitBlock(const bitset<64>& input, bitset<32>& left, bitset<32>& right) {
    for (int i = 0; i < 32; ++i) {
        left[i] = input[63 - i];  // Lewa część: pierwsze 32 bity
        right[i] = input[31 - i]; // Prawa część: kolejne 32 bity
    }
}

// // Funkcja rozszerzenia (E) 32 bity -> 48 bitów
// bitset<48> expand(const bitset<32>& R) {
//     bitset<48> expandedR;
//     for (int i = 0; i < 48; ++i) {
//         expandedR[i] = R[E[i] - 1];  // Indeksowanie od 1 w permutacji
//     }
//     return expandedR;
// }

template <size_t N, size_t M>
bitset<M> apply_permutation(const bitset<N>& input, const array<int, M>& permutation_table) {
    bitset<M> output;
    for (size_t i = 0; i < M; ++i) {
        output[M - 1 - i] = input[N - permutation_table[i]];
    }
    return output;
}

// Funkcja pomocnicza do rotacji w lewo
bitset<28> rotate_left(bitset<28>& part, int n) {
    return (part << n) | (part >> (28 - n));
}

// Funkcja generująca klucze rundowe
vector<bitset<48>> generate_round_keys(const bitset<64>& key) {
    // Klucze rundowe
    vector<bitset<48>> round_keys;
    
    // Krok 1: Permutacja początkowa (PC-1)
    bitset<56> permuted_key = apply_permutation(key, PC1);
    //cout << permuted_key << endl; 

    // Podziel klucz na dwie części (28 bitów każda)
    bitset<28> left = permuted_key.to_ulong() >> 28;  // Pierwsze 28 bitów
    bitset<28> right = permuted_key.to_ulong() & 0x0FFFFFFF;  // Pozostałe 28 bitów

    //cout << "Jest ok, miele dalej >>>> " << endl; 

    // Krok 2: Generowanie kluczy rundowych
    for (int round = 0; round < 16; ++round) {
        // Przesunięcie obu części
        left = (left << shiftTable[round]) | (left >> (28 - shiftTable[round]));
        right = (right << shiftTable[round]) | (right >> (28 - shiftTable[round]));

        // Połączenie obu części w jeden bitset<56>
        bitset<56> combined_key;
        for (int i = 0; i < 28; ++i) {
            combined_key[i] = left[i];  // Dodaj lewą część
            combined_key[28 + i] = right[i];  // Dodaj prawą część
        }

        // Zastosuj permutację końcową (PC-2) do połączonego klucza
        round_keys.push_back(apply_permutation(combined_key, PC2));
    }

    return round_keys;
}

// // Funkcja rundy Feistela
// pair<uint32_t, uint32_t> feistel_round(uint32_t L, uint32_t R, uint64_t round_key) {
//     cout << "Przed rundą Feistela: L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;
    
//     // Ekspansja Prawa część (R) na 48 bitów
//     uint64_t expanded_R = expand(R);
//     cout << "Rozszerzone R: " << bitset<48>(expanded_R) << endl;

//     // XOR z kluczem rundy
//     uint64_t xor_result = expanded_R ^ round_key;
//     cout << "R po XOR z kluczem rundy: " << bitset<48>(xor_result) << endl;

//     // S-Boxy (brak pełnej implementacji S-Boxów — wynik bezpośrednio kopiowany)
//     // Zakładamy uproszczony wynik (pierwsze 32 bity XOR wyników)
//     uint32_t sbox_result = static_cast<uint32_t>((xor_result >> 16) ^ xor_result); // Placeholder
//     cout << "Wynik S-Boxów: " << bitset<32>(sbox_result) << endl;

//     // Permutacja P
//     uint32_t pbox_result = permute_P(sbox_result);
//     cout << "Wynik permutacji P: " << bitset<32>(pbox_result) << endl;

//     // XOR z lewą częścią
//     uint32_t new_R = L ^ pbox_result;

//     // Zwracamy nowe części
//     return {R, new_R};
// }

int main() {
    string input = "Your lips are smoother than vaseline" ; // zmienna do trzymania wejścia z klawiatury
    //cout << "Enter plain text: " << endl; 
    //getline(cin, input); // wczytaj wejście

    string masterkey = generate_64bit_key();
    cout << "Generated 64-bit key: " << masterkey << endl;
    cout << endl;
    // Save to file fo decrypt on remote host
    ofstream file;
    file.open("generated_key.txt");
    file << masterkey;
    file.close();

    vector<uint64_t> input_in_uint64 = stringToUint64(input);

    // Przeprowadzenie IP i podział na L i R
    for (auto &block : input_in_uint64) {
        cout << "Original block: " << hex << block << endl;
        
        // Przeprowadź permutację początkową (IP)
        uint64_t permuted_block = initial_permute(block, IP.data(), 64);
        cout << "After Initial Permutation: " << hex << permuted_block << endl;
    }

    bitset<64> masterkeyBitset = stringToBitset64(masterkey);

    // Dla testów
    //bitset<64> key("133457799BBCDFF1");

    vector<bitset<48>> round_keys = generate_round_keys(masterkeyBitset);
    cout << "Master key in bits: " << masterkeyBitset << endl;

    // Wyświetlanie kluczy rundowych
    for (int i = 0; i < round_keys.size(); ++i) {
        cout << "Round " << i + 1 << " key: " << round_keys[i] << endl;
    }

    return 0;
}

// Your lips are smoother than vaseline