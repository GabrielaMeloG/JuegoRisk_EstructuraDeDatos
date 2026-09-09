#ifndef EJECUTOR_CONFIG_H
#define EJECUTOR_CONFIG_H
#include <string>
#include <vector>
#include "juego.h"

class EjecutorConfig {
    public:
        void inicializarJuego(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
        void obtenerUnidades(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
        void atacar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
        void fortificar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
        void estadoJuego(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
        void ayuda(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
        void guardar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
        void guardar_comprimido(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
        void costoConquista(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
        void conquistaMasBarata(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero);
};
#endif