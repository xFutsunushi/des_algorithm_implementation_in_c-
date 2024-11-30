#include <iostream>
using namespace std;

int main() {
    char operacja;
    double num1, num2;

    cout << "Podaj operację (+, -, *, /): ";
    cin >> operacja;
    cout << "Podaj dwie liczby: ";
    cin >> num1 >> num2;

    switch (operacja) {
        case '+':
            cout << "Result: " << num1 + num2 << endl;
            break;
        case '-':
            cout << "Result: " << num1 - num2 << endl;
            break;
        case '*':
            cout << "Result: " << num1 * num2 << endl;
            break;
        case '/':
            if (num2 != 0)
                cout << "Result: " << num1 / num2 << endl;
            else
                cout << "ERROR: division by zero!" << endl;
            break;
        default:
            cout << "ERROR: bad operation" << endl;
            break;
    }

    return 0;
}
