#ifndef EJECUTOR_CONFIG_H
#define EJECUTOR_CONFIG_H
#include <string>
#include <vector>
#include "juego.h"

class EjecutorConfig {
    public:
        void inicializarJuego(EstadoJuego &estado, std::vector<std::string> &tokens);
        void obtenerUnidades(EstadoJuego &estado, std::vector<std::string> &tokens);
        void atacar(EstadoJuego &estado, std::vector<std::string> &tokens);
        void fortificar(EstadoJuego &estado, std::vector<std::string> &tokens);
        void estadoJuego(EstadoJuego &estado, std::vector<std::string> &tokens);
        void ayuda(EstadoJuego &estado, std::vector<std::string> &tokens);
        void guardar(EstadoJuego &estado, std::vector<std::string> &tokens);
        void guardar_comprimido(EstadoJuego &estado, std::vector<std::string> &tokens);
        void costoConquista(EstadoJuego &estado, std::vector<std::string> &tokens);
        void conquistaMasBarata(EstadoJuego &estado, std::vector<std::string> &tokens);
};
#endif