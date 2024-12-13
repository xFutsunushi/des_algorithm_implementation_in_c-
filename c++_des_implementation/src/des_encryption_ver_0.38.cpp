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

// Funkcja permutacji P
uint32_t permute_P(uint32_t sbox_result) {
    uint32_t pbox_result = 0;
    for (int i = 0; i < 32; ++i) {
        int bit_position = P[i] - 1;  // Indeksowanie od 1 w permutacji
        pbox_result |= ((sbox_result >> bit_position) & 1) << (31 - i);  // Ustawiamy bity zgodnie z permutacją
    }
    return pbox_result;
}

// Funkcja permutacji ogólnej
uint64_t initial_permute(uint64_t input, const int *permutation_table, size_t output_size) {
    uint64_t output = 0;
    cout << "Input: " << bitset<64>(input) << " (" << hex << uppercase << input << ")" << endl;

    // Przechodzimy po tablicy permutacji
    for (size_t i = 0; i < output_size; ++i) {
        // Permutacja używa pozycji bitów od 1, więc musimy zmniejszyć o 1
        size_t bit_position = permutation_table[i] - 1;

        // Przesuwamy bity w prawo, by wydobyć odpowiedni bit
        uint64_t bit = (input >> (63 - bit_position)) & 1;

        // Ustawiamy bit w wynikowym słowie
        output |= bit << (output_size - 1 - i);

        // Wyświetlamy postęp
        //cout << "Bit " << i << ": from position " << permutation_table[i] << " -> " << bit
             //<< ", Output: " << bitset<64>(output) << " (0x" << hex << uppercase << output << ")" << endl;
    }

    cout << "Permuted Output: " << bitset<64>(output) << " (0x" << hex << uppercase << output << ")" << endl;
    return output;
}
// Funkcja pomocnicza do konwersji 64-bitowego klucza do bitów

// Funkcja konwertująca tekst na ciąg binarny
std::string text_to_binary(const std::string &text) {
    std::string binary_str = "";
    
    for (char c : text) {
        // Przekształcamy każdy znak w jego binarną reprezentację
        binary_str += std::bitset<8>(c).to_string();
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
    uint32_t new_R = L ^ pbox_result;

    // Zwracamy nowe części
    return {R, new_R};
}

uint64_t final_permutation(uint64_t input, const int* permutation_table) {
    uint64_t output = 0;
    for (size_t i = 0; i < 64; ++i) {
        size_t bit_position = FP[i] - 1;
        uint64_t bit = (input >> (63 - bit_position)) & 1;
        output |= bit << (63 - i);  // Zapisz bit w odpowiedniej pozycji w wyniku
    }
    return output;
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

int main() {
    string input = "Your lips are smoother than vaseline";

    string binary_input = text_to_binary(input);  // Twoja funkcja konwersji tekstu na binarny ciąg
    //cout << "Długość ciągu binarnego: " << binary_input.length() << endl;
    //cout << "Binarny ciąg: " << binary_input<< std::endl;

    string hex_masterkey = "0E329232EA6D0D73"; 
    bitset<64> masterkeyBitset = stringToBitset64(hex_masterkey);
    vector<uint64_t> input_in_uint64 = stringToUint64(input);

    vector<bitset<48>> round_keys = generate_round_keys(masterkeyBitset);
    cout << "Master key in bits: " << masterkeyBitset << endl;
    save_round_keys(round_keys, "roundkeys.bin");

    // // Wyświetlanie kluczy rundowych -- sprawdzone, funkcja dziala
    // for (int i = 0; i < round_keys.size(); ++i) {
    //     cout << "Round " << i + 1 << " key: " << round_keys[i] << endl;
    // }

    // Dzielimy tekst na 64-bitowe bloki
    vector<string> blocks = split_into_blocks(input);

    // Wypisanie podzielonych bloków
    cout << "Podzielone bloki: " << endl;
    for (const auto &block : blocks) {
        cout << block << endl;
    }

    // // TEST FUNKCJI INIT PERMUTATION - DZIAŁA
    // // bitset<64> test_input = 0b0000000100100011010001010110011110001001101010111100110111101111;
    // bitset<64> test_input2 = 0b0011000000110001001100100011001100110100001101010011011000110111;
    // uint64_t input_value = test_input2.to_ullong();
    // uint64_t permute_test = initial_permute(input_value, IP.data(), 64);
    // cout << "PERMUTE TEST !!!!!!!!!!!: " << permute_test << endl;

    // Przetwarzanie każdego bloku przez 16 rund Feistela
    for (const auto &block : blocks) {
        //cout << "Przetwarzany blok: L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;
        uint32_t L = bitset<32>(block.substr(0, 32)).to_ulong();
        uint32_t R = bitset<32>(block.substr(32, 32)).to_ulong();

        // Przeprowadzenie 16 rund Feistela
        for (int i = 0; i < 16; ++i) {
            uint64_t round_key = round_keys[i].to_ullong(); // Konwersja klucza na uint64_t
            cout << "Runda " << i + 1 << " z kluczem: " << bitset<48>(round_key) << endl;
        
            // Wywołanie rundy Feistela
            tie(L, R) = feistel_round(L, R, round_key); // Wykonanie rundy Feistela
            cout << "Po rundzie " << i + 1 << ": L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;
    }

    // Zamiana miejsc po 16 rundach
    swap(L, R);
    // Przechowanie wyniku po 16 rundach
    uint32_t final_L = L;
    uint32_t final_R = R;

    cout << "Po zamianie miejsc: L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;

    // Wykonanie finalnej permutacji
    uint64_t final_block = final_permutation((static_cast<uint64_t>(L) << 32) | R, FP.data());
    cout << "Blok po finalnej permutacji: " << bitset<64>(final_block) << endl;

    // Wyświetlenie końcowego wyniku bloku
    cout << "Blok po 16 rundach i finalnej permutacji: " << bitset<64>(final_block) << " (" << hex << uppercase << final_block << ")" << endl;
    
    // cout << endl;
    // cout << "TEST Expand function" << endl; 

    //test_expand();

    // // TEST SBOX
    // bitset<48> sbox_input = 0b011000010001011110111010100001100110010100100111;  // Przykładowe dane wejściowe (6-bitowe)
    // bitset<32> sbox_output = apply_sboxes(sbox_input).to_ullong();  // Funkcja S-Box
    // cout << "S-Box input: " << bitset<48>(sbox_input) << endl;
    // cout << "S-Box output: " << bitset<32>(sbox_output) << endl;
    }
    return 0;
}