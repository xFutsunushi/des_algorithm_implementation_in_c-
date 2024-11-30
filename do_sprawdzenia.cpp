#include <iostream>
using namespace std;

int main()
{
    int liczba;
    cout << "Podaj punkty" << endl;
    cin >> liczba;

    if (liczba <= 100 && liczba >= 96) {
        cout << "A" << endl;
    }
    else if (liczba <= 95 && liczba >= 90) {
        cout << "B" << endl;
    }
    else if (liczba <= 89 && liczba >= 75) {
        cout << "C" << endl;
    }
    else if (liczba <= 74 && liczba >= 65) {
        cout << "D" << endl;
    }
    else if (liczba <= 64 && liczba >= 60) {
        cout << "F" << endl;
    }

    else if (liczba <= 59 && liczba >= 0) {
        cout << "F--" << endl;
    }

    else {
        cout << "Podaj poprawny wynik";
    }

    return 0;
}
