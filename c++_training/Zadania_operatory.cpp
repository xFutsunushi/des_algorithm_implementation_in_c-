// #include <iostream>

// using namespace std;

// int main() {
//     int x = 10;

//     while (x > 0) {
//         cout << x << endl;
//         x -= 3;
//     }


//     return 0;
// }

#include <iostream>
using namespace std;

int main() {
    int x = 2, y = 5, z = 3;
    int result = ++x * y-- - z++ + (x +y);
    cout << result << " " << x << " " << y << " " << z;

    return 0;
}

// Przećwiczyć