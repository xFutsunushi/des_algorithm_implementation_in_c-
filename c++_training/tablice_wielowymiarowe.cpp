#include <iostream>

using namespace std;

// int main (){

//     int rows = 4, cols = 5;
//     for ( int i = 0; i < rows; i++ ){
//         for (int j = 0; j < cols; j++ ){
//             cout << "*" << "\t";
//         }
//         cout << endl;
//     }
// }

// int main (){

//     int rows = 5, cols = 6;
//     //int tab[4][5];
//     for ( int i = 1; i < rows; i++ ){
//         for ( int j = 0; j < i; j++ ){
//             cout << "*";
//         }
//         cout << endl;
//    }
// }


int main (){

    int N;
    cout << "Podaj rozmiar tablicy: ";
    cin >> N;

    int value = 1;

    for ( int i = 1; i < N; i++ ){
        for ( int j = 0; j < N; j++ ){
            cout << value++ << "\t";
        }
        cout << endl;
   }
}