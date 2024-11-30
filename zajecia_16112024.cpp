#include <iostream>
using namespace std;

int main(){

// int a = 5;
// int b = 3;
// int c = 10;

int wynik1 = 3 + 5 * 2 - 4 / 2;

int wynik2 = ( 8 + 2 ) * ( 5 - 3 ) + 7;

int wynik3 = 10 - 3 * 2 + 8 / 4;

int wynik4 = 4 * ( 3 + 2 ) - 6 / ( 1 + 1 );

bool wynik_bool1 = ( 5 + 3 * 2 > 10 ) && ( 4 - 2 < 5 );

bool wynik_bool2 = (10 / 2 + 3 ) == 8 || ( 7 - 5 * 2 != 3 );

bool wynik_bool3 = ( 6 + 4 * 2 <= 14 ) && !( 3 + 1 == 5 );


cout << "Wynik numer 1: " << wynik1 << endl;
cout << "Wynik numer 2: " << wynik2 << endl;
cout << "Wynik numer 3: " << wynik3 << endl;
cout << "Wynik numer 4: " << wynik4 << endl;
cout << "-----------------------------------------" << endl; 
cout << "Wynik numer 4: " << wynik_bool1 << endl;
cout << "Wynik numer 5: " << wynik_bool2 << endl;
cout << "Wynik numer 5: " << wynik_bool3 << endl;
cout << "-----------------------------------------" << endl; 

// cout << "Wynik a wynosi: " << a << endl; 
// int wynik_post = a++ + b * 2;
// cout << "Wynik postinkrementacji: " << wynik_post << endl;
// cout << "Wynik a wynosi: " << a << endl; 

// cout << "------------------------------------------" << endl;
// int wynik_post2 = ++a + b * 2;
// cout << "Wynik preinkrementacji: " << wynik_post2 << endl;
// cout << "Wynik a wynosi: " << a << endl; 

// cout << "------------------------------------------" << endl;
// int wynik_post3 = --c + b;
// cout << "Wynik preinkrementacji: " << wynik_post3 << endl;
// cout << "Wynik a wynosi: " << a << endl; 

// int a = 10;
// a *= 3 + 2; // tyle co a = 10 * 3 + 2
// cout << "Wynik a wynosi: " << a << endl; 

// int b = 8; b -= 4 * 2 - 1; // dopiero po obliczeniu prawej strony 
// cout << b << endl; 


int d = 15; d %= 4 + 1;
cout << d << endl; 

}
