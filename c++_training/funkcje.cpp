#include <iostream>
#include <ctime>
#include <cstdlib>

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

// void printOurSymbol(int symbCount, int lineCout, char ourSymbol);

// int main() {
//     printOurSymbol(7, 5, '*');
//     cout << endl << endl;
//     return 0;
// }

// void printOurSymbol(int symbCount, int lineCout, char ourSymbol){
//     for ( int i = 0; i < lineCout; i++ ){
//         for ( int j = 0; j < symbCount; j++ ){
//             cout << ourSymbol;
//         }
//         cout << endl;
//     }
// }

// void fillAndShowArray(int arrayForFilling[], int size);


// int main(){

//     const int SIZE1 = 8;
//     const int SIZE2 = 14;

//     int arrayForFilling1 [SIZE1] = {};
//     int arrayForFilling2 [SIZE2] = {};

//     fillAndShowArray(arrayForFilling1, SIZE1);
//     fillAndShowArray(arrayForFilling2, SIZE2);

//     return 0;
// }


// void fillAndShowArray(int arrayForFilling[], int size){
//     for ( int i = 0; i < size; i++ ){
//         arrayForFilling[i] = i + 1;
//         cout << arrayForFilling[i] << " ";
//     }
//     cout << endl;
// }

const int SIZE = 5;

void fillAndShowArray(int[][SIZE], const int size);
int findMinElement(int[][SIZE], const int size);
int findMaxElement(int[][SIZE], const int size);

int main(){
    int matrix[SIZE][SIZE] = {};

    fillAndShowArray(matrix, SIZE); 

    cout << endl;
    cout << "Minimum: " << findMinElement(matrix, SIZE) << endl;
    cout << "Maximum: " << findMaxElement(matrix, SIZE) << endl;
    return 0;

}

void fillAndShowArray(int matrix[][SIZE], const int size) {
    srand(time(0)); // Ustawienie ziarna losowości

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = 30 + rand() % 31; // Losowanie liczb z zakresu 0-99
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int findMinElement(int matrix[][SIZE], const int size) {
    int min = matrix[0][0];
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (matrix[i][j] < min) {
                min = matrix[i][j];
            }
        }
    }
    return min;
}

int findMaxElement(int matrix[][SIZE], const int size) {
    int max = matrix[0][0];
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (matrix[i][j] > max) {
                max = matrix[i][j];
            }
        }
    }
    return max;
}