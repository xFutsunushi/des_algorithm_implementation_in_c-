#include <iostream>
#include <string>
#include <cstring>
#include <bits/stdc++.h>
using namespace std;

int main() {
    // const int max_length = 30;
    // string zdanie[max_length];
    const string zdanie = "Policz zdania w tekscie raz dwa raz";

    // cout << "Podaj zdanie do podzielenia na slowa" << endl; 
    // getline(cin, zdanie);
    //cout << zdanie;
    int words = 0;
    //int n = zdanie.length();

    for ( int i = 0; i < zdanie.length(); ++i) {
        if ( zdanie[i] == ' ' )
        words++;
    }
    words = words + 1; // Ostatni wyraz kończy się bez spacji więc dodajemy +1
    cout << words << endl; 

    return 0;
}