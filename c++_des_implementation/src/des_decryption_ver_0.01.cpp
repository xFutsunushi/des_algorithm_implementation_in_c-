// Documentaions: https://billstclair.com/grabbe/des.htm
// DES Algorithm implementation in C++
// Author: Łukasz Świeżak

#include "permutation_tables.h"
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

// Funkcja konwertująca bitset<64> na tekst
string bitsetToText(const bitset<64>& block) {
    string result = "";
    for (int i = 0; i < 8; ++i) {
        // Pobieramy 8-bitową część
        bitset<8> byte(block.to_ullong() >> (i * 8));
        result = char(byte.to_ulong()) + result;
    }
    return result;
}

// Funkcja przekształcająca dane z formatu hex na bitset<64>
bitset<64> hexToBitset64(const string& hex_str) {
    bitset<64> result;
    stringstream ss;
    ss << hex << hex_str;
    ss >> result;
    return result;
}

string binaryToText(const string &binary) {
    if (binary.size() % 8 != 0) {
        throw invalid_argument("Binarka musi być wielokrotnością 8 bitów.");
    }

    string plaintext;
    for (size_t i = 0; i < binary.size(); i += 8) {
        bitset<8> byte(binary.substr(i, 8)); // Pobieramy 8-bitowy fragment
        plaintext += static_cast<char>(byte.to_ulong()); // Konwertujemy na znak
    }

    return plaintext;
}

string textToHex(const string& text, size_t blockSize = 4) {
    stringstream hexStream;
    size_t count = 0;

    for (char c : text) {
        hexStream << hex << uppercase << setw(2) << setfill('0') << static_cast<int>(static_cast<unsigned char>(c));
        count++;

        // Dodaj spację co "blockSize" znaków, aby poprawić czytelność
        if (blockSize > 0 && count % blockSize == 0) {
            hexStream << " ";
        }
    }

    return hexStream.str();
}

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

// Funkcja permutacji P
uint32_t permute_P(uint32_t sbox_result) {
    uint32_t pbox_result = 0;
    for (int i = 0; i < 32; ++i) {
        int bit_position = P[i] - 1;  // Indeksowanie od 1 w permutacji
        pbox_result |= ((sbox_result >> bit_position) & 1) << (31 - i);  // Ustawiamy bity zgodnie z permutacją
    }
    return pbox_result;
}

// Funkcja permutacji z debugowaniem
bitset<64> initial_permutation(const bitset<64>& input, const array<int, 64>& IP) {
    bitset<64> permuted;
    cout << "\nDebugowanie Initial Permutation (IP):" << endl;
    cout << "Wejściowy ciąg bitów: " << input << endl;

    bitset<64> output;
    for (size_t i = 0; i < 64; ++i) {
        output[63 - i] = input[64 - IP[i]]; // Mapowanie pozycji zgodnie z tablicą IP

    // Debug: wyświetl mapowanie dla każdego bitu
        cout << "Pozycja bitu w wejściu: " << (64 - IP[i])
            << " (bit: " << input[64 - IP[i]] << ") -> Pozycja w wyniku: " << (63 - i) << std::endl;
    }

    cout << "Wynikowy ciąg bitów: " << output << endl;
    return output;
}
// Funkcja pomocnicza do konwersji 64-bitowego klucza do bitów

// Funkcja konwertująca tekst na ciąg binarny
string text_to_binary(const string &text) {
    string binary_str = "";
    
    for (char c : text) {
        // Przekształcamy każdy znak w jego binarną reprezentację
        binary_str += bitset<8>(c).to_string();
    }
    
    return binary_str;
}

bitset<64> string_to_bitset(const string& str) {
    bitset<64> result;
    for (size_t i = 0; i < str.size(); ++i) {
        result[i] = (str[i] == '1');
    }
    return result;
}

