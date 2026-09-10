#include <iostream>
#include <string>
#include "utilidades.h"
#include <vector>
#include "juego.h"
#include "ejecutor_config.h"

int main() {
    srand(time(0));
    EjecutorConfig ejecutor;
    EstadoJuego estado;
    Baraja baraja = Baraja();
    Tablero tablero = Tablero();
    std::vector<std::string> tokens;
    bool continuar = true;
    std::string comando;
    std::cout << "***********************************" << std::endl;
    std::cout << "*         Apoyo para Risk         *" << std::endl;
    std::cout << "* Escriba ayuda para ver comandos *" << std::endl;
    std::cout << "*        Escriba salir para       *" << std::endl;
    std::cout << "*       terminar el programa      *" << std::endl;
    std::cout << "*       ayuda [comando] para      *" << std::endl;
    std::cout << "*  ver el llamado de un comando   *" << std::endl;
    std::cout << "***********************************" << std::endl;
    while (continuar) {
        
        std::cout << "$";   
        std::getline(std::cin, comando);
        comando = trim(comando);
        tokens = tokenizar(comando);
        if (tokens.empty()) {
            continue;
        }
        tokens[0] = pasarAMinusculas(tokens[0]);
        if (tokens[0] == "salir") {
            continuar = false;
        } else if (tokens[0] == "ayuda") { 
            ejecutor.ayuda(estado, tokens, tablero, baraja);
        } else if (tokens[0] == "inicializar") {
            ejecutor.inicializarJuego(estado, tokens, tablero, baraja);
        } else if (tokens[0] == "obtener_unidades") {
            ejecutor.obtenerUnidades(estado, tokens, tablero, baraja);
        } else if (tokens[0] == "atacar") {
            ejecutor.atacar(estado, tokens, tablero, baraja);
        } else if (tokens[0] == "fortificar") {
            ejecutor.fortificar(estado, tokens, tablero, baraja);
        } else if (tokens[0] == "estado_juego") {
            ejecutor.estadoJuego(estado, tokens, tablero, baraja);
        }else if (tokens[0] == "guardar") {
            ejecutor.guardar(estado, tokens, tablero, baraja);
        }else if (tokens[0] == "guardar_comprimido") {
            ejecutor.guardar_comprimido(estado, tokens, tablero, baraja);
        }else if (tokens[0] == "costo_conquista") {
            ejecutor.costoConquista(estado, tokens, tablero, baraja);
        }else if (tokens[0] == "conquista_mas_barata") {
            ejecutor.conquistaMasBarata(estado, tokens, tablero, baraja);
        }else if(estado.getTerminado()){
            std::cout << "LA PARTIDA A TERMINADO CON " << estado.getJugadores()[estado.getTurno()].getNombre() << " COMO GANADOR DE LA PARTIDA"<<std::endl;
            continuar = false;
        } else {
            std::cout << "Comando no reconocido: " << tokens[0] << ". Escriba ayuda para ver la lista de comandos" << std::endl;
        }
    
    }
    
    return 0;
}
