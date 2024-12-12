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

string generate_64bit_key() {
    // Używamy generatora liczb losowych
    random_device rd;
    mt19937_64 gen(rd());  // Generator liczb losowych 64-bitowy
    uniform_int_distribution<unsigned long long> dis(0, 0xFFFFFFFFFFFFFFFF);  // Zakres 64 bitów

    // Generujemy losowy 64-bitowy klucz
    unsigned long long masterkey = dis(gen);

    // Konwertujemy na format hex (16 znaków)
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << masterkey;
    return ss.str();
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

void print_hex(uint64_t block) {
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << block;
    cout << ss.str() << endl;
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

// Funkcja do podziału 64-bitowego bloku na lewą i prawą część
pair<uint32_t, uint32_t> splitBlock(uint64_t block) {
    // Lewa połowa to pierwsze 32 bity (najbardziej znaczące)
    uint32_t left = static_cast<uint32_t>(block >> 32);
    // Prawa połowa to ostatnie 32 bity (najmniej znaczące)
    uint32_t right = static_cast<uint32_t>(block & 0xFFFFFFFF);

    return {left, right};
}

// Funkcja rozszerzenia
uint64_t expand(uint32_t R) {
    uint64_t expanded_R = 0;

    for (int i = 0; i < 48; ++i) {
        int bit_position = E[i] - 1;  // Indeksowanie od 1 w tabeli
        uint64_t bit = (R >> bit_position) & 1;  // Pobranie odpowiedniego bitu z R
        expanded_R |= (bit << (47 - i));  // Ustawienie bitu w odpowiedniej pozycji
    }

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
uint32_t apply_sboxes(uint64_t xor_result) {
    uint32_t sbox_result = 0;

    for (int i = 0; i < 8; ++i) {
        // Pobierz odpowiednie 6 bitów (zaczynając od najbardziej znaczących)
        uint32_t chunk = (xor_result >> (6 * (7 - i))) & 0x3F;

        // Wyciągnij bity rzędu (pierwszy i ostatni bit)
        uint32_t row = ((chunk & 0x20) >> 4) | (chunk & 0x01);

        // Wyciągnij bity kolumny (środkowe 4 bity)
        uint32_t col = (chunk >> 1) & 0x0F;

        // Znajdź wartość z odpowiedniego S-Boxa i zapisz w wyniku
        sbox_result |= (SBOX[i][row][col] << (4 * (7 - i)));
    }

    //cout << "Wynik S-Boxów: " << bitset<32>(sbox_result) << endl;
    return sbox_result;
}

// Funkcja rundy Feistela
pair<uint32_t, uint32_t> feistel_round(uint32_t L, uint32_t R, uint64_t round_key) {
    //cout << "Przed rundą Feistela: L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;
    
    // Ekspansja Prawa część (R) na 48 bitów
    uint64_t expanded_R = expand(R);
    //cout << "Rozszerzone R: " << bitset<48>(expanded_R) << endl;

    // XOR z kluczem rundy
    uint64_t xor_result = expanded_R ^ round_key;
    //cout << "R po XOR z kluczem rundy: " << bitset<48>(xor_result) << endl;

    uint32_t sbox_result = apply_sboxes(xor_result);

    // Wynik po S-Boxach
    //cout << "Wynik po S-Boxach: " << bitset<32>(sbox_result) << endl;

    // Permutacja P
    uint32_t pbox_result = permute_P(sbox_result);
    //cout << "Wynik permutacji P: " << bitset<32>(pbox_result) << endl;

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

int main() {
    string input = "Your lips are smoother than vaseline" ; // zmienna do trzymania wejścia z klawiatury
    //string input = "596F7572206C6970 732061726520736D 6F6F746865722074 68616E2076617365 6C696E650D0A";
    //string input = "8787878787878787";

    // Padding
    while (input.length() % 16 != 0) {
        input += "00"; // Padding 0s
    }

    string hex_masterkey = "0E329232EA6D0D73"; 
    bitset<64> masterkeyBitset = stringToBitset64(hex_masterkey);
    vector<uint64_t> input_in_uint64 = stringToUint64(input);

    vector<bitset<48>> round_keys = generate_round_keys(masterkeyBitset);
    cout << "Master key in bits: " << masterkeyBitset << endl;
    save_round_keys(round_keys, "roundkeys.bin");

    // // Wyświetlanie kluczy rundowych
    // for (int i = 0; i < round_keys.size(); ++i) {
    //     cout << "Round " << i + 1 << " key: " << round_keys[i] << endl;
    // }

    vector<pair<uint32_t, uint32_t>> split_blocks;
    for (auto &block : input_in_uint64) {
        uint64_t permuted_block = initial_permute(block, IP.data(), 64);
        //cout << "Permuted block: " << bitset<64>(permuted_block) << endl;
        auto [L0, R0] = splitBlock(permuted_block);
        split_blocks.emplace_back(L0, R0);
    }

    // // TEST FUNKCJI INIT PERMUTATION - DZIAŁA
    // // bitset<64> test_input = 0b0000000100100011010001010110011110001001101010111100110111101111;
    // bitset<64> test_input2 = 0b0011000000110001001100100011001100110100001101010011011000110111;
    // uint64_t input_value = test_input2.to_ullong();
    // uint64_t permute_test = initial_permute(input_value, IP.data(), 64);
    // cout << "PERMUTE TEST !!!!!!!!!!!: " << permute_test << endl;

    // Przetwarzanie każdego bloku przez 16 rund Feistela
    for (auto &[L, R] : split_blocks) {
        //cout << "Przetwarzany blok: L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;

        // Przeprowadzenie 16 rund Feistela
        for (int i = 0; i < 16; ++i) {
            uint64_t round_key = round_keys[i].to_ullong(); // Konwersja klucza na uint64_t
            //cout << "Runda " << i + 1 << " z kluczem: " << bitset<48>(round_key) << endl;
        
            // Wywołanie rundy Feistela
            tie(L, R) = feistel_round(L, R, round_key); // Wykonanie rundy Feistela
            //cout << "Po rundzie " << i + 1 << ": L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;
    }

    // Zamiana miejsc po 16 rundach
    swap(L, R);
    // Przechowanie wyniku po 16 rundach
    uint32_t final_L = L;
    uint32_t final_R = R;

    //cout << "Po zamianie miejsc: L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;

    // Wykonanie finalnej permutacji
    uint64_t final_block = final_permutation((static_cast<uint64_t>(L) << 32) | R, FP.data());
    //cout << "Blok po finalnej permutacji: " << bitset<64>(final_block) << endl;

    // Wyświetlenie końcowego wyniku bloku
    //cout << "Blok po 16 rundach i finalnej permutacji: " << bitset<64>(final_block) << " (" << hex << uppercase << final_block << ")" << endl;

        // Wyświetlanie wyników po wszystkich rundach
    cout << "Final Block (after 16 rounds and final permutation): "
         << "L = " << bitset<32>(final_L) << ", R = " << bitset<32>(final_R)
         << ", Final block: " << bitset<64>(final_block) << " (Hex: " << hex << final_block << ")" << endl;
}
    return 0;
}