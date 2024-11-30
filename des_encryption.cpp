#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;


string str;
string ret;
string splited;
// Kroki do szyfrowania DES
/*

Zdefiniuj główne funkcje algorytmu:

Permutacja początkowa (IP).
Generacja podkluczy (16 kluczy po 48 bitów).
Funkcja przekształcająca (F-function).
Operacje na tabelach S-box.
Permutacja końcowa (FP).
Stwórz strukturę danych:

Blok wejściowy (64-bitowy).
Klucz (64-bitowy).
*/

// Poprosić użytkownika o ciąg znaków do szyfrowania
// Podzielenie wejścia na bloki 64 bit (8 bajtów) - jeden znak to 
// Permutacja pocztąkowa
// Dzielenie na dwie połowy 32 bitowe
// 16 x wykonana funkcja feistela
// Permutacja końcowa

vector<string> Split(const string& str, int splitLength) {

   int NumSubstrings = str.length() / splitLength;
   vector<string> ret;

   for (auto i = 0; i < NumSubstrings; i++)
   {
        ret.push_back(str.substr(i * splitLength, splitLength));
   }

   if (str.length() % splitLength != 0)
   {
        ret.push_back(str.substr(splitLength * NumSubstrings));
   }

   return ret;
}



// string entry_perm() {

//     cout << "Podaj ciag znakow do zaszyfrowania: " << endl; 
//     cin >> str;

//     // Po podzieleniu na bloki 64 bitowe, bierzemy i na każdym bloku robimy permutację początkow
//     Split(str,8);

//     return 0;
// }


vector<string> main() {

    cout << "Wprowadz ciag znakow do zaszyfrowania: " << endl;
    cin >> str;

    Split(str,8);
    
    return Split;

}