bitset<64> stringToBitset64(const string& hex_str) {
    bitset<64> result;
    for (size_t i = 0; i < hex_str.size(); ++i) {
        result <<= 4;  // Przesuń wynik w lewo o 4 bity
        if (hex_str[i] >= '0' && hex_str[i] <= '9') {
            result |= (hex_str[i] - '0');
        } else if (hex_str[i] >= 'A' && hex_str[i] <= 'F') {
            result |= (hex_str[i] - 'A' + 10);
        } else if (hex_str[i] >= 'a' && hex_str[i] <= 'f') {
            result |= (hex_str[i] - 'a' + 10);
        }
    }
    return result;
}

// Funkcja do dzielenia ciągu binarnego na 64-bitowe bloki
vector<string> split_into_blocks(const string &input) {
    vector<string> blocks;
    size_t block_size = 64;  // Rozmiar bloku 64 bity
    size_t input_size = input.size() * 8;  // Rozmiar wejścia w bitach

    // Konwertujemy string na ciąg bitów
    string binary_input = "";
    for (char c : input) {
        binary_input += std::bitset<8>(c).to_string();
    }

    // Upewniamy się, że ciąg ma długość podzielną przez 64 bity
    if (binary_input.size() % block_size != 0) {
        size_t padding_size = block_size - (binary_input.size() % block_size);
        binary_input.append(padding_size, '0');  // Dodajemy padding
    }

    // Dzielimy ciąg bitów na 64-bitowe bloki
    for (size_t i = 0; i < binary_input.size(); i += block_size) {
        blocks.push_back(binary_input.substr(i, block_size));
    }

    return blocks;
}

uint64_t expand(uint32_t R) {
    uint64_t expanded_R = 0;

    // Iteracja po tabeli rozszerzenia
    for (int i = 0; i < 48; ++i) {
        int bit_position = E[i] - 1;  // Indeksowanie od 1 w tabeli (0-bazowe w C)
        uint64_t bit = (R >> (31 - bit_position)) & 1;  // Poprawne przesunięcie bitu z R

        // Debugowanie: Wypisanie bitu, który jest kopiowany
        //cout << "Bit " << i << ": " << bit << " (from R[" << (31 - bit_position) << "])" << std::endl;

        expanded_R |= (bit << (47 - i));  // Ustawienie bitu w odpowiedniej pozycji
    }

    // Debugowanie: Wypisanie całego rozszerzonego wyniku
    //cout << "Expanded R: " << std::bitset<48>(expanded_R) << std::endl;

    return expanded_R;
}

template <size_t N, size_t M>
bitset<M> apply_permutation(const bitset<N>& input, const array<int, M>& permutation_table) {
    bitset<M> output;
    for (size_t i = 0; i < M; ++i) {
        output[M - 1 - i] = input[N - permutation_table[i]];
    }
    return output;
}

// Funkcja do przesunięcia bitset<28> w lewo
bitset<28> rotate_left(const bitset<28>& input, int shift) {
    return (input << shift) | (input >> (28 - shift));
}

// Funkcja generująca klucze rundowe
vector<bitset<48>> generate_round_keys(const bitset<64>& key) {
    vector<bitset<48>> round_keys;

    // Krok 1: Permutacja początkowa (PC-1)
    bitset<56> permuted_key = apply_permutation(key, PC1);
    //cout << "TEST KLUCZA !!! 56 bit:    " << permuted_key << endl; // sprawdzone działa dobrze

    // Podziel klucz na dwie części (28 bitów każda)
    bitset<28> left, right;
    for (int i = 0; i < 28; ++i) {
        left[i] = permuted_key[i];
        right[i] = permuted_key[i + 28];
    }

    // cout << "Podzielenie na dwie czesci:   " <<  endl; // JEST POPRAWNIE - SPRAWDZONE
    // cout << "Right: " << right << endl;
    // cout << "Left: " << left << endl;

    // Krok 2: Generowanie kluczy rundowych
    for (int round = 0; round < 16; ++round) {
        // Przesunięcie obu części
        left = rotate_left(left, shiftTable[round]);
        right = rotate_left(right, shiftTable[round]);

        // Połączenie obu części w jeden klucz
        bitset<56> combined_key;
        for (int i = 0; i < 28; ++i) {
            combined_key[i] = left[i];
            combined_key[28 + i] = right[i];
        }

        //cout << "Combined key: " << combined_key << endl;
        // Permutacja końcowa (PC-2)
        //bitset<48> round_key = apply_permutation(combined_key, PC2);
        round_keys.push_back(apply_permutation(combined_key, PC2));
        //cout << "Round " << round + 1 << " key: " << round_key << endl;

    }

    return round_keys;
}

