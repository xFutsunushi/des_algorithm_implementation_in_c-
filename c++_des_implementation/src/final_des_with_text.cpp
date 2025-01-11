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

vector<bitset<64>> split_string_into_binary_blocks(const string &input) {
    vector<bitset<64>> blocks;
    size_t block_size = 64;  // Rozmiar bloku 64 bity
    size_t input_size = input.size() * 8;  // Rozmiar wejścia w bitach

    // Konwertujemy string na ciąg bitów
    string binary_input = "";
    for (char c : input) {
        binary_input += bitset<8>(c).to_string();
    }

    // Upewniamy się, że ciąg ma długość podzielną przez 64 bity
    if (binary_input.size() % block_size != 0) {
        size_t padding_size = block_size - (binary_input.size() % block_size);
        binary_input.append(padding_size, '0');  // Dodajemy padding
    }

    // Dzielimy ciąg bitów na 64-bitowe bloki
    for (size_t i = 0; i < binary_input.size(); i += block_size) {
        bitset<64> block(binary_input.substr(i, block_size));
        blocks.push_back(block);
    }

    return blocks;
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

bitset<32> permute_P(const bitset<32>& sbox_result, const array<int, 32>& P, bool
 debug = false) {
    bitset<32> pbox_result;

    for (int i = 0; i < 32; ++i) {
        // Indeksowanie od lewej do prawej w P, dostosowanie do indeksów bitset
        int bit_position = 31 - (P[i] - 1);

        // Przypisanie bitu z sbox_result do odpowiedniej pozycji w pbox_result
        pbox_result[ 31 - i ] = sbox_result[bit_position];

        // Debugowanie
        if (debug) {
            cout << "Przed permutacją: bit " << i << " z P[" << i << "] = " << P[i]
                 << ", wartość: " << sbox_result[bit_position] << endl;
            cout << "Po permutacji: bit " << i << " w pbox_result[" << 31 - i << "] = "
                 << pbox_result[ 31 - i ] << endl;
        }
    }

    return pbox_result;
}

bitset<64> initial_permutation(const bitset<64>& input, const array<int, 64>& IP) {
    bitset<64> permuted;

        bitset<64> output;
        for (size_t i = 0; i < 64; ++i) {
            output[63 - i] = input[64 - IP[i]]; // Mapowanie pozycji zgodnie z tablicą IP
    }

    return output;
}

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

vector<bitset<64>> split_binary_into_blocks(const bitset<64>& binary_input) {
    vector<bitset<64>> blocks;
    size_t block_size = 64;  // Rozmiar bloku 64 bity

    // Dzielimy ciąg bitów na 64-bitowe bloki
    size_t input_size = binary_input.size();
    for (size_t i = 0; i < input_size; i += block_size) {
        bitset<64> block;
        for (size_t j = 0; j < block_size; ++j) {
            if (i + j < input_size) {
                block[j] = binary_input[i + j];
            }
        }
        blocks.push_back(block);
    }

    return blocks;
}

bitset<48> expand(const bitset<32>& R) {
    bitset<48> expanded_R;

    // Iteracja po tabeli rozszerzenia
    for (int i = 0; i < 48; ++i) {
        int bit_position = E[i] - 1;  // Indeksowanie od 1 w tabeli (0-bazowe w C)
        
        // Przesunięcie bitu z R na odpowiednią pozycję
        bool bit = R[31 - bit_position];  // Używamy indeksowania bitset w odwrotnej kolejności

        // Ustawienie bitu w odpowiedniej pozycji w expanded_R
        expanded_R[47 - i] = bit;  // Ustawienie bitu w odpowiedniej pozycji
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

bitset<32> apply_sboxes(bitset<48> xor_result, bool debug = false) {
    bitset<32> sbox_result;

    for (int i = 0; i < 8; ++i) {
        // Zmieniamy sposób wyciągania bitów, zapewniając odpowiednią kolejność
        bitset<6> chunk;
        for (int j = 0; j < 6; ++j) {
            chunk[j] = xor_result[47 - (6 * i + (5 - j))]; // Odczytujemy bity od lewej do prawej
        }

        if (debug) {
            cout << "Krok " << i + 1 << " - chunk (6 bitów): " << chunk << endl;
        }

        // Wyciągamy bity rzędu (pierwszy i ostatni bit)
        bitset<1> row_bit1(chunk[0]); // Pierwszy bit
        bitset<1> row_bit2(chunk[5]); // Ostatni bit
        bitset<2> row;
        row[0] = row_bit1[0]; // Przypisujemy pierwszy bit
        row[1] = row_bit2[0]; // Przypisujemy ostatni bit

        if (debug) {
            cout << "Rząd: " << row << " (bity: " << row_bit1[0] << ", " << row_bit2[0] << ")" << endl;
        }

        // Wyciągamy bity kolumny (środkowe 4 bity)
        bitset<4> col_bits;
        for (int j = 1; j < 5; ++j) {
            col_bits[j - 1] = chunk[j]; // Środkowe bity (1 do 4)
        }

        if (debug) {
            cout << "Kolumna: " << col_bits << endl;
        }

        // Znajdujemy wartość z odpowiedniego S-Boxa i zapisujemy ją w wyniku
        bitset<8> sbox_value = SBOX[i][row.to_ulong()][col_bits.to_ulong()]; // Wartość z S-Boxa

        if (debug) {
            cout << "Wartość z S-Boxa: " << sbox_value << endl;
        }

        // Przekształcamy sbox_value do bitset<32> (przesuwamy ją na odpowiednią pozycję)
        bitset<32> sbox_value_32;
        sbox_value_32 |= (sbox_value.to_ulong() << (4 * (7 - i))); // Przesuwamy i ustawiamy w odpowiednich bitach

        if (debug) {
            cout << "Wartość S-Boxa po przesunięciu: " << sbox_value_32 << endl;
        }

        // Łączymy sbox_result z nową wartością
        sbox_result |= sbox_value_32;

        if (debug) {
            cout << "Sbox_result po dodaniu: " << sbox_result << endl;
        }
    }

    return sbox_result;
}

pair<bitset<32>, bitset<32>> feistel_round(bitset<32> L, bitset<32> R, bitset<48> round_key) {
    cout << "Przed rundą Feistela: L = " << L << ", R = " << R << endl;
    
    // Ekspansja Prawa część (R) na 48 bitów
    bitset<48> expanded_R = expand(R); // Funkcja expand powinna zwracać bitset<48>
    cout << "Rozszerzone R: " << expanded_R << endl;

    // XOR z kluczem rundy
    bitset<48> xor_result = expanded_R ^ round_key;  // XOR na poziomie bitów
    cout << "R po XOR z kluczem rundy: " << xor_result << endl;
    cout << endl;

    // Zastosowanie S-boxów 
    bitset<32> sbox_result = apply_sboxes(xor_result, false); // Zmieniamy na bitset<32>
    cout << "Wynik po S-Boxach: " << sbox_result << endl;

    // Permutacja P
    bitset<32> pbox_result = permute_P(sbox_result, P, true); // Funkcja permute_P powinna zwracać bitset<32>
    cout << "Wynik permutacji P: " << pbox_result << endl;

    // XOR z lewą częścią
    bitset<32> new_R = L ^ pbox_result;  // XOR na poziomie bitów z lewą częścią
    cout << "XOR z lewa czescia: " << new_R << endl;

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


int main() {
    string input = "Your lips are smoother than vaseline";
    string hex_masterkey = "0E329232EA6D0D73"; // For Text text
    bitset<64> masterkeyBitset = stringToBitset64(hex_masterkey);
    vector<bitset<48>> round_keys = generate_round_keys(masterkeyBitset);

    // Dzielimy tekst na 64-bitowe bloki
    vector<bitset<64>> blocks = split_string_into_binary_blocks(input);

    // Wypisanie podzielonych bloków
    cout << "Podzielone bloki: " << endl;
    for (const auto &block : blocks) {
        cout << block << endl;
    }

    // Wypisujemy oryginalne bloki przed inicjalną permutacją
    for (const auto &block : blocks) {
        //cout << "Blok przed initial Permutation: " << block << endl;
    }

    // Przetwarzamy każdy blok 64 bitowy przez inicjalną permutację
    for (const auto &block : blocks) {
        // Wykonanie inicjalnej permutacji
        bitset<64> permuted_block = initial_permutation(block, IP);
        //cout << "Wynik po Initial Permutation: " << permuted_block << endl;
    }

    // // Wyświetlanie kluczy rundowych -- sprawdzone, funkcja dziala
    // for (int i = 0; i < round_keys.size(); ++i) {
    //     cout << "Round " << i + 1 << " key: " << round_keys[i] << endl;
    // }

    // Iteracja po blokach, wykonywanie rund Feistela
    for (int block_idx = 0; block_idx < blocks.size(); ++block_idx) {
        bitset<64> block = blocks[block_idx];

        // Wykonanie inicjalnej permutacji na bloku
        bitset<64> permuted_block = initial_permutation(block, IP);

        // Dzielimy permutowany blok na dwie części
        bitset<32> L, R;
        for (int i = 0; i < 32; ++i) {
            R[i] = permuted_block[i];          // Pierwsze 32 bity idą do L
            L[i] = permuted_block[i + 32];     // Kolejne 32 bity idą do R
    }

        // Wyświetlamy L0 i R0 po podzieleniu
        cout << "L0: " << L << ", R0: " << R << endl;

        // Przeprowadzenie 16 rund Feistela
        for (int i = 0; i < 16; ++i) {
            uint64_t round_key = round_keys[i].to_ullong(); // Konwersja klucza na uint64_t
            cout << endl;
            cout << "Runda " << i + 1 << " z kluczem: " << bitset<48>(round_key) << endl;

            // Wywołanie rundy Feistela
            tie(L, R) = feistel_round(L, R, round_key); // Wykonanie rundy Feistela
            cout << "Po rundzie " << i + 1 << ": L = " << bitset<32>(L) << ", R = " << bitset<32>(R) << endl;
            cout << "----------------------------------------------------------------------" << endl;
            cout << endl;
        }

        // Zamiana miejsc po 16 rundach
        swap(L, R);
        bitset<32> final_L = L;
        bitset<32> final_R = R;

// Połączenie final_R i final_L w final_block (64 bity)
bitset<64> final_block;
for (int i = 0; i < 32; ++i) {
    final_block[i] = final_R[i];      // Kopiowanie bitów z final_R do final_block (od 0. bitu w górę)
    final_block[i + 32] = final_L[i]; // Kopiowanie bitów z final_L do final_block (od 32. bitu w górę)
}

    cout << "Final block przed final permutation: " << final_block << endl; 

    bitset<64> full_final_block = final_permutation(final_block, FP);

    // Wypisanie finalnego bloku
    cout << "Blok po 16 rundach i finalnej permutacji: " << endl;

    // Wypisanie w postaci binarnej
    cout << "Blok " << block_idx + 1 << " (w binarnym): " << full_final_block << endl;

    // Wypisanie w postaci szesnastkowej
    cout << "Blok " << block_idx + 1 << " (w hex): 0x" << uppercase << hex << setw(16) << setfill('0') << full_final_block.to_ullong() << endl;
    }

    return 0;
}