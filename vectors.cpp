#include <iostream>
#include <vector>

using namespace std;

int main() {

    vector<int> wholeNumbers;

    wholeNumbers.push_back(45); // dodawanie na koniec - tylko inty tak jak zainicjowaliśmy
    wholeNumbers.push_back(20);

    cout << wholeNumbers[0] << endl; 
    cout << wholeNumbers[1] << endl;

    

    return 0;

}