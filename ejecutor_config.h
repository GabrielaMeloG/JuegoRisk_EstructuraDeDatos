#ifndef EJECUTOR_CONFIG_H
#define EJECUTOR_CONFIG_H
#include <string>
#include <vector>
#include "juego.h"

class EjecutorConfig {
    public:
        bool validarEstructuraArchivo(const std::string& ruta, Tablero& tablero, std::string& errorMsg);
        void inicializarJuego(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
        void obtenerUnidades(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
        void atacar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
        void fortificar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
        void estadoJuego(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
        void ayuda(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
        void guardar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
        void guardar_comprimido(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
        void costoConquista(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
        void conquistaMasBarata(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja);
};
#endif