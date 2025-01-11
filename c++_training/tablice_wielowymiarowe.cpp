#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

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


// int main (){

//     int N;
//     cout << "Podaj rozmiar tablicy: ";
//     cin >> N;

//     int value = 1;

//     for ( int i = 1; i < N; i++ ){
//         for ( int j = 0; j < N; j++ ){
//             cout << value++ << "\t";
//         }
//         cout << endl;
//    }
// }

// int main (){

//     const int rows = 9, cols = 9;
//     int ourMatrix[rows][cols]  = {};
//     for ( int i = 1; i <= rows; i++) {
//         for ( int j = 1; j <= cols; j++ ) {
//             ourMatrix[i-1][j-1] = i * j;
//             cout << "\t" << ourMatrix[i-1][j-1];
//         }
//     cout << endl;
//     }
// }

// int main (){

//     const int MatrixSize = 3;
//     int ourMatrix[MatrixSize][MatrixSize] = {};
//     int rowSum[MatrixSize] = {};
//     int columnSum[MatrixSize] = {};

//     cout << "Wypełnij macierz " << MatrixSize << 'x' << MatrixSize << " liczbami wiersz po wierszu\n";

//     for ( int i = 0; i < MatrixSize; i++ ) {
//         for ( int j = 0; j < MatrixSize; j++ ) {
//                 cout << "Podaj element [" << i << "][" << j << "]: " ;
//                 cin >> ourMatrix[i][j];
//         }
//     }


// for ( int rowNum = 0; rowNum < MatrixSize; rowNum++){
//     cout << " | ";
//     for ( int columnNum = 0; columnNum < MatrixSize; columnNum++)
//     {
//         cout << setw(4) << ourMatrix[rowNum][columnNum] << " " ;
//         rowSum[rowNum] += ourMatrix[rowNum][columnNum];
//         columnSum[rowNum] += ourMatrix[columnNum][rowNum];
//     }
//     cout << " | " << endl;
//     }

// cout << "\n Suma po wierszach:  \n";
// for ( int i = 0; i < MatrixSize; i++ ){
//     cout << i + 1 << " -j wiersz: " << rowSum[i] << endl; 
// }

// cout << "\nSuma po kolumnach: \n";  
// for ( int i = 0; i < MatrixSize; i++ ){
//     cout << i + 1 << " -a wiersz: " << columnSum[i] << endl; 
// }

//     return 0;
// }

int main(){
    const int matrixSize = 7;
    int ourMatrix[matrixSize][matrixSize] = {};
    int rowSum[matrixSize] = {};

    int numMaxRow = 0;

    srand(time(NULL));

    for (int rowNum = 0; rowNum < matrixSize; rowNum++){
        cout << " | ";
        for (int columnNum = 0; columnNum < matrixSize; columnNum++){
            ourMatrix[rowNum][columnNum] = 0 + rand() % 101;
            cout << setw(4) << ourMatrix[rowNum][columnNum] << " ";
        }
        cout << " | " << endl; 
    }
    for ( int rowNum = 0; rowNum < matrixSize; rowNum++){
        for (int columnNum = 0; columnNum < matrixSize; columnNum++ ){
            rowSum[rowNum] += ourMatrix[rowNum][columnNum];
        }
    }

    cout << "\n Suma według wierszy: \n";
    for ( int i = 0 ; i < matrixSize; i++ ){
        cout << i + 1 << " po wierszach: " << rowSum[i] << endl;
    }

    for (int i = 1; i < matrixSize; i++){
        if (rowSum[i] > rowSum[numMaxRow]){
            numMaxRow = i;
        }
    }

    cout << "Wiersz z maksymalna wartoscia sumy: " << numMaxRow +1;
    cout << endl << endl;
}
