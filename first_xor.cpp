#include <iostream>
#include <bitset>

using namespace std;

// Funkcja wykonująca operację XOR na rozszerzonym bloku i kluczu rundy
uint64_t xor_with_round_key(uint64_t expanded_R, uint64_t round_key) {
    // Wykonaj XOR między rozszerzonym blokiem a kluczem rundy
    uint64_t xor_result = expanded_R ^ round_key;
    return xor_result;
}

int main() {
    // Przykładowe dane (48-bitowe)
    uint64_t expanded_R = 0x0000000000A2B3C4; // Przykładowy rozszerzony blok R
    uint64_t round_key = 0x0F1E2D3C4B5A6978; // Przykładowy klucz rundy (48-bitowy)

    // Wykonaj XOR
    uint64_t result = xor_with_round_key(expanded_R, round_key);

    // Wyświetl wynik XOR (w postaci binarnej)
    cout << "Expanded R: " << bitset<48>(expanded_R) << endl;
    cout << "Round Key: " << bitset<48>(round_key) << endl;
    cout << "XOR Result: " << bitset<48>(result) << endl;

    return 0;
}
