#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <iomanip>
#include <cstdint>

//using namespace std;

// Stałe tablice dla DES
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
uint64_t text_to_block(const std::string &text, size_t block_index) {
    uint64_t block = 0;
    for (size_t i = 0; i < 8; ++i) {
        if (block_index * 8 + i < text.size()) {
            block |= (static_cast<uint64_t>(text[block_index * 8 + i]) & 0xFF) << (56 - 8 * i);
        }
    }
    return block;
}

// Funkcja do konwersji bloku 64-bitowego na tekst
std::string block_to_text(uint64_t block) {
    std::string text;
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
    std::string input;
    uint64_t key = 0x133457799BBCDFF1; // 64-bitowy klucz

    std::cout << "Podaj tekst do zaszyfrowania: ";
    std::getline(std::cin, input);

    // Padding
    while (input.size() % 8 != 0) {
        input += '\0';
    }

    std::vector<uint64_t> encrypted_blocks;

    // Szyfrowanie blok po bloku
    for (size_t i = 0; i < input.size() / 8; ++i) {
        uint64_t block = text_to_block(input, i);
        uint64_t encrypted = des_encrypt(block, key);
        encrypted_blocks.push_back(encrypted);
    }

    std::cout << "Zaszyfrowane dane: ";
    for (auto block : encrypted_blocks) {
        std::cout << std::hex << std::setw(16) << std::setfill('0') << block << " ";
    }
    std::cout << std::endl;

    std::string decrypted_text;

    // Deszyfrowanie blok po bloku
    for (auto block : encrypted_blocks) {
        uint64_t decrypted = des_decrypt(block, key);
        decrypted_text += block_to_text(decrypted);
    }

    std::cout << "Odszyfrowane dane: " << decrypted_text << std::endl;

    return 0;
}
