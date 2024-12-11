#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int main() {
    
    const int max_length = 30; // max liczba odczytanych znaków
    
    char text[max_length]; // tablica do odczytania ciągu
    cout << "Enter some text: " << endl;

    // Czytamy znaki łącznie ze spacjami 
    cin.getline(text, max_length);

    //cin >> text;

    cout << strlen(text) << endl; 
    cout << sizeof(text) << endl;

    cout << "You entered: \n" << text << endl;

    return 0;
}