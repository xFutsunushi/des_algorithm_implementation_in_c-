#include <iostream>
#include <exception>

using namespace std;

int main(){

    const int floorNumber = 10;
    const int parkingNumber = 10;
    int parkingArray[floorNumber][parkingNumber] = {};

    int floor = 0;
    int parking = 0;
    char exit  = '1'; 

    for ( int f = 0; f < floorNumber; f++ ){

        cout << f + 1 << " Piętro: ";
        for ( int p = 0; p < parkingNumber; p++){
            parkingArray[f][p] = p + 1;
            cout << parkingArray[f][p] << " | "; 
        }
        cout << endl << "-------------------------------------------------" << endl; 
    }

    cout << "To reserve parking place enter floor and parking number: " << endl; 

    do {
        do {
            // try {
            // throw exception; // Throw an exception when a problem arise
            // }
            // catch () {
            // // Block of code to handle errors
            // }
            cout << "Enter floor number: " << endl;
            cin >> floor;
            if ( floor < 1 || floor > 10 ){
                cout << "That floor does'nt exists!!! ";
            } 
        } while ( floor < 1 || floor > 10 );

        do {
            cout << "Enter parking number" << endl;
            cin >> parking;
            if ( parking < 1 || parking > 10 ){
                cout << "This number does'nt exists!!!";
            }

        } while ( parking < 1 || parking > 10 );

        if (parkingArray[floor - 1][parking - 1] != 0 ){
            parkingArray[floor -1][parking -1] = 0;
            cout << "Congrats! Place is reserved!" << endl;
            cout << "Enter the 1 to close the program or 2 to try another reservation: ";
            cin >> exit;
        }

        else{
            cout << "ERROR! The place just reserved!!! " << endl;

            cout << "Reservation table: " << endl  << endl;

            for ( int f = 0; f < floorNumber; f++)
            {
                cout << f + 1 << " Parking: ";
                for ( int j = 0; j < parkingNumber; j++)
                {
                    cout << parkingArray[f][j] << " | ";
            }
            cout << endl << "--------------------------------------------------" << endl;
        }
        return 1;
    }

} while (exit != '1');

}