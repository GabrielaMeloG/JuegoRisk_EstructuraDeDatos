#include "juego.h"
#include "utilidades.h"

EstadoJuego::EstadoJuego() {
 reiniciar();
}

void EstadoJuego::reiniciar() {
    inicializado = false;
    terminado = false;
    jugadores.clear();
    territorios.clear();
    turnos.clear();
}

bool EstadoJuego::existeJugador(std::string nombre) {
    return indiceJugador(nombre) != -1;
}

int EstadoJuego::indiceJugador(std::string nombre) {
    return buscarIndicePorNombre(jugadores, nombre);    
}

bool EstadoJuego::esTurnoDe(std::string nombre) {
    if (turnos.empty()) {
        return false;
    }
    return turnos.front() == nombre;
}

std ::string EstadoJuego::jugadorActual() {
    if (turnos.empty()) {
        return "";
    }
    return turnos.front();
}

void EstadoJuego::siguienteTurno() {
    if (!turnos.empty()) {
        std::string jugadorActual = turnos.front();
        turnos.pop_front();
        turnos.push_back(jugadorActual);
    }
}

int EstadoJuego::indiceTerritorio(std::string codigo) {
    for (size_t i = 0; i < territorios.size(); i++) {
        if (territorios[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}