// Funkcja do zapisania kluczy rundowych do pliku
void save_round_keys(const vector<bitset<48>>& round_keys, const string& filename) {
    ofstream file(filename, ios::binary); // Otwieramy plik w trybie binarnym

    if (!file) {
        cerr << "Nie udało się otworzyć pliku!" << endl;
        return;
    }

    // Zapisujemy każdy klucz rundowy do pliku
    for (const auto& key : round_keys) {
        uint64_t key_value = 0;

        // Konwertujemy bitset<48> do uint64_t
        for (size_t i = 0; i < 48; ++i) {
            if (key[i]) {
                key_value |= (1ULL << (47 - i)); // Ustawiamy odpowiedni bit w 64-bitowej liczbie
            }
        }

        // Zapisujemy 64-bitową liczbę do pliku
        file.write(reinterpret_cast<const char*>(&key_value), sizeof(key_value));
    }

    file.close();
    cout << "Klucze rundowe zostały zapisane do pliku: " << filename << endl;
}

// Funkcja do wczytania kluczy rundowych z pliku
void load_round_keys(vector<bitset<48>>& round_keys, const string& filename) {
    ifstream file(filename, ios::binary); // Otwieramy plik w trybie binarnym

    if (!file) {
        cerr << "Nie udało się otworzyć pliku!" << endl;
        return;
    }

    uint64_t key_value;
    while (file.read(reinterpret_cast<char*>(&key_value), sizeof(key_value))) {
        bitset<48> key;
        for (size_t i = 0; i < 48; ++i) {
            key[i] = (key_value >> (47 - i)) & 1; // Kopiujemy bity z 64-bitowej liczby do bitset<48>
        }

        round_keys.push_back(key); // Dodajemy klucz rundowy do vectora
    }

    file.close();
    cout << "Klucze rundowe zostały załadowane z pliku: " << filename << endl;
}

// Funkcja obsługująca S-Boxy
bitset<32> apply_sboxes(bitset<48> xor_result) {
    bitset<32> sbox_result;

    for (int i = 0; i < 8; ++i) {
        // Pobierz odpowiednie 6 bitów (zaczynając od najbardziej znaczących)
        uint64_t chunk = (xor_result.to_ulong() >> (6 * (7 - i))) & 0x3F; // Zamiana na unsigned long

        // Wyciągnij bity rzędu (pierwszy i ostatni bit)
        uint32_t row = ((chunk & 0x20) >> 4) | (chunk & 0x01); // Najbardziej znaczący i najmniej znaczący bit
        // Wyciągnij bity kolumny (środkowe 4 bity)
        uint32_t col = (chunk >> 1) & 0x0F;

        // Znajdź wartość z odpowiedniego S-Boxa i zapisz w wyniku
        uint8_t sbox_value = SBOX[i][row][col];

        // Zapewniamy, żeby wynik z S-Boxa był zapisany w odpowiedniej pozycji w sbox_result
        sbox_result |= (bitset<32>(sbox_value) << (4 * (7 - i)));
    }

    return sbox_result;
}

