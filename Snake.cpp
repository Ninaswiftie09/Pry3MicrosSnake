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
int direccion_snake1 = 0; 
int direccion_snake2 = 0; 

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
    exit(0);
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

void Movimiento_Snake(vector<Coordenada>& serp, int& direccion, int tecla_arriba, int tecla_abajo, int tecla_izquierda, int tecla_derecha, int& puntaje) {
    Coordenada inicio = { WIDTH / 2, HEIGHT / 2 };
    serp.push_back(inicio);

    while (true) {
        mtx.lock();
        system("cls");
        Limites();
        Dibuja_Serpiente(serp, 'O', 'o');  
        gotoxy(comida.x, comida.y);
        cout << "@";  
        MostrarPuntuacion();
        mtx.unlock();

        Coordenada nueva_pos = serp[0];
        if (direccion == 0) nueva_pos.y--; 
        else if (direccion == 1) nueva_pos.y++; 
        else if (direccion == 2) nueva_pos.x--; 
        else if (direccion == 3) nueva_pos.x++; 

        if (nueva_pos.x == 0 || nueva_pos.x == WIDTH - 1 || nueva_pos.y == 0 || nueva_pos.y == HEIGHT - 1) {
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

    
        for (size_t i = serp.size() - 1; i > 0; i--) {
            serp[i] = serp[i - 1];
        }

        serp[0] = nueva_pos;

        Sleep(100);  
    }
}

void Movimiento_Snake2(vector<Coordenada>& serp, int& direccion, int& puntaje) {
    Coordenada inicio = { WIDTH / 2 - 1, HEIGHT / 2 };
    serp.push_back(inicio);

    while (true) {
        mtx.lock();
        system("cls");
        Limites();
        Dibuja_Serpiente(serp, 'X', 'x');  
        gotoxy(comida.x, comida.y);
        cout << "@";  
        MostrarPuntuacion();
        mtx.unlock();

        
        Coordenada nueva_pos = serp[0];
        
        if (direccion == 0) nueva_pos.y--; 
        else if (direccion == 1) nueva_pos.y++; 
        else if (direccion == 2) nueva_pos.x--; 
        else if (direccion == 3) nueva_pos.x++; 

        if (nueva_pos.x == 0 || nueva_pos.x == WIDTH - 1 || nueva_pos.y == 0 || nueva_pos.y == HEIGHT - 1) {
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

        for (size_t i = serp.size() - 1; i > 0; i--) {
            serp[i] = serp[i - 1];
        }

        serp[0] = nueva_pos;

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
    exit(0);
}



void Teclas() {
    while (true) {
        if (_kbhit()) {
            char tecla = _getch();
            if (tecla == 'w' && direccion_snake1 != 1) direccion_snake1 = 0; 
            if (tecla == 's' && direccion_snake1 != 0) direccion_snake1 = 1; 
            if (tecla == 'a' && direccion_snake1 != 3) direccion_snake1 = 2;
            if (tecla == 'd' && direccion_snake1 != 2) direccion_snake1 = 3; 

            if (modo_juego == 2) {
                if (tecla == 'i' && direccion_snake2 != 1) direccion_snake2 = 0; 
                if (tecla == 'k' && direccion_snake2 != 0) direccion_snake2 = 1; 
                if (tecla == 'j' && direccion_snake2 != 3) direccion_snake2 = 2; 
                if (tecla == 'l' && direccion_snake2 != 2) direccion_snake2 = 3; 
            }
        }
        Sleep(50);  
    }
}

int main() {
    Bienvenida();
    serpiente.push_back({ WIDTH / 2, HEIGHT / 2 });
    direccion_snake1 = 0; 
    
    if (modo_juego == 2) {
        serpiente2.push_back({ WIDTH / 2 - 1, HEIGHT / 2 });
        direccion_snake2 = 0; 
    }

    // Crear hilos
    thread t1(Movimiento_Snake, ref(serpiente), ref(direccion_snake1), 'w', 's', 'a', 'd', ref(puntuacion));
    

    thread t2;
    if (modo_juego == 2) {
        t2 = thread(Movimiento_Snake2, ref(serpiente2), ref(direccion_snake2), ref(puntuacion2)); 
    }

    thread t3(Teclas);
    thread t4(Comida, nullptr);
    thread t5(Temporizador, nullptr);

    t1.join();
    if (modo_juego == 2) {
        t2.join();
    }
    t3.join();
    t4.join();
    t5.join();

    return 0;
}
