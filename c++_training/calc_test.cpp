#include <iostream>

using namespace std;

int main(){

    char operation;
    int x;
    int y;

    cout << "Podaj jaką operacje chcesz wykonać: '+', '-', '*', '/', '%'";
    cin >> operation;

    switch (operation) {
        case '+':
            cout << "Wynik dodawania x + y = " << x + y << endl;
            break;
        default:
            cout << "Podaj poprawne liczby całkowite" << endl;
            break;
    }

    return 0;

}