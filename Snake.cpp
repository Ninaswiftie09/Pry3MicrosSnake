#include <iostream>
#include <thread>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

void Bienvenida() {
    cout << "------------------------------------------------" << endl;
    cout << "              WELCOME TO SNAKE GAME             " << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Presione cualquier tecla para continuar..." << endl;
    _getch();
    system("cls");
}

void Limites() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1) {
                cout << "-";
            } else if (j == 0 || j == width - 1) {
                cout << "|";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void GameOver() {
    system("cls");
    cout << "   ____      _      __  __  U _____ u       U  ___ u__     __ U _____ u   ____     \n";
    cout << "U /\"___|uU  /\"\\  uU|' \\/ '|u\\| ___\"|/        \\/\"_ \\/\\ \\   /\"/u\\| ___\"|/U |  _\"\\ u  \n";
    cout << "\\| |  _ / \\/ _ \\/ \\| |\\/| |/ |  _|\"          | | | | \\ \\ / //  |  _|\"   \\| |_) |/  \n";
    cout << " | |_| |  / ___ \\  | |  | |  | |___      .-,_| |_| | /\\ V /_,-.| |___    |  _ <    \n";
    cout << "  \\____| /_/   \\_\\ |_|  |_|  |_____|      \\_)-\\___/ U  \\_/-(_/ |_____|   |_| \\_\\   \n";
    cout << "  _)(|_   \\\\    >><<,-,,-.   <<   >>           \\\\     //       <<   >>   //   \\\\_  \n";
    cout << " (__)__) (__)  (__)(./  \\.) (__) (__)         (__)   (__)     (__) (__) (__)  (__) \n";
}

void Movimiento_Snake() {
    char tecla;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    srand(time(0));
    int x = rand() % (width - 2) + 1;
    int y = rand() % (height - 2) + 1;

    gotoxy(x, y);
    cout << "o";

    while (true) {
        if (_kbhit()) {
            tecla = _getch();
            gotoxy(x, y);
            cout << " ";

            if (tecla == 'w') y--;
            else if (tecla == 's') y++;
            else if (tecla == 'a') x--;
            else if (tecla == 'd') x++;

            
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                GameOver();
                break;
            }

            gotoxy(x, y);
            cout << "o";
        }
        Sleep(100);
    }
}

int main() {
    thread bienvenida(Bienvenida);
    bienvenida.join();

    thread limites(Limites);
    limites.join();

    thread snake(Movimiento_Snake);
    snake.join();

    return 0;
}
