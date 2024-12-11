#ifndef PERMUTATION_TABLES_H
#define PERMUTATION_TABLES_H

#include <vector>
#include <array>
using namespace std;

// TABLICE PERMUTACJI 
// Permutacja początkowa  ( Initial permutation )
extern const array<int, 64> IP;

// Permutacja końcowa  ( Final permutation )
extern const array<int, 64> FP;

// Rozszerzenie ( Expansion Table - E )
extern const array<int, 48> E;

// Permutacja P
extern const array<int, 32> P;

// Tablice S-Box (Substitution Boxes)
extern const int SBOX[8][4][16];

// Permutacja klucza (PC1)
extern const array<int, 56> PC1;

// Permutacja klucza (PC2)
extern const array<int, 48> PC2;

// Przesunięcie w lewo (liczba bitów) dla każdej rundy
extern const int shiftTable[16];

// Tablica do odwrotnej permutacji
extern array<int, 64> IP_inverse;

#endif // PERMUTATION_TABLES_H