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
        Territorio();
        std::string getCodigo();
        std::string getNombre();
        std::string getColorOcupante();
        int getUnidades();
        std::vector<std::string> getTerritoriosAdyacentes();
        void setColorOcupante(std::string color);
        void setUnidades(int cantidad);
        bool esAdyacente(std::string codigo);
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
        std::vector<Carta> mano;
    public:
        Jugador(std::string nombre, std::string color);
        std::string getNombre();
        std::string getColor();
        bool getObtenidoUnidades();
        bool getHaAtacado();
        void quitarCartas(int indices[3]);
        std::vector<Territorio> getTerritoriosOcupados();
        bool poseeTerritorio(std::string codigo);
        int getUnidades();
        std::vector<Carta>& getCartas();
        void agregarCarta(Carta carta);
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
        int getCantidadCartasEntregadas();
        void cartasEntregadas();
        std::vector<Continente> getContinentes();
        void agregarUnidadesTerritorio(std::string codigoTerritorio, int cantidad);
        Territorio& getTerritorio(std::string codigoTerritorio);
};
class Carta {
    private:
        std::string codigoTerritorio;
        std::string dibujo;
        bool esComodin;
    public:
        Carta(std::string id, std::string dibujo);
        std::string getIdTerritorio();
        std::string getDibujo();
        bool getEsComodin();
        void setComodin(bool comodin);
};
class Baraja {
    private:
        std::stack<Carta> cartas;
    public:
        Baraja();
        Carta tomarCarta();
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
        std::vector<Jugador>& getJugadores();
        void setInicializado(bool estado);
        void setTerminado(bool estado);
        void setTurno(int turno);
        int getTurno();
        bool compararCartas(Jugador jugador, int indices[3]);
        int getUnidadesPorCartas(Tablero tablero);
        void siguienteTurno(int cantidadJugadores);
        bool getInicializado();
        bool getTerminado();
        bool existeJugador(std::string nombre);
        int indiceJugador(std::string nombre);
        bool esTurnoDe(std::string nombre);
        void siguienteTurno();
        bool existeTerritorio(std::string codigo, Tablero tablero);
        void reiniciar();
        bool compararDados(int dadosAtacante[3], int dadosDefensor[2]);
};

#endif