// Funkcja rundy Feistela
pair<uint32_t, uint32_t> feistel_round(uint32_t L, uint32_t R, uint64_t round_key) {
    cout << "Przed rundą Feistela: L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;
    
    // Ekspansja Prawa część (R) na 48 bitów
    uint64_t expanded_R = expand(R);
    cout << "Rozszerzone R: " << bitset<48>(expanded_R) << endl;

    // XOR z kluczem rundy
    uint64_t xor_result = expanded_R ^ round_key;
    cout << "R po XOR z kluczem rundy: " << bitset<48>(xor_result) << endl;
    // Debugowanie: Wypisz oba operandy przed XOR
    cout << "expanded_R: " << bitset<48>(expanded_R) <<    endl;
    cout << "round_key: " << bitset<48>(round_key) << endl;

    // Wypisz wynik operacji XOR
    cout << "xor_result: " << bitset<48>(xor_result) <<    endl;

    uint32_t sbox_result = apply_sboxes(xor_result).to_ullong();

    // Wynik po S-Boxach
    cout << "Wynik po S-Boxach: " << bitset<32>(sbox_result) << endl;

    // Permutacja P
    uint32_t pbox_result = permute_P(sbox_result);
    cout << "Wynik permutacji P: " << bitset<32>(pbox_result) << endl;

    // XOR z lewą częścią
    uint32_t new_R = L ^ pbox_result; /// Sprawdzić czemu lewa 

    // Zwracamy nowe części
    return {R, new_R};
}

bitset<64> final_permutation(const bitset<64>& block, const array<int, 64>& perm_table) {
    bitset<64> permuted_block;
    
    for (int i = 0; i < 64; ++i) {
        permuted_block[63 - i] = block[64 - perm_table[i]];
    }
    
    return permuted_block;
}

void test_expand() {
    uint32_t test_R = 0b11110000101010101111000010101010;
    uint64_t expected_result = 0b011110100001010101010101011110100001010101010101;
    uint64_t actual_result = expand(test_R);
    
    cout << "Input R: " << bitset<32>(test_R) << endl;
    cout << "Expanded R: " << bitset<48>(actual_result) << endl;

    if (actual_result == expected_result) {
        cout << "Test PASSED!" << endl;
    } else {
        cout << "Test FAILED!" << endl;
        cout << "Expected: " << bitset<48>(expected_result) << endl;
    }
}

// Funkcja deszyfrowania bloku
bitset<64> decryptBlock(const bitset<64>& block, const vector<bitset<48>>& subkeys) {
    // Permutacja początkowa (IP)
    bitset<64> permutedBlock = initial_permutation(block, IP);

    bitset<32> left, right;
    for (size_t i = 0; i < 32; ++i) {
        left[i] = permutedBlock[i + 32];
        right[i] = permutedBlock[i];
    }

    // Rundy deszyfrowania z odwróconymi kluczami (deszyfrowanie to odwrócenie kolejności rund)
    for (int round = 15; round >= 0; --round) {
        // Przekazujemy prawą część, lewą część i klucz rundy
        auto [newLeft, newRight] = feistel_round(left.to_ullong(), right.to_ullong(), subkeys[round].to_ullong());

        // Ustawiamy nowe wartości
        left = newLeft;
        right = newRight;
    }

    // Po zakończeniu rund, łączymy lewą i prawą część w odpowiedniej kolejności
    bitset<64> preOutput;
    for (size_t i = 0; i < 32; ++i) {
        preOutput[i] = right[i];  // Prawa część staje się pierwszą połową
        preOutput[i + 32] = left[i];  // Lewa część staje się drugą połową
    }

    // Ostateczna permutacja (IP^-1)
    return final_permutation(preOutput, IP_inverse); // Zamień IP na IP^-1, jeśli to odpowiednia permutacja końcowa
}

