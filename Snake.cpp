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

vector<Coordenada> serpiente, serpiente2;
Coordenada comida;
bool comida_generada = false;
int puntuacion = 0, puntuacion2 = 0;

const int WIDTH = 50;
const int HEIGHT = 25;

// Definición de direcciones
const int ARRIBA = 0;
const int ABAJO = 1;
const int IZQUIERDA = 2;
const int DERECHA = 3;
const int SIN_MOVIMIENTO = -1;  // Estado inicial sin movimiento

void Bienvenida() {
    cout << "------------------------------------------------" << endl;
    cout << "              WELCOME TO SNAKE GAME             " << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Seleccione su modo de juego" << endl;
    cout << "1. Un Jugador" << endl;
    cout << "2. Dos Jugadores" << endl;
    cin >> modo_juego;
    
    if (modo_juego != 1 && modo_juego != 2) {
        cout << "Seleccione una de las dos opciones disponibles." << endl;
        system("pause");
        system("cls");
        Bienvenida();
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

void Dibuja_Serpiente(const vector<Coordenada>& serp, char cabeza, char cuerpo) {
    for (size_t i = 0; i < serp.size(); i++) {
        gotoxy(serp[i].x, serp[i].y);
        if (i == 0) {
            cout << cabeza;
        } else {
            cout << cuerpo;
        }
    }
}

void MostrarPuntuacion() {
    gotoxy(0, HEIGHT);
    cout << "Puntaje Jugador 1: " << puntuacion << " | Puntaje Jugador 2: " << puntuacion2 << endl;
}

void Movimiento_Snake(vector<Coordenada>& serp, int tecla_arriba, int tecla_abajo, int tecla_izquierda, int tecla_derecha, int& puntaje, bool is_arrow_keys = false) {
    int direccion = SIN_MOVIMIENTO;  // Inicialmente sin movimiento
    Coordenada inicio = { WIDTH / 2, HEIGHT / 2 };
    serp.push_back(inicio);

    while (true) {
        mtx.lock();
        system("cls");
        Limites();
        Dibuja_Serpiente(serpiente, 'O', 'o');
        if (modo_juego == 2) {
            Dibuja_Serpiente(serpiente2, 'X', 'x');
        }
        gotoxy(comida.x, comida.y);
        cout << "@";  
        MostrarPuntuacion();
        mtx.unlock();

        if (_kbhit()) {
            int tecla = _getch();

            if (tecla == 0 || tecla == 224) {
                tecla = _getch();  // Leer tecla de dirección o especial
            }

            mtx.lock();

            // Cambiar la dirección dependiendo la flecha que se presione
            if (tecla == tecla_arriba && direccion != ABAJO) {
                direccion = ARRIBA;
            } else if (tecla == tecla_abajo && direccion != ARRIBA) {
                direccion = ABAJO;
            } else if (tecla == tecla_izquierda && direccion != DERECHA) {
                direccion = IZQUIERDA;
            } else if (tecla == tecla_derecha && direccion != IZQUIERDA) {
                direccion = DERECHA;
            }

            mtx.unlock();
        }

        // Si la dirección es SIN_MOVIMIENTO, no se mueve la serpiente 
        if (direccion == SIN_MOVIMIENTO) {
            Sleep(100);  // Pausar el ciclo 
            continue;
        }

        mtx.lock();

        // Mover la serpiente en la dirección actual
        Coordenada nueva_pos = serp[0];

        switch (direccion) {
            case ARRIBA:
                nueva_pos.y--;
                break;
            case ABAJO:
                nueva_pos.y++;
                break;
            case IZQUIERDA:
                nueva_pos.x--;
                break;
            case DERECHA:
                nueva_pos.x++;
                break;
        }

        // Chequear colisión con los bordes
        if (nueva_pos.x == 0 || nueva_pos.x == WIDTH - 1 || nueva_pos.y == 0 || nueva_pos.y == HEIGHT - 1) {
            GameOver();
            break;
        }

        // Chequear si come comida
        if (nueva_pos.x == comida.x && nueva_pos.y == comida.y) {
            serp.push_back(serp.back());  // Crece la serpiente
            puntaje += 7;  
            comida_generada = false;  
        }

        // Chequear colisión con su propio cuerpo
        for (size_t i = 1; i < serp.size(); i++) {
            if (nueva_pos.x == serp[i].x && nueva_pos.y == serp[i].y) {
                GameOver();
                return;
            }
        }

        // Mover el cuerpo de la serpiente
        for (size_t i = serp.size() - 1; i > 0; i--) {
            serp[i] = serp[i - 1];
        }

        serp[0] = nueva_pos;
        mtx.unlock();

        Sleep(100);  // Velocidad de la serpiente
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

void* Temporizador(void*) {
    int tiempo_total = 60;
    while (tiempo_total > 0) {
        Sleep(1000);
        mtx.lock();
        gotoxy(WIDTH + 5, 0);
        cout << "Tiempo restante: " << tiempo_total << " segundos";
        tiempo_total--;
        mtx.unlock();
    }
    GameOver();
    return nullptr;
}

int main() {
    Bienvenida();

    thread limites(Limites);
    limites.join();

    thread comida_thread(Comida, nullptr);
    thread temporizador(Temporizador, nullptr);

    thread snake1(Movimiento_Snake, ref(serpiente), 'w', 's', 'a', 'd', ref(puntuacion), false);
    thread snake2;

    if (modo_juego == 2) {
        snake2 = thread(Movimiento_Snake, ref(serpiente2), 72, 80, 75, 77, ref(puntuacion2), true);
    }

    snake1.join();
    if (modo_juego == 2) {
        snake2.join();
    }

    comida_thread.join();
    temporizador.join();

    return 0;
}
