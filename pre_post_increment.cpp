#include  <iostream>
using namespace std;


bool prawda_falsz() {

    bool a = true;
    bool b = false;
    bool c = true;

    bool wynik = a || b && c;

    cout << "Wynikt (a || b && c) ==: " << wynik << endl;

    return 0;
}

int main() {

//     int x = 10;
//     x--;
//     cout << x << endl;
//     x++;

//     cout << x << endl;

//     int wynik1 = x-- + 5;
//     cout << "Wynik (x-- + 5): " << wynik1 << endl;
//     cout << "Wartość x po postfiksowej inkrementacji: " << x << endl;

// //   cout << wynik1 << "Wynik 1 po post inkrementacji";

//     x = 10;
//     int wynik2 = --x + 5;

//     cout << "Wynik (--x + 5): " << wynik2 << endl;
//     cout << "Wartos x po prefiksowej inkrementacji: " << wynik2 << endl;

    int a = 5; 
    int b = 3;

    int wynik = a & b + 2; // najpierw +
    cout << "Wynik (a & b + 2): " << wynik << endl;

    return 0;
}

// & dodawanie binarne 
// 0101
// 0101 +
// --------------
// 0101
// czyli wynik to 5 (bo najpierw b+2 czyli 5 i binarnie dodajemy 5 + 5 jak wyżej)

