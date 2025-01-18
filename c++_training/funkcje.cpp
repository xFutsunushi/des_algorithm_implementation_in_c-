#include <iostream>
using namespace std;

// void printStars( int countStars );

// int main() {
//     for (int i = 0; i < 10; i++){
//     printStars(i);
//     cout << endl;
//     }

//     cout << endl << endl;

//     return 0;

// }

// void printStars( int starCount ){
//     for (int i = 0; i < starCount; i++){
//         cout << '*';
//     }

// }

void printOurSymbol(int symbCount, int lineCout, char ourSymbol);

int main() {
    printOurSymbol(7, 5, '*');
    cout << endl << endl;
    return 0;
}

void printOurSymbol(int symbCount, int lineCout, char ourSymbol){
    for ( int i = 0; i < lineCout; i++ ){
        for ( int j = 0; j < symbCount; j++ ){
            cout << ourSymbol;
        }
        cout << endl;
    }
}

