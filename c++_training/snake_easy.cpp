#include <iostream>
#include <conio.h>
#include <vector>
#include <windows.h>

using namespace std;

int main() {
    int w = 20, h = 10, x = w / 2, y = h / 2, fx = rand() % w, fy = rand() % h, score = 0;
    vector<pair<int, int>> snake = {{x, y}};
    char dir = 'd', input;

    while (true) {
        system("cls");
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (i == y && j == x) cout << 'O';
                else if (i == fy && j == fx) cout << 'F';
                else {
                    bool body = false;
                    for (auto s : snake) if (s.first == j && s.second == i) body = true;
                    cout << (body ? 'o' : '.');
                }
            }
            cout << endl;
        }

        if (_kbhit()) {
            input = _getch();
            if (input == 'w' || input == 'a' || input == 's' || input == 'd') dir = input;
        }

        switch (dir) {
            case 'w': y--; break;
            case 's': y++; break;
            case 'a': x--; break;
            case 'd': x++; break;
        }

        if (x == fx && y == fy) {
            fx = rand() % w;
            fy = rand() % h;
            score++;
        } else snake.pop_back();

        snake.insert(snake.begin(), {x, y});
        if (x < 0 || x >= w || y < 0 || y >= h) break; // Koniec gry na kolizji ze ścianą
    }

    cout << "Game Over! Your score: " << score << endl;
    return 0;
}