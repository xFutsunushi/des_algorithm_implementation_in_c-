// #include <iostream>
// using namespace std;

// int main(){
//     int i = 10;
//     while ( i >= 1 ) {
//         cout << "Wartość i: " << i << endl;
//         i--;
//     }

//     return 0;

// }


// ----------------------- DO WHILE -------------------------------------------

#include <iostream>
using namespace std;


int main(){

    int liczba; 

    do  {
        cout << "Podaj liczbe: " << endl;
        cin >> liczba;

    if ( liczba != 0 ) {

        cout << "Podałeś liczbę: " << liczba << endl; 
    }

    } while ( liczba != 0 ); 

    return 0;

}