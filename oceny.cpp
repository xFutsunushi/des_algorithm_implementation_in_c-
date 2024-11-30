#include <iostream>
#include <vector>

using namespace std;

int main() {
    int points;
    cout << "Podaj liczbę punktów z egzaminu: ";
    cin >> points;

        switch (points) {
        case 96 ... 100:
            cout << "Ocena:  A" << endl;
            break;
        case 90 ... 95:
            cout << "Ocena: B"<< endl;
            break;
        case 75 ... 89:
            cout << "Ocena: C" << endl;
            break;
        case 65 ... 74 :
            cout << "Ocena: D" << endl;
            break;
        case 60 ... 64 :
            cout << "Ocena: E" << endl;
            break;

        case 0 ... 59 :
            cout << "Ocena: F" << endl;
            break;

        default:
            cout << "ERROR: Nie podałeś punktacji" << endl;
            break;
        }

    return 0;
}