// Funkcja wczytująca klucze z pliku i odwracająca ich kolejność
vector<bitset<48>> loadReversedSubkeys(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Nie można otworzyć pliku: " << filename << endl;
        exit(1);
    }

    vector<bitset<48>> subkeys(16);
    for (int i = 0; i < 16; ++i) {
        bitset<48> subkey;
        for (int j = 0; j < 48; ++j) {
            char bit;
            file.read(&bit, 1);
            subkey[47 - j] = bit - '0';
        }
        subkeys[15 - i] = subkey; // Odwrócenie kolejności
    }

    file.close();
    return subkeys;
}

// int main() {
//     string input = "Your lips are smoother than vaseline";
//     //string binary_input = text_to_binary(input);  // Twoja funkcja konwersji tekstu na binarny ciąg

//     string hex_masterkey = "0E329232EA6D0D73"; 
//     bitset<64> masterkeyBitset = stringToBitset64(hex_masterkey);
//     vector<bitset<48>> round_keys = generate_round_keys(masterkeyBitset);

//     cout << "Podany tekst hexadecymalnie: ";
//     cout << textToHex(input, 8)<<  endl;
//     cout << endl;

//     // Konwertujemy ciąg binarny na 64-bitowe bloki
//     vector<string> blocks = split_into_blocks(input);

//     // Wypisujemy oryginalne bloki przed inicjalną permutacją
//     cout << "Bloki przed inicjalną permutacją: " << endl;
//     for (const auto &block : blocks) {
//         cout << block << endl;
//     }

//     // Przetwarzamy każdy blok przez inicjalną permutację
//     for (const auto &block : blocks) {
//         // Tworzymy bitset 64-bitowy z bloku
//         bitset<64> block_bitset(block);
        
//         // Wykonanie inicjalnej permutacji
//         bitset<64> permuted_block = initial_permutation(block_bitset, IP);

//         // Wypisanie permutowanego bloku
//         cout << permuted_block << endl;
//     }

//     // Wyświetlanie kluczy rundowych -- sprawdzone, funkcja dziala
//     for (int i = 0; i < round_keys.size(); ++i) {
//         cout << "Round " << i + 1 << " key: " << round_keys[i] << endl;
//     }

//     // Iteracja po blokach, wykonywanie rund Feistela
//     for (int block_idx = 0; block_idx < blocks.size(); ++block_idx) {
//         string block = blocks[block_idx];
//         uint32_t L = bitset<32>(block.substr(0, 32)).to_ulong();
//         uint32_t R = bitset<32>(block.substr(32, 32)).to_ulong();

//         cout << "Przetwarzany blok " << block_idx + 1 << ": " << endl;
//         cout << "L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;

//     for (int i = 15; i >= 0; --i) {
//         uint64_t round_key = round_keys[i].to_ullong(); // Konwersja klucza na uint64_t
//         cout << "Runda " << (16 - i) << " z kluczem: " << bitset<48>(round_key) << endl;

//         // Wywołanie rundy Feistela
//         tie(L, R) = feistel_round(L, R, round_key); // Wykonanie rundy Feistela
//         cout << "Po rundzie " << (16 - i) << ": L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;
// }
//         // Zamiana miejsc po 16 rundach
//         swap(L, R);
//         uint32_t final_L = L;
//         uint32_t final_R = R;

//         // Przed finalną permutacją
//         bitset<64> final_block = final_permutation((static_cast<uint64_t>(final_L) << 32) | final_R, FP);
//         cout << "Blok po 16 rundach i finalnej permutacji: ";
        
//         // Wypisanie finalnego bloku w formacie szesnastkowym
//         cout << "Blok " << block_idx + 1 << ": 0x" << uppercase << hex << setw(16) << setfill('0') << final_block.to_ullong() << endl;
//     }

//     return 0;
// }

// int main() {
//     string input = "Your lips are smoother than vaseline";
//     //string binary_input = text_to_binary(input);  // Twoja funkcja konwersji tekstu na binarny ciąg

