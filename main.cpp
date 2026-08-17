#include <iostream>
#include <string>
#include "utilidades.h"
#include <vector>
#include "juego.h"
#include "ejecutor_config.h"

int main() {
    EjecutorConfig ejecutor;
    EstadoJuego estado;
    std::vector<std::string> tokens;
    std::vector<std::string> lol;
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
            agregarPrimeros2(tokens, lol);
            ejecutor.inicializarJuego(estado, lol);
        } else if (tokens[0] == "obtener_unidades") {
            agregarPrimeros2(tokens, lol);
            ejecutor.obtenerUnidades(estado, lol);
        } else if (tokens[0] == "atacar") {
            agregarPrimeros2(tokens, lol);
            ejecutor.atacar(estado, lol);
        } else if (tokens[0] == "fortificar") {
            agregarPrimeros2(tokens, lol);
            ejecutor.fortificar(estado, lol);
        } else if (tokens[0] == "estado_juego") {
            agregarPrimeros2(tokens, lol);
            ejecutor.estadoJuego(estado, lol);
        }else if (tokens[0] == "guardar") {
            agregarPrimeros2(tokens, lol);
            ejecutor.guardar(estado, lol);
        }else if (tokens[0] == "guardar_comprimido") {
            agregarPrimeros2(tokens, lol);
            ejecutor.guardar_comprimido(estado, lol);
        }else if (tokens[0] == "costo_conquista") {
            agregarPrimeros3(tokens, lol);
            ejecutor.costoConquista(estado, lol);
        }else if (tokens[0] == "conquista_mas_barata") {
            agregarPrimeros2(tokens, lol);
            ejecutor.conquistaMasBarata(estado, lol);
        }else {
            std::cout << "Comando no reconocido: " << tokens[0] << ". Escriba ayuda para ver la lista de comandos" << std::endl;
        }
    
    }
    
    return 0;
}
