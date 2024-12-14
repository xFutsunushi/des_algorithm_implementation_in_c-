#include <iostream>
#include <cstring>
using namespace std;

int main() {
    const int rozmiar = 6; // Rozmiar tablicy samogłosek
    char substring[rozmiar] = {'a', 'e', 'i', 'o', 'u', 'y'};

    string tekst;
    cout << "Podaj ciag znakow: ";
    getline(cin, tekst);

    cout << "Znalezione samogloski: ";
    for (char znak : tekst) {
        // Sprawdzamy, czy znak jest samogłoską
        for (int i = 0; i < rozmiar; i++) {
            if (tolower(znak) == substring[i]) { // Konwersja do małych liter
                cout << znak << " ";
                break;
            }
        }
    }
    cout << endl;

    return 0;
}