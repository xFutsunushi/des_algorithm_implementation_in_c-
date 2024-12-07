// #include <iostream>
// #include <string>
// #include <cstring>
// using namespace std;

// int main() {
//     const int rozmiar = 50;
//     char text[rozmiar];

//     //Wypisujemy ciag
//     cout << "Podaj text: ";
//     cin.getline(text, rozmiar);
//     //Liczenie liczby samoglosek
//     int liczbaSamoglosek;

//     char substring[10] = {'a', 'e', 'i', 'o', 'u', 'y'};
//     string text_find = text;

//     for ( int i = 0; text[i] != '\0'; ++i) {
//         if ( text.find(substring) != string::npos );
//         cout << substring; 
//         //liczbaSamoglosek++;
//         //cout << "Znaleziono samogloske" << endl; 

//     }

//     cout << liczbaSamoglosek << endl;

//     return 0;
// }

#include <set>
#include <string>
#include <iostream>

int main()
{   
    std::string x;
    int n = 0;
    std::set<char> vowels;

    std::cout << "Enter any string\n";
    std::cin >> x;
    n = x.size();

    for (int i = 0; i < n; ++i)
        if (x[i] == 'a' || x[i] == 'e' || x[i] == 'i' || x[i] == 'o' || x[i] == 'u')
           vowels.insert(x[i]);

    std::cout << vowels.size() <<'\n';
}
// DO POPRAWKI !!!!