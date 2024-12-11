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

// Przykładowy S-box (S-box 0 w DES)
vector<vector<uint8_t>> S_BOX_0 = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
    {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
    {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
    {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
};

// Funkcja, która stosuje S-box
uint8_t apply_Sbox(uint8_t input, const vector<vector<uint8_t>>& sbox) {
    uint8_t row = (input >> 5) | (input & 1);  // Bity 5 i 0
    uint8_t col = (input >> 1) & 0x0F;         // Bity 4, 3, 2, 1

    return sbox[row][col];
}

// Funkcja do testowania całej tablicy S-box
void test_entire_Sbox(const vector<vector<uint8_t>>& sbox) {
    cout << "Testing the entire S-box..." << endl;

    // Iterujemy przez wszystkie 6-bitowe wartości (od 0x00 do 0x3F)
    for (uint8_t i = 0; i <= 0x3F; ++i) {
        uint8_t result = apply_Sbox(i, sbox);
        
        // Wypisujemy wynik dla każdej wartości wejściowej
        cout << "Input: " << bitset<6>(i) << " (" << hex << (int)i << ") -> "
             << "Output: " << bitset<4>(result) << " (" << hex << (int)result << ")" << endl;
    }
}

int main() {
    test_entire_Sbox(S_BOX_0);  // Testujemy całą tablicę S-box 0

    return 0;
}
