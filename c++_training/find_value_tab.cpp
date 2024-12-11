#include <iostream>
using namespace std;

const unsigned int N = 10;
int arr[N] = {5,12,34,16,29,4,76,54,8,6};

int main() {
    int pattern = 8; // szukana wartość
    unsigned int i;

    for ( i = 0; i < N; i++ ) {
        if ( arr[i] == pattern ) {
            cout << "Znaleziono szukana wartosc" << endl;
            break;
        }
    }


if ( i >= N ) {
    cout << "Element nie został znaleziony " << endl;
}

else {
    cout << "Indeks pierwsze go wystapienia szukanej wartosci: " << i << endl;
}
}