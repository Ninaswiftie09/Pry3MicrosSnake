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
int nivel = 1;  // Nivel de dificultad inicial
int velocidad = 100;  // velocidad inicial
mutex mtx;

struct Coordenada {
    int x, y;
};

vector<Coordenada> serpiente, serpiente2;
Coordenada comida;
vector<Coordenada> trampas;  // Vector para almacenar las posiciones de las trampas
bool comida_generada = false;
int puntuacion = 0, puntuacion2 = 0;

const int WIDTH = 50;
const int HEIGHT = 25;

const int ARRIBA = 0;
const int ABAJO = 1;
const int IZQUIERDA = 2;
const int DERECHA = 3;
const int SIN_MOVIMIENTO = -1;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

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
    cout << "Instrucciones del Juego" << endl;
     cout << "Modo de un jugador" << endl;
        cout << "Utiliza las teclas WASD para moverte:" << endl;
        cout << "W: Arriba" << endl;
        cout << "A: Izquierda" << endl;
        cout << "S: Abajo" << endl;
        cout << "D: Derecha" << endl;
        cout << "" << endl;

        cout << "Modo de dos jugadores " << endl;
        cout << "Jugador 1: Utiliza las teclas WASD" << endl;
        cout << "Jugador 2: Utiliza las teclas de flechas de direccion " << endl;
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
    // Dibuja los límites una sola vez
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            gotoxy(j, i);
            if (i == 0 || i == HEIGHT - 1) {
                cout << "-";
            } else if (j == 0 || j == WIDTH - 1) {
                cout << "|";
            }
        }
    }
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
    cout << "!" << endl;
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
    gotoxy(0, HEIGHT + 1);
    cout << "Nivel: " << nivel << " | Velocidad: " << velocidad << " ms" << endl;
}

void Dibuja_Trampas() {
    for (const auto& trampa : trampas) {
        gotoxy(trampa.x, trampa.y);
        cout << "X";  // Símbolo de las trampas
    }
}

void Borrar_Trampas() {
    for (const auto& trampa : trampas) {
        gotoxy(trampa.x, trampa.y);
        cout << " ";  // Borrar la trampa 
    }
}

void GenerarTrampas(int cantidad) {
    trampas.clear();  // Limpiar trampas anteriores
    for (int i = 0; i < cantidad; i++) {
        Coordenada nueva_trampa;
        nueva_trampa.x = rand() % (WIDTH - 2) + 1;
        nueva_trampa.y = rand() % (HEIGHT - 2) + 1;
        trampas.push_back(nueva_trampa);
    }
}

bool ColisionaConTrampa(const Coordenada& posicion) {
    for (const auto& trampa : trampas) {
        if (posicion.x == trampa.x && posicion.y == trampa.y) {
            return true;
        }
    }
    return false;
}

void Movimiento_Snake(vector<Coordenada>& serp, int tecla_arriba, int tecla_abajo, int tecla_izquierda, int tecla_derecha, int& puntaje, bool is_arrow_keys = false) {
    int direccion = SIN_MOVIMIENTO;
    Coordenada inicio = { WIDTH / 2, HEIGHT / 2 };
    serp.push_back(inicio);

    while (true) {
        if (_kbhit()) {
            int tecla = _getch();

            if (tecla == 0 || tecla == 224) {
                tecla = _getch();
            }

            mtx.lock();
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

        if (direccion == SIN_MOVIMIENTO) {
            Sleep(100);
            continue;
        }

        mtx.lock();
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

        // Verificar colisión con los límites
        if (nueva_pos.x == 0 || nueva_pos.x == WIDTH - 1 || nueva_pos.y == 0 || nueva_pos.y == HEIGHT - 1 || ColisionaConTrampa(nueva_pos)) {
            GameOver();
            break;
        }

        if (nueva_pos.x == comida.x && nueva_pos.y == comida.y) {
            serp.push_back(serp.back());
            puntaje += 7;
            comida_generada = false;
        }

        for (size_t i = 1; i < serp.size(); i++) {
            if (nueva_pos.x == serp[i].x && nueva_pos.y == serp[i].y) {
                GameOver();
                return;
            }
        }

        // Borrar la última parte de la serpiente
        gotoxy(serp.back().x, serp.back().y);
        cout << " ";

        for (size_t i = serp.size() - 1; i > 0; i--) {
            serp[i] = serp[i - 1];
        }

        serp[0] = nueva_pos;

        // Dibujar la nueva posición de la serpiente
        gotoxy(serp[0].x, serp[0].y);
        cout << "O";  // Dibujar cabeza
        for (size_t i = 1; i < serp.size(); i++) {
            gotoxy(serp[i].x, serp[i].y);
            cout << "o";  // Dibujar cuerpo
        }

        if (comida_generada) {
            gotoxy(comida.x, comida.y);
            cout << "@";
        }

        MostrarPuntuacion();
        Dibuja_Trampas();
        mtx.unlock();

        Sleep(velocidad);
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
    int tiempo_total = 40;  // Tiempo por nivel en segundos
    while (true) {
        while (tiempo_total > 0) {
            Sleep(1000);
            mtx.lock();
            gotoxy(WIDTH + 5, 0);
            cout << "Tiempo restante: " << tiempo_total;
            tiempo_total--;
            mtx.unlock();
        }

        // Aumentar el nivel cada vez que se agota el tiempo
        nivel++;
        velocidad -= 10;  // Aumentar velocidad
        tiempo_total = 40;  // Reiniciar tiempo

        // Generar trampas a partir del nivel 3
        if (nivel >= 2) {
            int cantidad_trampas = nivel - 2;  // Aumentar la cantidad de trampas con el nivel
            GenerarTrampas(cantidad_trampas);
        }
    }
}

// Función para regenerar trampas cada 3 segundos
void* RegenerarTrampas(void*) {
    while (true) {
        Sleep(3000);  // Espera 3 segundos
        mtx.lock();
        Borrar_Trampas();  // Borrar trampas actuales
        GenerarTrampas(nivel - 2);  // Generar nuevas trampas según el nivel
        mtx.unlock();
    }
}

int main() {
    Bienvenida();
    Limites();

    thread comida_thread(Comida, nullptr);
    thread temporizador_thread(Temporizador, nullptr);
    thread trampas_thread(RegenerarTrampas, nullptr);  // Hilo para regenerar trampas

 
    thread snake1;
    thread snake2;

    if (modo_juego == 1) {
        snake1 = thread(Movimiento_Snake, ref(serpiente), 'w', 's', 'a', 'd', ref(puntuacion), false);
    } else if (modo_juego == 2) {
        snake1 = thread(Movimiento_Snake, ref(serpiente), 'w', 's', 'a', 'd', ref(puntuacion), false);
        snake2 = thread(Movimiento_Snake, ref(serpiente2), 72, 80, 75, 77, ref(puntuacion2), true);
    }

    if (snake1.joinable()) {
        snake1.join();
    }
    if (snake2.joinable()) {
        snake2.join();
    }
    comida_thread.join();
    temporizador_thread.join();
    trampas_thread.join();
    return 0;
}
