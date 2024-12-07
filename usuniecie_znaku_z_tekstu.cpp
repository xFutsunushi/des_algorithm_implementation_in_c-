#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    string text;
    char to_remove;

    cout << "Podaj tekst: ";
    getline(cin, text);
    cout << "Podaj znak do usunięcia: ";
    cin >> to_remove;

    // text.erase(remove(text.begin(), text.end(), to_remove), text.end());
    auto new_end = remove(text.begin(), text.end(), to_remove);
    text.erase(new_end, text.end()); // Usunięcie niepotrzebnych elementów

    cout << "Tekst po usunięciu znaku: " << text << endl;
    return 0;
}
