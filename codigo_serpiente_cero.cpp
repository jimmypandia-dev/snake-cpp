#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

class Serpiente {
public:
    enum Direccion { DERECHA = 1, ARRIBA = 2, IZQUIERDA = 3, ABAJO = 4 };
    int ancho = 0, alto = 0, 
    bool esta_viva = true;
    Direccion direccion = DERECHA;
    vector<int> celdas;
    Serpiente();
    Serpiente(int w, int h);
    bool tiene(int una_celda);
    void cambiar_direccion(Direccion d);
    int cabeza();
    int proxima_cabeza();
};

Serpiente::Serpiente() {

}

Serpiente::Serpiente(int w, int h) {
    ancho = w;
    alto = h;
}

bool Serpiente::tiene(int una_celda) {
    for (int i = 0; i < celdas.size(); i++) {
        if (celdas[i] == una_celda) {
            return true;
        }
    }
    return false;
}

void Serpiente::cambiar_direccion(Direccion d) {
    if (2 != abs(direccion - d)) {
        direccion = d;
    }
}

int Serpiente::cabeza() {
return celdas.back();
}

int Serpiente::proxima_cabeza() {
    int x = cabeza()%ancho;
    int y = cabeza()/ancho;
    switch (direccion) {
        case DERECHA:   x++; break;
        case IZQUIERDA: x--; break;
        case ARRIBA:    y--; break;
        case ABAJO:     y++; break;
        default: break;
    }

    if (x < 0 || x >= ancho || y < 0 || y >= alto) {
        return -1;
    }
    return y * ancho + x;
}

class LogicaJuego {
    private:
        vector<int> tablero, celdas_libres;
        Serpiente serpiente;
        Serpiente serpiente2;
        string nombre_jugador;
        int puntaje = 0, celda_comida = 0, velocidad_serpiente = 0;

        void leer_entrada();
        void generar_comida();
        void actualizar();
        void ocultar_o_mover_cursor();
        void redibujar();

    public:
        LogicaJuego(Serpiente s);
        void configurar();
        void ejecutar();
};

void LogicaJuego::leer_entrada() {
    int tiempo_esperado_ms = 0;
    int tiempo_espera_tecla = 20;
    while (velocidad_serpiente > tiempo_esperado_ms){
        if (kbhit()) {
            switch (getch()) {
                case 'a': case 'A': serpiente.cambiar_direccion(Serpiente::IZQUIERDA); break;
                case 's': case 'S': serpiente.cambiar_direccion(Serpiente::ABAJO);      break;
                case 'd': case 'D': serpiente.cambiar_direccion(Serpiente::DERECHA);    break;
                case 'w': case 'W': serpiente.cambiar_direccion(Serpiente::ARRIBA);     break;
                case 'f': case 'F': serpiente2.cambiar_direccion(Serpiente::IZQUIERDA); break;
                case 'g': case 'G': serpiente2.cambiar_direccion(Serpiente::ABAJO);      break;
                case 'h': case 'H': serpiente2.cambiar_direccion(Serpiente::DERECHA);    break;
                case 't': case 'T': serpiente2.cambiar_direccion(Serpiente::ARRIBA);     break;
                case 'X': serpiente.esta_viva = 0; break;
                default: break;

            }
        }
        tiempo_esperado_ms += tiempo_espera_tecla;
        Sleep(tiempo_espera_tecla);  // esperar antes del siguiente chequeo de teclado
    }
}

void LogicaJuego::generar_comida() {
    celdas_libres.clear();
    for (int i = 0; i < tablero.size(); i++) {
        if (!serpiente.tiene(i)) {
            celdas_libres.push_back(i);
        }
    }
    if (celdas_libres.empty()) {
        serpiente.esta_viva = 0;
        celda_comida = -1;
        return;
    }
    int posicion = rand() % celdas_libres.size();
    celda_comida = celdas_libres[posicion];
}

