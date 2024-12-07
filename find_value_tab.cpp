#include <iostream>
using namespace std;

const unsigned int tablica = 10;
int arr[tablica] = {5,12,34,16,29,4,76,54,8,6};

int main() {
    int pattern = 8; // szukana wartość
    unsigned int i;

    for ( i = 0; i < sizeof(tablica); i++ ) {
        if ( arr[i] == pattern ) {
            cout << "Znaleziono szukana wartosc!" << endl;
            break;
        }
    }


if ( i >= tablica ) {
    cout << "Element nie został znaleziony " << endl;
}

else {
    cout << "Indeks pierwszego wystapienia szukanej wartosci: " << i << endl;
}
}