//     string hex_masterkey = "0E329232EA6D0D73"; 
//     bitset<64> masterkeyBitset = stringToBitset64(hex_masterkey);
//     vector<bitset<48>> round_keys = generate_round_keys(masterkeyBitset);

//     cout << "Podany tekst hexadecymalnie: ";
//     cout << textToHex(input, 8) << endl;
//     cout << endl;

//     // Konwertujemy ciąg binarny na 64-bitowe bloki
//     vector<string> blocks = split_into_blocks(input);

//     // Wypisujemy oryginalne bloki przed inicjalną permutacją
//     cout << "Bloki przed inicjalną permutacją: " << endl;
//     for (const auto &block : blocks) {
//         cout << block << endl;
//     }

//     // Iteracja po blokach, wykonywanie rund Feistela
//     for (int block_idx = 0; block_idx < blocks.size(); ++block_idx) {
//         string block = blocks[block_idx];
//         bitset<64> block_bitset(block);
        
//         // Wykonanie inicjalnej permutacji
//         bitset<64> permuted_block = initial_permutation(block_bitset, IP);  // Używamy permutacji IP

//         // Rozdzielenie na lewą i prawą część
//         uint32_t L = permuted_block.to_ulong() >> 32;
//         uint32_t R = permuted_block.to_ulong() & 0xFFFFFFFF;

//         cout << "Przetwarzany blok " << block_idx + 1 << ": " << endl;
//         cout << "L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;

//         // Rundy deszyfrowania z odwróconymi kluczami (od 15 do 0)
//         for (int i = 15; i >= 0; --i) {
//             uint64_t round_key = round_keys[i].to_ullong();  // Klucz rundy
//             cout << "Runda " << (16 - i) << " z kluczem: " << bitset<48>(round_key) << endl;

//             // Wywołanie rundy Feistela
//             tie(L, R) = feistel_round(L, R, round_key);  // Wykonanie rundy Feistela
//             cout << "Po rundzie " << (16 - i) << ": L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;
//         }

//         // Zamiana miejsc po 16 rundach
//         swap(L, R);
        
//         // Przed finalną permutacją (IP^-1)
//         bitset<64> final_block = final_permutation((static_cast<uint64_t>(L) << 32) | R, IP_inverse);  // IP^-1 na końcu
//         cout << "Blok po 16 rundach i finalnej permutacji: ";
        
//         // Wypisanie finalnego bloku w formacie szesnastkowym
//         cout << "Blok " << block_idx + 1 << ": 0x" << uppercase << hex << setw(16) << setfill('0') << final_block.to_ullong() << endl;
//     }

//     return 0;
// }

int main() {
    // Klucz główny
    string hex_masterkey = "0E329232EA6D0D73"; 
    bitset<64> masterkeyBitset = stringToBitset64(hex_masterkey);
    vector<bitset<48>> round_keys = generate_round_keys(masterkeyBitset);

    cout << "Klucz główny (w szesnastkowo): " << hex << uppercase << setw(16) << setfill('0') << masterkeyBitset.to_ullong() << endl;

    // Bloki do odszyfrowania
    vector<string> encrypted_blocks = {
        "5F381D838059F01E", "F7EC72F42157BB40", 
        "F4C312059F9A6386", "6E163F14940F223D", "9CA350518752E038"
    };

    // Odszyfrowanie bloków
    for (const auto& hex_block : encrypted_blocks) {
        bitset<64> block_bitset = hexToBitset64(hex_block);  // Konwersja bloku hex na bitset

        // Deszyfrowanie bloku
        bitset<64> decrypted_block = decryptBlock(block_bitset, round_keys);

        // Konwersja odszyfrowanego bloku na tekst
        string decrypted_text = bitsetToText(decrypted_block);

        // Wyświetlanie odszyfrowanego tekstu
        cout << "Odszyfrowany tekst: " << decrypted_text << endl;
    }

    return 0;
}