void LogicaJuego::actualizar() {
    int proxima = serpiente.proxima_cabeza();
    int proxima2 = serpiente2.proxima_cabeza();

    if (proxima == -1 || serpiente.tiene(proxima)) {
        serpiente.esta_viva = 0;
    }
    else {
        if (proxima == celda_comida) {
            serpiente.celdas.push_back(proxima);
            generar_comida();
            puntaje++;
        } else {
            serpiente.celdas.erase(serpiente.celdas.begin());
            serpiente.celdas.push_back(proxima);
        }
    }

    if (proxima2 == -1 || serpiente2.tiene(proxima2)) {
        serpiente2.esta_viva = 0;
    }
    else {
        if (proxima2 == celda_comida) {
            serpiente2.celdas.push_back(proxima2);
            generar_comida();
            puntaje++;
        } else {
            serpiente2.celdas.erase(serpiente2.celdas.begin());
            serpiente2.celdas.push_back(proxima2);
        }
    }
}

void LogicaJuego::ocultar_o_mover_cursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);
}

void LogicaJuego::redibujar() {
    ocultar_o_mover_cursor();
    cout << "Serpiente en c++: Controles: A-izquierda, S-abajo, D-derecha, W-arriba" << endl;
    int ancho = serpiente.ancho;
    int celda_cabeza = serpiente.cabeza();
    int celda_cabeza2 = serpiente2.cabeza();
    for (int celda_actual = 0; celda_actual < tablero.size(); celda_actual++) {
        if (celda_actual % ancho == 0) {
            cout << " ~]";
        }
        if (celda_actual == celda_comida){
            cout << " @ ";
        } else if (celda_actual == celda_cabeza) {
            cout << " o ";
        } else if (celda_actual == celda_cabeza2){
            cout << " O ";
        } else if(serpiente.tiene(celda_actual)){
            cout << " x ";
        } else if (serpiente2.tiene(celda_actual)){
             cout << " X ";
        } else {
            cout << " . ";
        }

        if ((celda_actual + 1) % ancho == 0) {
            cout << "[~" << endl;
        }
    }
    cout << nombre_jugador << " - PUNTAJE: " << puntaje << "    " << endl;
}

LogicaJuego::LogicaJuego(Serpiente s) {
    serpiente = s;
    serpiente2 = Serpiente(s.ancho, s.alto);
    int forma = serpiente.ancho * serpiente.alto;
    for (int i = 0; i < forma; i++) {
        tablero.push_back(i);
    }
}

void LogicaJuego::configurar() {
    cout << "Ingresa tu nombre: ";
    getline(cin, nombre_jugador);
    if (nombre_jugador.empty()) { nombre_jugador = "Jugador"; }

    int opcion = 1;
    cout << "\nSelecciona la dificultad:\n";
    cout << "1. Facil (Lento)\n";
    cout << "2. Medio (Normal)\n";
    cout << "3. Dificil (Rapido)\n";
    cout << "Opcion: ";
    cin >> opcion;

    if (opcion == 1){
        velocidad_serpiente = 150;
    }
    else if (opcion == 2) {
        velocidad_serpiente = 90;
    }
    else {
        velocidad_serpiente = 40;
    }

    srand((unsigned int)time(0));

    serpiente.celdas.push_back(0);
    serpiente.celdas.push_back(1);
    serpiente.direccion = Serpiente::DERECHA;
    serpiente2.celdas.push_back(299);
    serpiente2.celdas.push_back(298);
    serpiente2.direccion = Serpiente::IZQUIERDA;
    generar_comida();

    system("cls");
}

void LogicaJuego::ejecutar() {
    while (serpiente.esta_viva || serpiente2.esta_viva) {
        leer_entrada();
        actualizar();
        redibujar();
    }
}

int main() {
    LogicaJuego juego(Serpiente(20, 15));
    juego.configurar();
    juego.ejecutar();
    cout << "JUEGO TERMINADO." << endl;
    getch();
    return 0;
}
