#include <cstdint>
#include <array>
#include <cstdio>

// Permutacje i tablice dla DES
const std::array<int, 64> IP = { /* tablica permutacji początkowej IP */ };
const std::array<int, 64> FP = { /* tablica permutacji końcowej FP */ };
const std::array<int, 56> PC1 = { /* permutacja klucza początkowego PC1 */ };
const std::array<int, 48> PC2 = { /* permutacja klucza dla generowania subkluczy PC2 */ };

// S-boxy w DES (8 S-boxów, każdy 6-bitowy na 4-bitowy)
const std::array<std::array<uint8_t, 64>, 8> S_BOXES = {{
    { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 4, 1, 14, 8, 13, 11, 5, 9, 0, 15, 10, 3, 7, 12, 2, 13, 15, 10, 3, 8, 4, 2, 14, 12, 5, 6, 1, 7 },
    { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 9, 5, 11, 6, 14, 7, 13, 5, 0, 15, 10, 3, 12, 9, 1, 2, 8, 15, 4, 11, 7, 13, 14, 12, 3, 10, 9, 6, 1, 5, 0, 11 },
    { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 8, 2, 4, 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 15, 13, 4, 12, 9, 8, 15, 3, 0, 5, 14, 7, 11, 10, 12, 6, 1, 2, 11, 5, 8, 15, 3, 10, 0, 7, 9, 14, 4, 1, 13 },
    { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 15, 12, 4, 9, 5, 3, 2, 12, 10, 15, 14, 8, 13, 7, 11, 14, 4, 1, 13, 10, 15, 9, 0, 5, 8, 7, 4, 15, 3, 2, 14, 12, 13, 1, 6, 11, 9, 0, 8, 3, 6, 7, 5, 12, 13 },
    { 1, 15, 14, 8, 13, 10, 0, 7, 11, 5, 9, 12, 6, 3, 4, 2, 13, 0, 11, 7, 5, 10, 3, 14, 9, 8, 6, 15, 12, 2, 4, 1, 10, 15, 14, 4, 2, 8, 3, 5, 0, 6, 7, 9, 13, 1, 12, 11, 9, 14, 0, 3, 5, 13, 6, 11, 8 },
    { 4, 11, 2, 14, 15, 0, 8, 7, 9, 3, 5, 13, 10, 12, 6, 1, 15, 9, 7, 11, 8, 3, 2, 13, 5, 10, 0, 4, 6, 14, 12, 1, 3, 6, 11, 7, 13, 5, 0, 15, 12, 2, 8, 9, 4, 14, 1, 10, 6, 7, 3, 8, 0, 12, 14, 9, 5 },
    { 15, 5, 1, 4, 11, 10, 7, 9, 2, 8, 0, 6, 3, 12, 13, 14, 0, 4, 7, 9, 15, 2, 5, 6, 8, 10, 13, 14, 1, 3, 12, 11, 4, 0, 15, 3, 1, 10, 2, 7, 9, 13, 6, 12, 8, 5, 11, 14, 0, 10, 12, 15, 9, 13, 7, 11 },
    { 15, 7, 8, 1, 12, 14, 2, 4, 10, 0, 13, 6, 3, 11, 9, 5, 2, 13, 7, 9, 1, 3, 4, 6, 8, 5, 10, 14, 12, 0, 15, 11, 10, 15, 0, 13, 6, 3, 12, 9, 4, 2, 8, 5, 7, 1, 14, 11, 0, 12, 15, 2, 10, 3, 13, 9 }
}};

// Funkcja permutacji (implementacja uproszczona)
uint64_t permute(uint64_t block, const std::array<int, 64>& perm_table, size_t size) {
    uint64_t result = 0;
    for (size_t i = 0; i < size; ++i) {
        if (block & (1ULL << (size - perm_table[i]))) {
            result |= (1ULL << (size - i - 1));
        }
    }
    return result;
}

// Funkcja rozszerzenia (dla DES - E)
uint32_t expand(uint32_t half_block) {
    // Zastosowanie permutacji rozszerzenia E na połowę bloku (32 bity -> 48 bitów)
    uint32_t result = 0;
    for (int i = 0; i < 48; ++i) {
        result |= ((half_block >> (31 - i)) & 1) << (47 - i);
    }
    return result;
}

