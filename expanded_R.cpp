#include <iostream>
#include <bitset>
#include <cstdint>
#include <vector>

using namespace std;

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

// Funkcja rozszerzająca R z 32 bitów do 48 bitów za pomocą tablicy rozszerzenia (E)
uint64_t expansion_function(uint32_t R) {
    uint64_t expanded_R = 0;

    // Przesuwamy i kopiujemy odpowiednie bity z 32-bitowego R, według tablicy E
    for (int i = 0; i < 48; ++i) {
        int bit_position = E[i] - 1; // Tablica E jest 1-indeksowana
        expanded_R |= ((R >> (31 - bit_position)) & 1) << (47 - i);  // Bit przesunięty na odpowiednią pozycję
    }

    return expanded_R;
}

int main() {
    uint32_t R = 0x12345678; // Przykładowy 32-bitowy blok danych
    cout << "Original R: " << bitset<32>(R) << endl;

    uint64_t expanded_R = expansion_function(R);

    cout << "Expanded R: " << bitset<48>(expanded_R) << endl;

    return 0;
}
