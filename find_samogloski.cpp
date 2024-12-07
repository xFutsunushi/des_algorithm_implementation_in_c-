#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int main() {
    const int rozmiar = 50;
    char text[rozmiar];

    //Wypisujemy ciag
    cout << "Podaj text: ";
    cin.getline(text, rozmiar);
    //Liczenie liczby samoglosek
    int liczbaSamoglosek;

    const char *cstr = text;
    string s = cstr;

    char substring[rozmiar] = {'a', 'e', 'i', 'o', 'u', 'y'};

    for ( int i = 0; i < cstr.length(); ++i) {
        if ( text[i]  substring );
        cout << substring; 
        //liczbaSamoglosek++;
        //cout << "Znaleziono samogloske" << endl; 

    }

    cout << liczbaSamoglosek << endl;

    return 0;
}

// DO POPRAWKI !!!!