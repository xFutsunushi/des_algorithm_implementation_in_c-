#include <iostream>
#include <bitset>
#include <string>

// Funkcja do wykonania operacji XOR na dwóch ciągach bitów
std::string xorOperation(const std::string& r, const std::string& roundKey) {
    std::string result;
    
    // Sprawdzenie, czy długości obu ciągów są takie same
    if (r.length() != roundKey.length()) {
        std::cerr << "Błąd: długości ciągów nie są zgodne!" << std::endl;
        return "";
    }

    // Wykonanie operacji XOR dla każdego bitu
    for (size_t i = 0; i < r.length(); ++i) {
        result += (r[i] == roundKey[i] ? '0' : '1');  // XOR
    }

    return result;
}

int main() {
    // Rozszerzona część R
    std::string r = "001010100001011000001100001111111110100000000000";
    
    // Klucz rundy
    std::string roundKey = "100011111001101000000100000010000000110110111010";
    
    // Wykonanie operacji XOR
    std::string result = xorOperation(r, roundKey);

    if (!result.empty()) {
        // Wyświetlenie wyniku operacji XOR
        std::cout << "Wynik XOR: " << result << std::endl;

        // Porównanie z oczekiwanym wynikiem
        std::string expected = "101001011000110000001000001101111110010110111010";
        if (result == expected) {
            std::cout << "Operacja XOR wykonana poprawnie!" << std::endl;
        } else {
            std::cout << "Wynik XOR jest niepoprawny." << std::endl;
        }
    }

    return 0;
}
