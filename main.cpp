#include <iostream>
#include <string>
#include "utilidades.h"
#include <vector>
#include "juego.h"
#include "ejecutor_config.h"

int main() {
    EjecutorConfig ejecutor;
    EstadoJuego estado;
    Jugador gabriela;
    gabriela.nombre="gabriela";
    gabriela.color="naranja";
    estado.turnos.push_front("gabriela");
    estado.jugadores.push_back(gabriela);
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
            ejecutor.ayuda(estado, tokens);
        } else if (tokens[0] == "inicializar") {
            ejecutor.inicializarJuego(estado, tokens);
        } else if (tokens[0] == "obtener_unidades") {
            ejecutor.obtenerUnidades(estado, tokens);
        } else if (tokens[0] == "atacar") {
            ejecutor.atacar(estado, tokens);
        } else if (tokens[0] == "fortificar") {
            ejecutor.fortificar(estado, tokens);
        } else if (tokens[0] == "estado_juego") {
            ejecutor.estadoJuego(estado, tokens);
        }else if (tokens[0] == "guardar") {
            ejecutor.guardar(estado, tokens);
        }else if (tokens[0] == "guardar_comprimido") {
            ejecutor.guardar_comprimido(estado, tokens);
        }else if (tokens[0] == "costo_conquista") {
            ejecutor.costoConquista(estado, tokens);
        }else if (tokens[0] == "conquista_mas_barata") {
            ejecutor.conquistaMasBarata(estado, tokens);
        }else {
            std::cout << "Comando no reconocido: " << tokens[0] << ". Escriba ayuda para ver la lista de comandos" << std::endl;
        }
    
    }
    
    return 0;
}
