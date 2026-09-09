#ifndef JUEGO_H
#define JUEGO_H
#include <string>
#include <vector>
#include <deque>
#include <stack>

//TAD jugador 
class Jugador {
    private:
        std::string nombre;
        std::string color;
        bool obtenidoUnidades;
        bool haAtacado; 
    public:
        Jugador(std::string nombre, std::string color);
        std::string getNombre();

};

//TAD territorio
class Territorio {
    private:
        std::string nombre;
        std::string codigo;
        std::string colorOcupante;
        int unidades;
        std::vector<std::string> territoriosAdyacentes;
    public:
        Territorio(std::string nombre, std::string codigo, std::vector<std::string> adyacentes);
        std::string getCodigo();
        std::string getNombre();
        void setColorOcupante(std::string color);
        void setUnidades(int cantidad);
};
class Continente {
    private:
        std::string nombre;
        int codigo;
        std::vector<Territorio> territorios;
    public:
        Continente(int id, std::string nombre, std::vector<Territorio> territorios);
        std::string getNombre();
        std::vector<Territorio> getTerritorios();
        int getCodigo();
};
class Tablero {
    private:
        std::vector<Continente> continentes;
        int cantidadCartasEntregadas;
    public:
        Tablero();
        std::vector<Continente> getContinentes();
};
class Carta {
    private:
        std::string codigoTerritorio;
        std::string dibujo;
        bool esComodin;
        bool esMision;
    public:
        Carta(std::string id, std::string dibujo);
        void setComodin(bool comodin);
        void setMision(bool mision);
};
class Baraja {
    private:
        std::stack<Carta> cartas;
    public:
        Baraja();
};
class Juego {
    private:
    public:
        Juego();
};
//TAD EstadoJuego
class EstadoJuego {
    private:
        bool inicializado;
        bool terminado;
        std::vector<Jugador> jugadores;
        std::vector<Territorio> territorios;
        std::deque<std::string> turnos;
    public: 
        EstadoJuego();
        std::vector<Jugador> getJugadores();
        void setInicializado(bool estado);
        void setTerminado(bool estado);
        bool getInicializado();
        bool getTerminado();
        bool existeJugador(std::string nombre);
        int indiceJugador(std::string nombre);
        bool esTurnoDe(std::string nombre);
        std::string jugadorActual();
        void siguienteTurno();
        int indiceTerritorio(std::string codigo);
        void reiniciar();
        
};

#endif

