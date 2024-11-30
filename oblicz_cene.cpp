#include <iostream>
using namespace std;

int main (){

double cena = 500;
double promocja = 50;
double podatek = 0.23;

cout << "Dodaje rabat na produkt: ";
cena *= 0.85;

cout << "Cena produktu po rabacie 15%: " << cena << endl;

cout << "Dodaje podatek VAT do produktu: " << endl; 

cena = cena * 1.23 - 50 ;

cout << "Cena produktu po dodaniu podatku vat i rabatu 50zl: " << cena << endl; 


}