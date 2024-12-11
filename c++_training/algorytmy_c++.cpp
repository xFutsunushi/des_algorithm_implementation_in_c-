#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

typedef pair<int, int> pii;

// Sortowanie bąbelkowe (Bubble Sort)
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// int main_bubble() {
//     vector<int> arr = {64, 25, 12, 22, 11};
//     bubbleSort(arr);
//     cout << "Posortowana tablica: ";
//     for (int num : arr) {
//         cout << num << " ";
//     }
//     return 0;
// }

// // Wyszukiwanie binarne (Binary Search)
// int binarySearch(const vector<int>& arr, int target) {
//     int left = 0, right = arr.size() - 1;
//     while (left <= right) {
//         int mid = left + (right - left) / 2;
//         if (arr[mid] == target)
//             return mid;
//         else if (arr[mid] < target)
//             left = mid + 1;
//         else
//             right = mid - 1;
//     }
//     return -1;  // Element nie został znaleziony
// }

// int main_binary() {
//     vector<int> arr = {10, 20, 30, 40, 50, 60};
//     int target = 30;
//     int result = binarySearch(arr, target);
//     if (result != -1)
//         cout << "Element znaleziony na indeksie: " << result << endl;
//     else
//         cout << "Element nie został znaleziony" << endl;
//     return 0;
// }

// // Sortowanie przez wstawianie (Insertion Sort)
// void insertionSort(vector<int>& arr) {
//     int n = arr.size();
//     for (int i = 1; i < n; i++) {
//         int key = arr[i];
//         int j = i - 1;
//         while (j >= 0 && arr[j] > key) {
//             arr[j + 1] = arr[j];
//             j--;
//         }
//         arr[j + 1] = key;
//     }
// }

// int main_insertion() {
//     vector<int> arr = {12, 11, 13, 5, 6};
//     insertionSort(arr);
//     cout << "Posortowana tablica: ";
//     for (int num : arr) {
//         cout << num << " ";
//     }
//     return 0;
// }

// // Największy wspólny dzielnik (GCD) – Algorytm Euklidesa
// int gcd(int a, int b) {
//     if (b == 0)
//         return a;
//     return gcd(b, a % b);
// }

// int main_gcd() {
//     int a = 56, b = 98;
//     cout << "GCD(" << a << ", " << b << ") = " << gcd(a, b) << endl;
//     return 0;
// }



// // Szybkie potęgowanie (Exponentiation by Squaring)
// long long power(long long base, long long exp) {
//     if (exp == 0)
//         return 1;
//     long long half = power(base, exp / 2);
//     if (exp % 2 == 0)
//         return half * half;
//     else
//         return half * half * base;
// }

// int main_power() {
//     long long base = 2, exp = 10;
//     cout << base << "^" << exp << " = " << power(base, exp) << endl;
//     return 0;
// }