// Funkcja obróbki S-boxów
uint32_t s_box_substitution(uint32_t input) {
    uint32_t output = 0;
    for (int i = 0; i < 8; ++i) {
        // Wydzielamy 6-bitowy segment wejściowy dla każdego S-boxa
        uint8_t s_box_input = (input >> (6 * (7 - i))) & 0x3F;
        uint8_t row = ((s_box_input >> 5) & 0x02) | (s_box_input & 0x01);  // Wiersz (bity 0 i 5)
        uint8_t col = (s_box_input >> 1) & 0x0F;  // Kolumna (bity 1-4)
        uint8_t s_box_output = S_BOXES[i][row * 16 + col];  // Zastosowanie S-boxa
        output |= (s_box_output << (4 * (7 - i)));  // Przeniesienie do wyniku
    }
    return output;
}

// Funkcja Feistel
uint32_t feistel(uint32_t right_half, uint64_t subkey) {
    uint32_t expanded = expand(right_half);
    uint32_t xor_result = expanded ^ subkey;  // XOR z subkluczem
    uint32_t sbox_result = s_box_substitution(xor_result);  // Przetwarzanie przez S-boxy
    return sbox_result;
}

// Funkcja generowania subkluczy z głównego klucza
std::array<uint64_t, 16> generate_subkeys(uint64_t key) {
    std::array<uint64_t, 16> subkeys;
    
    // Permutacja klucza początkowego (PC1)
    uint64_t permuted_key = permute(key, PC1, 56);
    
    // Podział na dwie połowy i generowanie subkluczy
    for (int round = 0; round < 16; ++round) {
        // Wykonaj przesunięcia i permutacje, aby wygenerować subklucz dla danej rundy (PC2)
        subkeys[round] = permute(permuted_key, PC2, 48);  // Przykładowa permutacja
        // Wykonaj przesunięcia połówek
    }
    
    return subkeys;
}

// 16 rund szyfrowania DES
uint64_t des_encrypt(uint64_t block, uint64_t key) {
    uint64_t ip_block = permute(block, IP, 64);  // Permutacja początkowa
    uint32_t left = (ip_block >> 32) & 0xFFFFFFFF;
    uint32_t right = ip_block & 0xFFFFFFFF;

    // Generowanie subkluczy
    auto subkeys = generate_subkeys(key);
    
    for (int round = 0; round < 16; ++round) {
        uint32_t new_right = left ^ feistel(right, subkeys[round]);  // XOR z lewą połową
        left = right;  // Przesunięcie
        right = new_right;  // Nowa prawa połowa
    }
    
    // Złożenie bloku i permutacja końcowa
    uint64_t merged_block = ((uint64_t)right << 32) | left;
    uint64_t fp_block = permute(merged_block, FP, 64);
    return fp_block;
}

// 16 rund deszyfrowania DES (analogicznie)
uint64_t des_decrypt(uint64_t block, uint64_t key) {
    uint64_t ip_block = permute(block, IP, 64);  // Permutacja początkowa
    uint32_t left = (ip_block >> 32) & 0xFFFFFFFF;
    uint32_t right = ip_block & 0xFFFFFFFF;

    // Generowanie subkluczy
    auto subkeys = generate_subkeys(key);
    
    // Deszyfrowanie (odwrócone rundy)
    for (int round = 15; round >= 0; --round) {
        uint32_t new_right = left ^ feistel(right, subkeys[round]);  // XOR z lewą połową
        left = right;  // Przesunięcie
        right = new_right;  // Nowa prawa połowa
    }
    
    // Złożenie bloku i permutacja końcowa
    uint64_t merged_block = ((uint64_t)right << 32) | left;
    uint64_t fp_block = permute(merged_block, FP, 64);
    return fp_block;
}

int main() {
    uint64_t key = 0x133457799BBCDFF1;  // Przykładowy klucz
    uint64_t block = 0x0123456789ABCDEF;  // Przykładowy blok do zaszyfrowania

    uint64_t encrypted = des_encrypt(block, key);
    uint64_t decrypted = des_decrypt(encrypted, key);

    printf("Encrypted: %llx\n", encrypted);
    printf("Decrypted: %llx\n", decrypted);

    return 0;
}
