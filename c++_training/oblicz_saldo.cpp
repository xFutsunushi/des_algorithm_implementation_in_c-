#include <iostream>
using namespace std;

int main (){

double saldo = 1000;
double procent = 0.5;
double oplata = 50 ;

cout << "Dodanie 'procent' odsetek do salda" << endl;

double oprocentowanie = (saldo * procent) / 100.0;
cout << "Saldo konta: " << saldo + oprocentowanie << endl; 

cout << "Pobieram opłatę za prowadzenie konta";
saldo = saldo - oplata;



}