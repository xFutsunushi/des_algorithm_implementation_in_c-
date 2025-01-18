#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <cstdlib>

using namespace std;

const int w = 20, h = 10;
int plansza[h][w] = {0};
int x = w / 2, y = h / 2, fx = rand() % w, fy = rand() % h, score = 0;
char dir = 'd';


/* Zmieniamy ustawienia terminala na tryb niekanoniczny żeby czytać znaki od razu z terminala*/
/*https://www.bezkompilatora.pl/jak-dziala-linuxowy-terminal-czesc-2/*/
void setupTerminal(bool enable) {
    static struct termios oldt, newt; // Przechowuje bieżące i nowe ustawienia terminala.
    if (enable) { 
        tcgetattr(STDIN_FILENO, &oldt); // Pobiera aktualne ustawienia terminala i zapisuje je w `oldt`.
        newt = oldt; // Kopiuje aktualne ustawienia do `newt`, aby je zmodyfikować.
        newt.c_lflag &= ~(ICANON | ECHO); // Wyłącza tryb kanoniczny (ICANON) i echo znaków (ECHO).
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Zapisuje zmodyfikowane ustawienia terminala natychmiast (TCSANOW).
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Przywraca oryginalne ustawienia terminala.
    }
}

bool kbhit() {
    fd_set set; // Tworzy zestaw deskryptorów plików (dla monitorowania wejścia).
    struct timeval timeout; // Struktura przechowująca czas oczekiwania.
    FD_ZERO(&set); // Inicjalizuje zestaw deskryptorów jako pusty.
    FD_SET(STDIN_FILENO, &set); // Dodaje deskryptor wejścia standardowego (klawiatury) do zestawu.
    timeout.tv_sec = 0; // Ustawia czas oczekiwania na 0 sekund.
    timeout.tv_usec = 0; // Ustawia czas oczekiwania na 0 mikrosekund (brak opóźnienia).
    return select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0; 
    // Wywołuje `select`, aby sprawdzić, czy są dostępne dane na wejściu standardowym.
    // Zwraca true, jeśli klawisz został naciśnięty (dane są gotowe do odczytu).
}

int main() {
    setupTerminal(true);
    plansza[y][x] = 1;
    while (true) {
        system("clear");
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (i == y && j == x) cout << 'O';
                else if (i == fy && j == fx) cout << 'F';
                else cout << (plansza[i][j] > 0 ? 'o' : '.');
            }
            cout << endl;
        }
        // czytam wejście z klawiatury - kazde nacisniecie 
        if (kbhit()) {
            char input;
            read(STDIN_FILENO, &input, 1);
            if (input == 'w' || input == 'a' || input == 's' || input == 'd') dir = input;
        }

        int prevX = x, prevY = y;
        switch (dir) {
            case 'w': y--; break;
            case 's': y++; break;
            case 'a': x--; break;
            case 'd': x++; break;
        }
        // Sprawdzamy czy glowa weza znajduje sie w tej samej pozycji co litera F 
        // jesli tak, generujemy nowa losowa pozycje F i dodajemy punkt do wyniku 
        if (x == fx && y == fy) {
            fx = rand() % w;
            fy = rand() % h;
            score++;
        } else {
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    if (plansza[i][j] > 0) plansza[i][j]--;
                }
            }
        }

        if (x < 0 || x >= w || y < 0 || y >= h || plansza[y][x] > 0) break;

        plansza[y][x] = score + 1;
        usleep(100000); // Predkosc gry - 100ms jest za szybko 
    }

    setupTerminal(false); // Przywracamy tryb kanoniczny 
    cout << "Game Over! Your score: " << score << endl;
    return 0;
}

