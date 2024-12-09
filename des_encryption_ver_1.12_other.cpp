#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Szablon do permutacji
template <size_t N, size_t M>
bitset<N> apply_permutation(const bitset<M>& input, const int* perm_table) {
    bitset<N> result;
    for (int i = 0; i < N; ++i) {
        result[i] = input[perm_table[i] - 1]; // Indeksowanie od 1
    }
    return result;
}

// Rotacja w lewo dla bitset<28>
bitset<28> rotate_left(const bitset<28>& input, int shifts) {
    return (input << shifts) | (input >> (28 - shifts));
}

// Funkcja Feistela
bitset<32> feistel(const bitset<32>& R, const bitset<48>& round_key, const int* E, const int S_BOX[8][4][16], const int* P) {
    // Rozszerzenie do 48 bitów
    bitset<48> expanded_R = apply_permutation<48, 32>(R, E);

    // XOR z kluczem rundy
    bitset<48> xor_result = expanded_R ^ round_key;

    // S-Boxy
    bitset<32> sbox_output;
    for (int i = 0; i < 8; ++i) {
        int row = (xor_result[47 - 6 * i] << 1) | xor_result[47 - 6 * i - 5];
        int col = (xor_result[47 - 6 * i - 1] << 3) | (xor_result[47 - 6 * i - 2] << 2) |
                  (xor_result[47 - 6 * i - 3] << 1) | xor_result[47 - 6 * i - 4];
        int sbox_value = S_BOX[i][row][col];
        for (int j = 0; j < 4; ++j) {
            sbox_output[31 - 4 * i - j] = (sbox_value >> (3 - j)) & 1;
        }
    }

    // Permutacja P
    return apply_permutation<32, 32>(sbox_output, P);
}

// Generowanie kluczy rundowych
vector<bitset<48>> generate_round_keys(const bitset<64>& key, const int* PC1, const int* PC2) {
    vector<bitset<48>> round_keys;

    // Permutacja początkowa PC-1
    bitset<56> permuted_key = apply_permutation<56, 64>(key, PC1);

    // Podział na lewą i prawą część
    bitset<28> left = (permuted_key >> 28).to_ulong();
    bitset<28> right = (permuted_key & bitset<56>(0xFFFFFFF)).to_ulong();

    // Rotacje i klucze rundowe
    const int shifts[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    for (int round = 0; round < 16; ++round) {
        left = rotate_left(left, shifts[round]);
        right = rotate_left(right, shifts[round]);

        bitset<56> combined_key = (bitset<56>(left.to_ulong()) << 28) | right.to_ulong();
        round_keys.push_back(apply_permutation<48, 56>(combined_key, PC2));
    }

    return round_keys;
}

// DES - Funkcja główna
bitset<64> des_encrypt(const bitset<64>& plaintext, const vector<bitset<48>>& round_keys, const int* IP, const int* IP_INV, const int* E, const int S_BOX[8][4][16], const int* P) {
    // Permutacja początkowa
    bitset<64> permuted_text = apply_permutation<64, 64>(plaintext, IP);

    // Podział na L i R
    bitset<32> L = (permuted_text >> 32).to_ulong();
    bitset<32> R = (permuted_text & bitset<64>(0xFFFFFFFF)).to_ulong();

    // 16 rund DES
    for (int round = 0; round < 16; ++round) {
        bitset<32> temp = R;
        R = L ^ feistel(R, round_keys[round], E, S_BOX, P);
        L = temp;
    }

    // Połączenie R i L (odwrotna kolejność)
    bitset<64> combined_text = (bitset<64>(R.to_ulong()) << 32) | L.to_ulong();

    // Permutacja końcowa
    return apply_permutation<64, 64>(combined_text, IP_INV);
}

int main() {
    // Przykładowy klucz i plaintext (w formacie 64-bitowego bitsetu)
    bitset<64> key(0x133457799BBCDFF1);
    bitset<64> plaintext(0x0123456789ABCDEF);

    // Tablice permutacji (zakładamy, że są zdefiniowane w kodzie)
    extern const int PC1[], PC2[], IP[], IP_INV[], E[], P[];
    extern const int S_BOX[8][4][16];

    // Generowanie kluczy rundowych
    vector<bitset<48>> round_keys = generate_round_keys(key, PC1, PC2);

    // Szyfrowanie
    bitset<64> ciphertext = des_encrypt(plaintext, round_keys, IP, IP_INV, E, S_BOX, P);

    // Wynik
    cout << "Plaintext:  " << plaintext << endl;
    cout << "Ciphertext: " << ciphertext << endl;

    return 0;
}
