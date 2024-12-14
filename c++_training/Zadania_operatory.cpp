#include <iostream>

using namespace std;

int main() {
    int x = 20, y = 6, z = 2;
    int result = x / y * z + x % y - z * ( y / z );
    cout << result << endl;

    return 0;
}
