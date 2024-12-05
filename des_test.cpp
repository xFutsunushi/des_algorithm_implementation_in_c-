#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <iomanip>
#include <cstdint>

// Do zrobienia: 
// 1. Dynamiczna wartość klucza z main, z zapisywaniem do pliku aby mieć później do odszyfrowania (nowa funkcja)
// Nowa funkcja dla 16 rund kluczy feistela 


using namespace std;

// Stałe tablice dla DES

// IP - Tablica permutacji początkowej
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

// FP - tablica permutacji końcowej
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

// const std::array<int, 56> PC1 = {
//     57, 49, 41, 33, 25, 17, 9,   // pierwsza grupa
//     1, 58, 50, 42, 34, 26, 18,
//     10, 2, 59, 51, 43, 35, 27,
//     19, 11, 3, 60, 52, 44, 36,
//     28, 20, 12, 4, 61, 53, 45,
//     37, 29, 21, 13, 5, 62, 54,
//     46, 38, 30, 22, 14, 6, 63,
//     55, 47, 39, 31, 23, 15, 7,
//     56, 48, 40, 32
// };

// const std::array<int, 48> PC2 = {
//     14, 17, 11, 24, 1, 5,   // pierwsza grupa
//     3, 28, 15, 6, 21, 10,
//     23, 19, 12, 4, 26, 8,
//     16, 7, 27, 20, 13, 2,
//     41, 52, 31, 37, 47, 55,
//     30, 40, 51, 45, 33, 48,
//     44, 49, 39, 56, 34, 53,
//     46, 42, 50, 36, 29, 32,
//     43, 38, 56, 34
// };


// // S-boxy w DES (8 S-boxów, każdy 6-bitowy na 4-bitowy)
// const std::array<std::array<uint8_t, 64>, 8> S_BOXES = {{
//     { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 4, 1, 14, 8, 13, 11, 5, 9, 0, 15, 10, 3, 7, 12, 2, 13, 15, 10, 3, 8, 4, 2, 14, 12, 5, 6, 1, 7 },
//     { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 9, 5, 11, 6, 14, 7, 13, 5, 0, 15, 10, 3, 12, 9, 1, 2, 8, 15, 4, 11, 7, 13, 14, 12, 3, 10, 9, 6, 1, 5, 0, 11 },
//     { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 8, 2, 4, 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 15, 13, 4, 12, 9, 8, 15, 3, 0, 5, 14, 7, 11, 10, 12, 6, 1, 2, 11, 5, 8, 15, 3, 10, 0, 7, 9, 14, 4, 1, 13 },
//     { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 15, 12, 4, 9, 5, 3, 2, 12, 10, 15, 14, 8, 13, 7, 11, 14, 4, 1, 13, 10, 15, 9, 0, 5, 8, 7, 4, 15, 3, 2, 14, 12, 13, 1, 6, 11, 9, 0, 8, 3, 6, 7, 5, 12, 13 },
//     { 1, 15, 14, 8, 13, 10, 0, 7, 11, 5, 9, 12, 6, 3, 4, 2, 13, 0, 11, 7, 5, 10, 3, 14, 9, 8, 6, 15, 12, 2, 4, 1, 10, 15, 14, 4, 2, 8, 3, 5, 0, 6, 7, 9, 13, 1, 12, 11, 9, 14, 0, 3, 5, 13, 6, 11, 8 },
//     { 4, 11, 2, 14, 15, 0, 8, 7, 9, 3, 5, 13, 10, 12, 6, 1, 15, 9, 7, 11, 8, 3, 2, 13, 5, 10, 0, 4, 6, 14, 12, 1, 3, 6, 11, 7, 13, 5, 0, 15, 12, 2, 8, 9, 4, 14, 1, 10, 6, 7, 3, 8, 0, 12, 14, 9, 5 },
//     { 15, 5, 1, 4, 11, 10, 7, 9, 2, 8, 0, 6, 3, 12, 13, 14, 0, 4, 7, 9, 15, 2, 5, 6, 8, 10, 13, 14, 1, 3, 12, 11, 4, 0, 15, 3, 1, 10, 2, 7, 9, 13, 6, 12, 8, 5, 11, 14, 0, 10, 12, 15, 9, 13, 7, 11 },
//     { 15, 7, 8, 1, 12, 14, 2, 4, 10, 0, 13, 6, 3, 11, 9, 5, 2, 13, 7, 9, 1, 3, 4, 6, 8, 5, 10, 14, 12, 0, 15, 11, 10, 15, 0, 13, 6, 3, 12, 9, 4, 2, 8, 5, 7, 1, 14, 11, 0, 12, 15, 2, 10, 3, 13, 9 }
// }};

// Funkcja permutacji
uint64_t permute(uint64_t input, const int *table, int size) {
    uint64_t output = 0;
    for (int i = 0; i < size; ++i) {
        if (input & (1ULL << (64 - table[i]))) { // 1ULL to unsigned long long 
            output |= (1ULL << (size - i - 1));
        }
    }
    return output;
}

// Funkcja do konwersji tekstu na 64-bitowy blok
uint64_t text_to_block(const string &text, size_t block_index) {
    uint64_t block = 0;
    for (size_t i = 0; i < 8; ++i) {
        if (block_index * 8 + i < text.size()) {
            block |= (static_cast<uint64_t>(text[block_index * 8 + i]) & 0xFF) << (56 - 8 * i);
        }
    }
    return block;
}

// Funkcja do konwersji bloku 64-bitowego na tekst
string block_to_text(uint64_t block) {
    string text;
    for (int i = 7; i >= 0; --i) {
        text += static_cast<char>((block >> (i * 8)) & 0xFF);
    }
    return text;
}

// DES szyfrowanie (dla uproszczenia, pojedynczy blok)
uint64_t des_encrypt(uint64_t block, uint64_t key) {
    uint64_t ip_block = permute(block, IP, 64);
    // Tu nastąpiłoby przeprowadzenie 16 rund (pomijam szczegóły dla uproszczenia).
    uint64_t fp_block = permute(ip_block, FP, 64);
    return fp_block;
}

// DES deszyfrowanie (dla uproszczenia, pojedynczy blok)
uint64_t des_decrypt(uint64_t block, uint64_t key) {
    uint64_t ip_block = permute(block, IP, 64);
    // Tu nastąpiłoby przeprowadzenie odwróconych 16 rund.
    uint64_t fp_block = permute(ip_block, FP, 64);
    return fp_block;
}

int main() {
    using namespace std;

    string input;
    uint64_t key = 0x133457788AACDEE1; // 64-bitowy klucz

    cout << "Podaj tekst do zaszyfrowania: ";
    getline(cin, input);

    // Wypełniamy ciąg znaków zerami jeśli nie równa się 8 bitom
    while (input.size() % 8 != 0) {
        input += '\0';
    }

    vector<uint64_t> encrypted_blocks;

    // Szyfrowanie blok po bloku
    for (size_t i = 0; i < input.size() / 8; ++i) {
        uint64_t block = text_to_block(input, i);
        uint64_t encrypted = des_encrypt(block, key);
        encrypted_blocks.push_back(encrypted);
    }

    cout << "Zaszyfrowane dane: ";
    for (auto block : encrypted_blocks) {
        cout << hex << setw(16) << setfill('0') << block << " ";
    }
    cout << endl;

    string decrypted_text;

    // Deszyfrowanie blok po bloku
    for (auto block : encrypted_blocks) {
        uint64_t decrypted = des_decrypt(block, key);
        decrypted_text += block_to_text(decrypted);
    }

    cout << "Odszyfrowane dane: " << decrypted_text << endl;

    return 0;
}
