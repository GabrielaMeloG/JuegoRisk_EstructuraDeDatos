#ifndef JUEGO_H
#define JUEGO_H
#include <string>
#include <vector>
#include <deque>

//TAD jugador 
struct Jugador {
    std::string nombre;
    std::string color;
    bool obtenidoUnidades;
    bool haAtacado; 
};

//TAD territorio
struct Territorio {
    std::string codigo;
    std::string colorOcupante;
    int unidades;
};


//TAD EstadoJuego
class EstadoJuego {
    public: 
        EstadoJuego ();

        bool inicializado;
        bool terminado;

        std::vector<Jugador> jugadores;
        std::vector<Territorio> territorios;
        std::deque<std::string> turnos;

        bool existeJugador(std::string nombre);
        int indiceJugador(std::string nombre);
        bool esTurnoDe(std::string nombre);
        std::string jugadorActual();
        void siguienteTurno();

        int indiceTerritorio(std::string codigo);

        void reiniciar();

        
};
#endif

