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

// const int E[48] = {
//     32,  1,  2,  3,  4,  5,  
//      4,  5,  6,  7,  8,  9,  
//      8,  9, 10, 11, 12, 13, 
//     12, 13, 14, 15, 16, 17, 
//     16, 17, 18, 19, 20, 21, 
//     20, 21, 22, 23, 24, 25, 
//     24, 25, 26, 27, 28, 29, 
//     28, 29, 30, 31, 32,  1
// };

// uint64_t expand(uint32_t R) {
//     uint64_t expanded_R = 0;

//     for (int i = 0; i < 48; ++i) {
//         int bit_position = E[i] - 1;  // Indeksowanie od 1 w tabeli
//         uint64_t bit = (R >> bit_position) & 1;  // Pobranie odpowiedniego bitu z R
//         expanded_R |= (bit << (47 - i));  // Ustawienie bitu w odpowiedniej pozycji
//     }

//     return expanded_R;
// }

// int main() {
//     uint32_t R = 0b11001100110011001100110011001100; // Przykładowy blok
//     uint64_t expanded_R = expand(R);

//     cout << "R w bitach: " << bitset<32>(R) << endl;
//     cout << "Rozszerzone R: " << bitset<48>(expanded_R) << endl;

//     return 0;
// }

// // Tabela rozszerzenia E dla DES
// const vector<int> E = {
//     32, 1, 2, 3, 4, 5,
//     4, 5, 6, 7, 8, 9,
//     8, 9, 10, 11, 12, 13,
//     12, 13, 14, 15, 16, 17,
//     16, 17, 18, 19, 20, 21,
//     20, 21, 22, 23, 24, 25,
//     24, 25, 26, 27, 28, 29,
//     28, 29, 30, 31, 32, 1
// };

// // Funkcja rozszerzenia
// uint64_t expand(uint32_t R) {
//     uint64_t expanded_R = 0;
//     for (int i = 0; i < 48; ++i) {
//         int bit_position = E[i] - 1; // Indeksowanie od 1 w tabeli
//         uint64_t bit = (R >> bit_position) & 1; // Pobranie odpowiedniego bitu z R
//         expanded_R |= (bit << (47 - i)); // Ustawienie bitu w odpowiedniej pozycji
//     }
//     return expanded_R;
// }

// // Funkcja do wyświetlania szczegółowego porównania
// void verify_expansion(uint32_t R, uint64_t expected_expanded_R) {
//     uint64_t generated_expansion = expand(R);

//     cout << "R w bitach: " << bitset<32>(R) << endl;
//     cout << "Rozszerzone R (wygenerowane): " << bitset<48>(generated_expansion) << endl;
//     cout << "Rozszerzone R (podane):      " << bitset<48>(expected_expanded_R) << endl;
//     cout << "Czy wygenerowane rozszerzenie zgadza się z podanym? "
//          << (generated_expansion == expected_expanded_R ? "TAK" : "NIE") << endl;

//     // Szczegółowe porównanie bit po bicie
//     cout << "\nSzczegółowe porównanie:" << endl;
//     for (int i = 0; i < 48; ++i) {
//         uint64_t bit_generated = (generated_expansion >> (47 - i)) & 1;
//         uint64_t bit_given = (expected_expanded_R >> (47 - i)) & 1;
//         cout << "Bit " << (i + 1) << ": wygenerowany = " << bit_generated
//              << ", podany = " << bit_given
//              << ", " << (bit_generated == bit_given ? "OK" : "BŁĄD") << endl;
//     }
// }

// int main() {
//     // Dane wejściowe
//     uint32_t R = 0b11001100101010101111000011110000; // 32-bitowy ciąg binarny
//     uint64_t expanded_R = 0b011000110010101011111000011110000111100011001100; // 48-bitowy ciąg binarny

//     // Weryfikacja
//     verify_expansion(R, expanded_R);

//     return 0;
// }

// Tabela rozszerzenia E dla DES
const vector<int> E = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

// Funkcja rozszerzenia
uint64_t expand(uint32_t R) {
    uint64_t expanded_R = 0;
    for (int i = 0; i < 48; ++i) {
        int bit_position = E[i] - 1; // Indeksowanie od 1 w tabeli
        uint64_t bit = (R >> bit_position) & 1; // Pobranie odpowiedniego bitu z R
        expanded_R |= (bit << (47 - i)); // Ustawienie bitu w odpowiedniej pozycji
    }
    return expanded_R;
}

// Test funkcji rozszerzenia
void test_expand() {
    // Przykładowe dane wejściowe
    uint32_t R = 0b00000000111111100110001100001010;

    // Oczekiwany wynik na podstawie tablicy E
    uint64_t expected_expansion = 0;
    for (int i = 0; i < 48; ++i) {
        int bit_position = E[i] - 1;
        uint64_t bit = (R >> bit_position) & 1;
        expected_expansion |= (bit << (47 - i));
    }

    // Wygenerowany wynik przez funkcję expand
    uint64_t generated_expansion = expand(R);

    // Sprawdzenie poprawności
    cout << "R w bitach: " << bitset<32>(R) << endl;
    cout << "Oczekiwane rozszerzenie: " << bitset<48>(expected_expansion) << endl;
    cout << "Wynik funkcji expand:  " << bitset<48>(generated_expansion) << endl;

    assert(expected_expansion == generated_expansion && "Funkcja expand zwraca nieprawidłowy wynik!");

    cout << "Test przeszedł pomyślnie!" << endl;
}

int main() {
    test_expand();
    return 0;
}
