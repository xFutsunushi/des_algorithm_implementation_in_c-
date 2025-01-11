#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int width = 20;
const int height = 20;

int x, y, fruitX, fruitY, score;
vector<pair<int, int>> snake;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

termios orig_termios;

// Włączenie trybu raw w terminalu
void enableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &orig_termios);
    term = orig_termios;
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Przywrócenie domyślnego trybu terminala
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

// Inicjalizacja gry
void setup() {
    dir = RIGHT; // Wąż zaczyna poruszać się w prawo
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    snake.clear();
    snake.push_back({x, y});
}

// Rysowanie planszy
void draw() {
    system("clear");
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";

            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else {
                bool isBody = false;
                for (auto segment : snake) {
                    if (segment.first == j && segment.second == i) {
                        cout << "o";
                        isBody = true;
                        break;
                    }
                }
                if (!isBody)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
}

// Obsługa klawiatury w czasie rzeczywistym
void input() {
    // Użycie funkcji non-blocking (peek)
    if (cin.peek() != EOF) {
        switch (cin.get()) {
            case 'w': if (dir != DOWN) dir = UP; break;
            case 's': if (dir != UP) dir = DOWN; break;
            case 'a': if (dir != RIGHT) dir = LEFT; break;
            case 'd': if (dir != LEFT) dir = RIGHT; break;
            case 'x': disableRawMode(); exit(0); // Wyjście z gry
        }
    }
}

// Logika gry
void logic() {
    int prevX = x;
    int prevY = y;

    switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
    }

    // Sprawdzanie kolizji z krawędziami planszy
    if (x >= width || x < 0 || y >= height || y < 0) {
        cout << "Game Over!" << endl;
        disableRawMode();
        exit(0);
    }

    // Sprawdzanie kolizji z ciałem węża
    for (size_t i = 1; i < snake.size(); i++) {
        if (snake[i].first == x && snake[i].second == y) {
            cout << "Game Over!" << endl;
            disableRawMode();
            exit(0);
        }
    }

    // Aktualizacja pozycji węża
    snake.insert(snake.begin(), {x, y});

    // Jeśli wąż zje owoc
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
    } else {
        snake.pop_back();
    }
}

int main() {
    srand(time(0)); // Inicjalizacja generatora liczb losowych
    enableRawMode(); // Włączenie trybu raw w terminalu
    setup();

    while (true) {
        draw();        // Rysowanie planszy
        input();       // Obsługa wejścia
        logic();       // Logika gry
        usleep(100000); // Opóźnienie 100ms
    }

    disableRawMode(); // Przywrócenie trybu normalnego terminala
    return 0;
}

