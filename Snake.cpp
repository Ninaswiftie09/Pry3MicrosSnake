// PROYECTO 3, PROGRAMACIÓN DE MICROPROCESADORES
// NINA NÁJERA - 231088
// WILSON CALDERÓN - 22018
// MISHELL CIPRIAN - 231169

#include <iostream>
#include <thread>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <mutex>

using namespace std;

int modo_juego;  
mutex mtx;  

struct Coordenada {
    int x, y;
};

vector<Coordenada> serpiente;  
Coordenada comida;  
bool comida_generada = false;  
int puntuacion = 0;

const int WIDTH = 50;
const int HEIGHT = 25;

void Bienvenida() {
    cout << "------------------------------------------------" << endl;
    cout << "              WELCOME TO SNAKE GAME             " << endl;
    cout << "------------------------------------------------" << endl;
    cout << "           /\\/\\/\\                              " << endl;
    cout << "         _|__|  O|                             " << endl;
    cout << "/~     /~     \\_/ \\                           " << endl;
    cout << " \\____|__________/  \\                          " << endl;
    cout << "        \\_______      \\                        " << endl;
    cout << "                \\     \\                 \\     " << endl;
    cout << "                  |     |                  \\   " << endl;
    cout << "                 /      /                    \\ " << endl;
    cout << "                /     /                       \\" << endl;
    cout << "              /      /                         \\" << endl;
    cout << "             /     /                            \\" << endl;
    cout << "           /     /             _----_            " << endl;
    cout << "          /     /           _-~      ~-_         " << endl;
    cout << "         (      (        _-~    _--_    ~-_     _/" << endl;
    cout << "          \\      ~-____-~    _-~    ~-_    ~-_-~ " << endl;
    cout << "            ~-_           _-~          ~-_       " << endl;
    cout << "               ~--______-~                ~-___-~" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Seleccione su modo de juego" << endl;
    cout << "1. Un Jugador" << endl;
    cout << "2. Dos Jugadores" << endl;
    cin >> modo_juego;
    
    switch (modo_juego) {
        case 1:
            break;
        case 2:
            cout <<" Opcion aun no disponible (Wilson/Mishell)" << endl;
            system("pause");
            system("cls");
            Bienvenida();
            break;
        default:
            cout << "Seleccione una de las dos opciones disponibles." << endl;
            system("pause");
            system("cls");
            Bienvenida();
            break;
    }
    
    system("cls");
}

void Limites() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1) {
                cout << "-";
            } else if (j == 0 || j == WIDTH - 1) {
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

void Dibuja_Serpiente() {
    for (size_t i = 0; i < serpiente.size(); i++) {
        gotoxy(serpiente[i].x, serpiente[i].y);
        if (i == 0) {
            cout << "O";  
        } else {
            cout << "o";  
        }
    }
}

void MostrarPuntuacion() {
    gotoxy(0, HEIGHT);
    cout << "Puntaje: " << puntuacion << endl;  
}

void Movimiento_Snake() {
    char tecla;

    Coordenada inicio = { WIDTH / 2, HEIGHT / 2 };
    serpiente.push_back(inicio);

    while (true) {
        mtx.lock();
        system("cls");
        Limites();
        Dibuja_Serpiente();
        gotoxy(comida.x, comida.y);
        cout << "@";  
        MostrarPuntuacion();
        mtx.unlock();

        if (_kbhit()) {
            tecla = _getch();
            mtx.lock();

            Coordenada nueva_pos = serpiente[0];

            if (tecla == 'w') nueva_pos.y--;
            else if (tecla == 's') nueva_pos.y++;
            else if (tecla == 'a') nueva_pos.x--;
            else if (tecla == 'd') nueva_pos.x++;

            if (nueva_pos.x == 0 || nueva_pos.x == WIDTH - 1 || nueva_pos.y == 0 || nueva_pos.y == HEIGHT - 1) {
                GameOver();
                break;
            }

            if (nueva_pos.x == comida.x && nueva_pos.y == comida.y) {
                serpiente.push_back(serpiente.back());  
                puntuacion += 10;  
                comida_generada = false;  
            }

            for (size_t i = 1; i < serpiente.size(); i++) {
                if (nueva_pos.x == serpiente[i].x && nueva_pos.y == serpiente[i].y) {
                    GameOver();
                    return;
                }
            }

            for (size_t i = serpiente.size() - 1; i > 0; i--) {
                serpiente[i] = serpiente[i - 1];
            }

            serpiente[0] = nueva_pos;
            mtx.unlock();
        }

        Sleep(100);  
    }
}

void* Comida(void*) {
    srand(time(0));

    while (true) {
        if (!comida_generada) {  
            mtx.lock();
            comida.x = rand() % (WIDTH - 2) + 1;
            comida.y = rand() % (HEIGHT - 2) + 1;
            comida_generada = true;  
            mtx.unlock();
        }
        Sleep(5000);  
    }
}

int main() {
    Bienvenida();

    thread limites(Limites);
    limites.join();

    thread comida_thread(Comida, nullptr); 
    thread snake(Movimiento_Snake);

    comida_thread.join();
    snake.join();

    return 0;
}
