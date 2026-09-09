#ifndef JUEGO_H
#define JUEGO_H
#include <string>
#include <vector>
#include <deque>
#include <stack>

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
        std::string getColorOcupante();
        int getUnidades();
        std::vector<std::string> getTerritoriosAdyacentes();
        void setColorOcupante(std::string color);
        void setUnidades(int cantidad);
};
//TAD jugador 
class Jugador {
    private:
        std::string nombre;
        std::string color;
        bool obtenidoUnidades;
        bool haAtacado; 
        std::vector<Territorio> territoriosOcupados;
        int unidades;

    public:
        Jugador(std::string nombre, std::string color);
        std::string getNombre();
        std::string getColor();
        bool getObtenidoUnidades();
        bool getHaAtacado();
        std::vector<Territorio> getTerritoriosOcupados();
        int getUnidades();
        void setObtenidoUnidades(bool estado);
        void setHaAtacado(bool estado);
        void agegarTerritorio(Territorio territorio);
        void agregarUnidades(int cantidad);
        void eliminarUnidades(int cantidad);
        void eliminarTerritorio(std::string codigo);

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
        void agregarUnidadesTerritorio(std::string codigoTerritorio, int cantidad);
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
//TAD EstadoJuego
class EstadoJuego {
    private:
        bool inicializado;
        bool terminado;
        std::vector<Jugador> jugadores;
        std::deque<std::string> turnos;
        int turnoActual;
    public: 
        EstadoJuego();
        std::vector<Jugador> getJugadores();
        void setInicializado(bool estado);
        void setTerminado(bool estado);
        void setTurno(int turno);
        int getTurno();
        void siguienteTurno(int cantidadJugadores);
        bool getInicializado();
        bool getTerminado();
        bool existeJugador(std::string nombre);
        int indiceJugador(std::string nombre);
        bool esTurnoDe(std::string nombre);
        void siguienteTurno();
        bool existeTerritorio(std::string codigo, Tablero tablero);
        void reiniciar();
};

#endif

