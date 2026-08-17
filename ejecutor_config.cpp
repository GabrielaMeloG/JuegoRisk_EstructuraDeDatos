#include "ejecutor_config.h"
#include "utilidades.h"
#include <iostream>

void EjecutorConfig::inicializarJuego(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda inicializar' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.inicializado){
        std::cout << "Partida iniciada." << std::endl;
        estado.inicializado = true;
        return;
    }else{
        std::cout << "El juego ya ha sido inicializado." << std::endl;
        return;
    }
}
void EjecutorConfig::obtenerUnidades(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda obtener_unidades' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.inicializado){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.terminado){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else if(estado.jugadorActual() != tokens[1]){
        std::cout << "No es el turno del jugador " << tokens[1] << "." << std::endl;
        return;
    }else if(estado.existeJugador(tokens[1]) == false){
        std::cout << "El jugador " << tokens[1] << " no existe." << std::endl;
        return;
    }else{
        std::cout << "El jugador ha reclamado y ubicado todas sus unidades." << std::endl;
    }
}
void EjecutorConfig::atacar(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda atacar' para ver el uso correcto." << std::endl;
        return;
    }
    int idJugador = estado.indiceJugador(tokens[1]);
    if(!estado.inicializado){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.terminado){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else if(estado.jugadorActual() != tokens[1]){
        std::cout << "No es el turno del jugador " << tokens[1] << "." << std::endl;
        return;
    }else if(estado.existeJugador(tokens[1]) == false){
        std::cout << "El jugador " << tokens[1] << " no existe." << std::endl;
        return;
    }else if(estado.jugadores[idJugador].obtenidoUnidades == false){
        std::cout << "El jugador " << tokens[1] << " no ha reclamado y ubicado todas sus unidades." << std::endl;
        return;
    }else{
        std::cout << "El jugador " << tokens[1] << " ha terminado de atacar." << std::endl;
    }
}
void EjecutorConfig::fortificar(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda fortificar' para ver el uso correcto." << std::endl;
        return;
    }
    int idJugador = estado.indiceJugador(tokens[1]);
    if(!estado.inicializado){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.terminado){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else if(estado.jugadorActual() != tokens[1]){
        std::cout << "No es el turno del jugador " << tokens[1] << "." << std::endl;
        return;
    }else if(estado.existeJugador(tokens[1]) == false){
        std::cout << "El jugador " << tokens[1] << " no existe." << std::endl;
        return;
    }else if(estado.jugadores[idJugador].haAtacado == false){
        std::cout << "El jugador " << tokens[1] << " no ha atacado." << std::endl;
        return;
    }else{
        std::cout << "El jugador " << tokens[1] << " ha terminado de fortificar su posicion." << std::endl;
    }
}
void EjecutorConfig::estadoJuego(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() != 1){
        std::cout << "Parametros no validos. 'ayuda estado_juego' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.inicializado){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.terminado){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    
    }
}
void EjecutorConfig::ayuda(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() > 2){
        std::cout << "Comando incorrecto. ayuda [comando] para ver uso correcto de cualquier comando." << std::endl;
        return;
    }
    std::vector<std::string> comandos = {"inicializar", "obtener_unidades", "atacar", "fortificar", "estado_juego", "guardar", "guardar_comprimido", "costo_conquista", "conquista_mas_barata"};
    std::vector<std::string> usos = {
        "inicializar archivo_inicio.txt",
        "obtener_unidades nombre_jugador",
        "atacar nombre_jugador",
        "fortificar nombre_jugador",
        "estado_juego",
        "guardar archivo",
        "guardar_comprimido archivo",
        "costo_conquista nombre_jugador territorio",
        "conquista_mas_barata nombre_jugador"
    };
    if(tokens.size() == 1){
        std::cout << "Comandos disponibles:" << std::endl;
        for(size_t i = 0; i < comandos.size(); ++i){
            std::cout << "  " << comandos[i] << std::endl;
        }
    }else if(tokens.size() == 2){
        std::string comando = tokens[1];
        bool encontrado = false;
        for(size_t i = 0; i < comandos.size(); ++i){
            if(comando == comandos[i]){
                std::cout << "Uso del comando '" << comando << "': " << usos[i] << std::endl;
                encontrado = true;
                break;
            }
        }
        if(!encontrado){
            std::cout << "Comando no reconocido: " << comando << std::endl;
        }
    }
}
void EjecutorConfig::guardar(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda guardar' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.inicializado){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else{
        std::cout << "El juego ha sido guardado en el archivo " << tokens[1] << "." << std::endl;
    }
}
void EjecutorConfig::guardar_comprimido(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda guardar_comprimido' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.inicializado){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else{
        std::cout << "El juego ha sido guardado en el archivo " << tokens[1] << "." << std::endl;
    }
}
void EjecutorConfig::costoConquista(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() != 3){
        std::cout << "Parametros no validos. 'ayuda costo_conquista' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.inicializado){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.terminado){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else{
        std::cout << "El costo de conquistar el territorio " << tokens[2] << " desde para el jugador " << tokens[1] << " es de 5 unidades." << std::endl;
    }
}
void EjecutorConfig::conquistaMasBarata(EstadoJuego &estado, std::vector<std::string> &tokens){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda conquista_mas_barata' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.inicializado){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.terminado){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else{
        std::cout << "El territorio mas barato de conquistar para el jugador " << tokens[1] << " es el territorio XYZ con un costo de 3 unidades." << std::endl;
    }
}