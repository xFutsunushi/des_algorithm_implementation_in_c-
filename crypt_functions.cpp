#include <iostream>
#include <string>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <iomanip>
#include <sstream>
#include <cstring>

using namespace std;

// Funkcja pomocnicza do konwersji na hex
string toHex(const unsigned char* data, size_t length) {
    ostringstream oss;
    for (size_t i = 0; i < length; ++i) {
        oss << hex << setw(2) << setfill('0') << (int)data[i];
    }
    return oss.str();
}


// Funkcja: Hashowanie SHA256
string sha256(const string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.size(), hash);
    return toHex(hash, SHA256_DIGEST_LENGTH);
}

// Funkcja: Szyfrowanie AES
string aesEncrypt(const string& plaintext, const string& key) {
    unsigned char iv[AES_BLOCK_SIZE] = {0}; // Wektor inicjalizacyjny
    AES_KEY encryptKey;
    AES_set_encrypt_key((const unsigned char*)key.c_str(), 128, &encryptKey);

    unsigned char encrypted[AES_BLOCK_SIZE] = {0};
    AES_cfb128_encrypt((unsigned char*)plaintext.c_str(), encrypted, plaintext.size(), &encryptKey, iv, nullptr, AES_ENCRYPT);

    return toHex(encrypted, AES_BLOCK_SIZE);
}

// Funkcja: Deszyfrowanie AES
string aesDecrypt(const string& ciphertextHex, const string& key) {
    unsigned char iv[AES_BLOCK_SIZE] = {0}; // Wektor inicjalizacyjny
    AES_KEY decryptKey;
    AES_set_decrypt_key((const unsigned char*)key.c_str(), 128, &decryptKey);

    string ciphertext = ciphertextHex; // Konwersja hex do zwykłego tekstu, do zaimplementowania

    unsigned char decrypted[AES_BLOCK_SIZE] = {0};
    AES_cfb128_encrypt((unsigned char*)ciphertext.c_str(), decrypted, ciphertext.size(), &decryptKey, iv, nullptr, AES_DECRYPT);

    return string((char*)decrypted);
}

int main() {
    // Test funkcji SHA256
    string data = "password123";
    string hash = sha256(data);
    cout << "SHA256 Hash: " << hash << endl;

    // Test AES
    string aesKey = "1234567890123456"; // Klucz 128-bitowy (16 znaków)
    string aesEncrypted = aesEncrypt(text, aesKey);
    cout << "AES Encrypted: " << aesEncrypted << endl;

    // AES Decrypt (komentarz: wymaga konwersji hex -> plaintext, do zaimplementowania)
    // string aesDecrypted = aesDecrypt(aesEncrypted, aesKey);
    // cout << "AES Decrypted: " << aesDecrypted << endl;

    return 0;
}

