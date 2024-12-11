// #include <iostream>
// using namespace std;

// int main() {
//     for (int i = 1;  i < 300; i++) {
//         if ( i <= 100 )
//         continue;  
//         cout << i << " ";
//     }
//     return 0;
// }

// ----------------------------------------------------------------------------------------------------

#include <iostream>
using namespace std;

int main() {
    for (int i = 0; i <= 20; i++){
        if (i % 2 == 0) { 
            cout << "Liczba parzysta: " << i << endl;
        }
    }
    return 0;
}