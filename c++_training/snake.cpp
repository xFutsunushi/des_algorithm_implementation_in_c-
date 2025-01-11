#include <iostream>
#include <unistd.h> // usleep()
#include <termios.h> // Obsługa klawiatury
#include <vector>
#include <cstdlib> // rand(), srand()
#include <ctime> // time()

using namespace std;

// Wymiary planszy
const int width = 20;
const int height = 20;

// Zmienne gry
int x, y, fruitX, fruitY, score;
vector<pair<int, int>> snake;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

// Funkcja do ustawienia trybu niestandardowego dla terminala
void enableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Funkcja do przywrócenia trybu domyślnego terminala
void disableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Funkcja do inicjalizacji gry
void setup() {
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    snake.clear();
    snake.push_back({x, y});
}

// Funkcja do rysowania planszy
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

// Funkcja do obsługi wejścia
void input() {
    if (cin.peek() != EOF) {
        switch (cin.get()) {
            case 'w': dir = UP; break;
            case 's': dir = DOWN; break;
            case 'a': dir = LEFT; break;
            case 'd': dir = RIGHT; break;
            case 'x': exit(0); // Wyjście z gry
        }
    }
}

// Funkcja do logiki gry
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
    enableRawMode(); // Włączenie trybu surowego terminala
    setup();

    while (true) {
        draw();
        input();
        logic();
        usleep(100000); // Opóźnienie 100ms
    }

    disableRawMode(); // Przywrócenie trybu normalnego terminala
    return 0;
}

