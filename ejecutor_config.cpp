#include "ejecutor_config.h"
#include "utilidades.h"
#include <iostream>
#include <fstream>

void EjecutorConfig::inicializarJuego(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda inicializar' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.getInicializado()){
        std::cout << "Partida iniciada." << std::endl;
        estado.setInicializado(true);
        return;
    }else{
        std::cout << "El juego ya ha sido inicializado." << std::endl;
        return;
    }
    
    std::ifstream archivo(tokens[1]);
    if(!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo: " << tokens[1] << std::endl;
        return;
    }
    if(archivo.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "El archivo esta vacio: " << tokens[1] << std::endl;
        return;
    }
    int cantidadJugadores;
    archivo >> cantidadJugadores;
    if(cantidadJugadores < 3 || cantidadJugadores > 6){
        std::cout << "Cantidad de jugadores no valida. Debe ser entre 3 y 6." << std::endl;
    }
    for(int i = 0 ; i < cantidadJugadores; i++){
        std::string nombreJugador;
        std::string colorJugador;
        archivo >> nombreJugador >> colorJugador;
        Jugador jugador = Jugador(nombreJugador, colorJugador);
        std::vector<Jugador> jugadores = estado.getJugadores();
        jugadores.push_back(jugador);
    }
    for(int i = 0 ; i < 42 ; i++){
        std::string codigoTerritorio;
        std::string colorOcupante;
        int unidades;
        archivo >> codigoTerritorio >> colorOcupante >> unidades;
        std::vector<Continente> continentes = tablero.getContinentes();
        std::vector<Continente>::iterator itContinente;
        std::vector<Territorio>::iterator itTerritorio;
        for(itContinente = continentes.begin(); itContinente != continentes.end();itContinente++){
            std::vector<Territorio> territorios = itContinente->getTerritorios();
            for(itTerritorio = territorios.begin(); itTerritorio != territorios.end(); itTerritorio++){
                if(itTerritorio->getCodigo() == codigoTerritorio){
                    itTerritorio->setColorOcupante(colorOcupante);
                    itTerritorio->setUnidades(unidades);
                }
            }
        }
    }
    for(Jugador j : estado.getJugadores()){
        std::vector<Continente> continentes = tablero.getContinentes();
        for(Continente continente : continentes){
            std::vector<Territorio> territorios = continente.getTerritorios();
            for(Territorio territorio : territorios){
                if(territorio.getColorOcupante() == j.getColor()){
                    j.agegarTerritorio(territorio);
                    j.agregarUnidades(territorio.getUnidades());
                }
            }
        }
    }
    switch(cantidadJugadores){
        case 3:
            for(Jugador j : estado.getJugadores()){
                if(j.getUnidades() != 35){
                    std::cout << "Error: Hay jugadores que no tienen 35 unidades." << std::endl;
                    estado.reiniciar();
                    return;
                }
            }
            break;
        case 4:
            for(Jugador j : estado.getJugadores()){
                if(j.getUnidades() != 30){
                    std::cout << "Error: Hay jugadores que no tienen 30 unidades." << std::endl;
                    estado.reiniciar();
                    return;
                }
            }
            break;
        case 5:
            for(Jugador j : estado.getJugadores()){
                if(j.getUnidades() != 25){
                    std::cout << "Error: Hay jugadores que no tienen 25 unidades." << std::endl;
                    estado.reiniciar();
                    return;
                }
            }
            break;
        case 6:
            for(Jugador j : estado.getJugadores()){
                if(j.getUnidades() != 20){
                    std::cout << "Error: Hay jugadores que no tienen 20 unidades." << std::endl;
                    estado.reiniciar();
                    return;
                }
            }
            break;
    }
    archivo.close();
    std::cout << "Partida iniciada." << std::endl;
    return;
}
void EjecutorConfig::obtenerUnidades(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda obtener_unidades' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado aún." << std::endl;
        return;
    }else if(estado.getTerminado()){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else if(estado.existeJugador(tokens[1]) == false){
        std::cout << "El jugador " << tokens[1] << " no hace parte de esta partida." << std::endl;
        return;
    }else if(estado.getTurno() != estado.indiceJugador(tokens[1])){
        std::cout << "No es el turno del jugador " << tokens[1] << "." << std::endl;
        return;
    }else{ 
        std::vector<Jugador> jugadores = estado.getJugadores();
        std::vector<Territorio> territorios = jugadores[estado.getTurno()].getTerritoriosOcupados();
        int unidadesAgregadas = territorios.size() / 3;
        if(unidadesAgregadas < 3){
            unidadesAgregadas = 3;
            std::cout<< "El jugador " << tokens[1] << " ha reclamado " << unidadesAgregadas << " unidades." << std::endl;
        }else{
            std::cout<< "El jugador " << tokens[1] << " ha reclamado " << unidadesAgregadas << " unidades." << std::endl;
        }
        while(unidadesAgregadas > 0){
            int cantidad;
            std::string linea;
            std::cout << "Donde desea ubicar sus unidades? (Ingrese el codigo del territorio y la cantidad de unidades a ubicar)" << std::endl;
            std::cout << "Unidades restantes: " << unidadesAgregadas << std::endl;
            std::getline(std::cin, linea);
            std::vector<std::string> tokensUbicacion = tokenizar(linea);
            if(tokensUbicacion.size() != 2){
                std::cout << "Parametros no validos. Debe ingresar el codigo del territorio y la cantidad de unidades a ubicar." << std::endl;
                continue;
            }
            std::string codigoTerritorio = tokensUbicacion[0];
            cantidad = std::stoi(tokensUbicacion[1]);
            if(cantidad > unidadesAgregadas){
                std::cout << "No puede ubicar mas unidades de las que tiene disponibles." << std::endl;
                continue;
            }else{
                if(!estado.existeTerritorio(codigoTerritorio, tablero)){
                    std::cout << "El territorio con codigo " << codigoTerritorio << " no existe." << std::endl;
                    continue;
                }
                std::vector<Territorio> territoriosJugador = jugadores[estado.getTurno()].getTerritoriosOcupados();
                bool territorioValido = false;
                std::vector<Territorio>::iterator it;
                for(it = territoriosJugador.begin(); it != territoriosJugador.end(); ++it){
                    if(it->getCodigo() == codigoTerritorio){
                        territorioValido = true;
                        break;
                    }
                }
                if(!territorioValido){
                    std::cout << "El jugador " << tokens[1] << " no posee el territorio con codigo " << codigoTerritorio << "." << std::endl;
                    continue;
                }
                tablero.agregarUnidadesTerritorio(codigoTerritorio, cantidad);
                jugadores[estado.getTurno()].agregarUnidades(cantidad);
                unidadesAgregadas -= cantidad;
            }

        }
        std::cout << "El jugador " << tokens[1] << " ha terminado de ubicar sus unidades." << std::endl;
        jugadores[estado.getTurno()].setObtenidoUnidades(true);
    }
}
void EjecutorConfig::atacar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda atacar' para ver el uso correcto." << std::endl;
        return;
    }
    int idJugador = estado.indiceJugador(tokens[1]);
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.getTerminado()){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else if(estado.existeJugador(tokens[1]) == false){
        std::cout << "El jugador " << tokens[1] << " no hace parte de esta partida." << std::endl;
        return;
    }else if(estado.jugadorActual() != tokens[1]){
        std::cout << "No es el turno del jugador " << tokens[1] << "." << std::endl;
        return;
    }else if(estado.jugadores[idJugador].obtenidoUnidades == false){
        std::cout << "El jugador " << tokens[1] << " no ha reclamado y ubicado todas sus unidades." << std::endl;
        return;
    }else{
        std::cout << "El jugador " << tokens[1] << " ha terminado de atacar." << std::endl;
        estado.jugadores[idJugador].haAtacado=true;
    }
}
void EjecutorConfig::fortificar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda fortificar' para ver el uso correcto." << std::endl;
        return;
    }
    int idJugador = estado.indiceJugador(tokens[1]);
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.getTerminado()){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else if(estado.existeJugador(tokens[1]) == false){
        std::cout << "El jugador " << tokens[1] << " no hace parte de esta partida." << std::endl;
        return;
    }else if(estado.jugadorActual() != tokens[1]){
        std::cout << "No es el turno del jugador " << tokens[1] << "." << std::endl;
        return;
    }else if(estado.jugadores[idJugador].haAtacado == false){
        std::cout << "El jugador " << tokens[1] << " no ha atacado." << std::endl;
        return;
    }else{
        std::cout << "El jugador " << tokens[1] << " ha terminado de fortificar su posicion." << std::endl;
    }
}
void EjecutorConfig::estadoJuego(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
    if(tokens.size() != 1){
        std::cout << "Parametros no validos. 'ayuda estado_juego' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.getTerminado()){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else{
        std::cout << "aqui va la respuesta xdddd" << std::endl;
    }
}
void EjecutorConfig::ayuda(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
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
                std::cout << "Uso correcto del comando '" << comando << "': " << usos[i] << std::endl;
                encontrado = true;
                break;
            }
        }
        if(!encontrado){
            std::cout << "Comando no reconocido: " << comando << std::endl;
        }
    }
}
void EjecutorConfig::guardar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda guardar' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else{
        std::cout << "El juego ha sido guardado en el archivo " << tokens[1] << "." << std::endl;
    }
}
void EjecutorConfig::guardar_comprimido(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda guardar_comprimido' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else{
        std::cout << "El juego ha sido guardado en el archivo " << tokens[1] << "." << std::endl;
    }
}
void EjecutorConfig::costoConquista(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
    if(tokens.size() != 3){
        std::cout << "Parametros no validos. 'ayuda costo_conquista' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.getTerminado()){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else if(!estado.existeJugador(tokens[1])){
        std::cout << "El jugador " << tokens[1] << " no hace parte de esta partida." << std::endl;
    }else{
        std::cout << "Para conquistar el territorio "<< tokens[2] <<", "<<tokens[1]<<" debe atacar desde ABC , pasando por el territorio XYZ. Debe conquistar 7 unidades de ejército." << std::endl;
    }
}
void EjecutorConfig::conquistaMasBarata(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda conquista_mas_barata' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }else if(estado.getTerminado()){
        std::cout << "El juego ya ha terminado." << std::endl;
        return;
    }else if(!estado.existeJugador(tokens[1])){
        std::cout << "El jugador " << tokens[1] << " no hace parte de esta partida." << std::endl;
        return;
    }else{
        std::cout << "El territorio mas barato de conquistar para el jugador " << tokens[1] << " es el territorio XYZ con un costo de 3 unidades." << std::endl;
    }
}