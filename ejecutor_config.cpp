#include "ejecutor_config.h"
#include "utilidades.h"
#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <ctime> 

void EjecutorConfig::inicializarJuego(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
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
    std::vector<Jugador>::iterator itJugador;
    for(itJugador = estado.getJugadores().begin(); itJugador != estado.getJugadores().end(); itJugador++){
        for(int i = 0; i < 3; i++){
            Carta carta = baraja.tomarCarta();
            itJugador->agregarCarta(carta);
        }
    }
    std::cout << "Partida iniciada." << std::endl;
    return;
}
void EjecutorConfig::obtenerUnidades(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
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
        if(jugadores[estado.getTurno()].getCartas().size() >= 3){
            std::cout << "Desea utilizar cartas para obtener unidades adicionales? (si/no)" << std::endl;
            std::string respuesta;
            std::cin >> respuesta;
            if(trim(respuesta) == "si"){
                do{ 
                std::vector<Jugador> jugadores = estado.getJugadores();
                Jugador jugador = jugadores[estado.getTurno()];
                std::vector<Carta> cartasJugador = jugador.getCartas();
                std::cout << "Cartas disponibles:" << std::endl;
                for(int i = 0; i < cartasJugador.size(); i++){
                    std::cout << i+1 << ". Codigo: " << cartasJugador[i].getIdTerritorio() << ", Dibujo: " << cartasJugador[i].getDibujo() << std::endl;
                }
                std::cout << "Ingrese los numeros de las cartas que desea utilizar (separados por espacio):" << std::endl;
                std::string linea;
                std::getline(std::cin, linea);
                std::vector<std::string> tokensCartas = tokenizar(linea);
                if(tokensCartas.size() != 3){
                    std::cout << "Solo puede seleccionar 3 cartas." << std::endl;
                    return;
                }
                int indices[3];
                for(int i = 0; i < 3; i++){
                    indices[i] = std::stoi(tokensCartas[i]) - 1;
                    if(indices[i] < 0 || indices[i] >= cartasJugador.size()){
                        std::cout << "Indice de carta no valido: " << tokensCartas[i] << std::endl;
                        return;
                    }
                }
                if(estado.compararCartas(jugador, indices)){
                    unidadesAgregadas += estado.getUnidadesPorCartas(tablero);
                    jugador.quitarCartas(indices);
                    if(jugador.poseeTerritorio(cartasJugador[indices[0]].getIdTerritorio())){
                        unidadesAgregadas += 2;
                    }else if(jugador.poseeTerritorio(cartasJugador[indices[1]].getIdTerritorio())){
                        unidadesAgregadas += 2;
                    }else if(jugador.poseeTerritorio(cartasJugador[indices[2]].getIdTerritorio())){
                        unidadesAgregadas += 2;
                    }
                    std::cout << "Se han utilizado las cartas para obtener " << estado.getUnidadesPorCartas(tablero) << " unidades adicionales." << std::endl;
                }else{
                    std::cout << "Las cartas seleccionadas no son validas para obtener unidades adicionales." << std::endl;
                }
                std::cout << "Desea utilizar mas cartas? (si/no)" << std::endl;
                std::cin >> respuesta;
            }while(respuesta != "no"); 
            }
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
void EjecutorConfig::atacar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda atacar' para ver el uso correcto." << std::endl;
        return;
    }
    std::vector<Jugador> jugadores = estado.getJugadores();
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
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
    }else if(!jugadores[estado.getTurno()].getObtenidoUnidades()){
        std::cout << "El jugador " << tokens[1] << " no ha reclamado y ubicado todas sus unidades." << std::endl;
        return;
    }else{
        std::string territorioId;
        std::string respuesta;
        do{ 
        std::cout << "Desde que territorio desea atacar? (Ingrese el codigo del territorio)" << std::endl;
        std::cin >> territorioId;
        if(jugadores[estado.getTurno()].poseeTerritorio(territorioId)){
            std::vector<Territorio> territorios = jugadores[estado.getTurno()].getTerritoriosOcupados();
            std::vector<Territorio>::iterator it;
            int unidades;
            Territorio territorioAtacante = Territorio();
            for(it = territorios.begin();it!=territorios.end();it++){
                if(it->getCodigo()==territorioId){
                    unidades = it->getUnidades();
                    territorioAtacante = *it;
                    if(unidades == 1){
                        std::cout <<"El territorio seleccionado no tiene suficientes unidades para atacar"<<std::endl;
                    }
                }
            }
        std::cout << "Que territorio desea atacar? (Ingrese el codigo del territorio)" << std::endl;
        std::cin >> territorioId;
        Territorio territorioDefensor = tablero.getTerritorio(territorioId);
        if(jugadores[estado.getTurno()].poseeTerritorio(territorioId)){
            std::cout << " no se puede atacar un territorio que ya esta conquistado" << std::endl;
            continue;
        }else if(territorioAtacante.esAdyacente(territorioId)){
            int dadosAtacante[3];
            int dadosDefensor[2];
            for(int i = 0 ; i < 3; i++){
                srand(time(0));
                dadosAtacante[i] = std::rand() % 6 + 1;
            }
            for(int i = 0 ; i < 2; i++){
                srand(time(0));
                dadosDefensor[i] = std::rand() % 6 + 1;
            }
            bool ganadorAtacante = estado.compararDados(dadosAtacante, dadosDefensor);
            if(ganadorAtacante){
                territorioDefensor.setUnidades(territorioDefensor.getUnidades()-1);
                if(territorioDefensor.getUnidades()==0){
                    
                    std::cout << "El territorio " << territorioDefensor.getNombre() << " ha sido conquistado por " << tokens[1] << std::endl;
                    int cantidad;
                    do{
                    std::cout << "Cuantas unidades desea mover a ese territorio?"<<std::endl;
                    std::cin>>cantidad;
                    }while(cantidad < territorioAtacante.getUnidades());
                    Jugador defensor = Jugador(" ", " "); 
                    for(Jugador j : jugadores){
                        if(territorioDefensor.getColorOcupante() == j.getColor()){
                            defensor = j;
                        }
                    }
                    defensor.eliminarTerritorio(territorioDefensor.getCodigo());
                    territorioDefensor.setColorOcupante(jugadores[estado.getTurno()].getColor());
                    territorioDefensor.setUnidades(territorioDefensor.getUnidades()+cantidad);
                    territorioAtacante.setUnidades(territorioAtacante.getUnidades()-cantidad);
                    jugadores[estado.getTurno()].agegarTerritorio(territorioDefensor);
                    break;
                }
            }else{
                territorioAtacante.setUnidades(territorioAtacante.getUnidades()-1);
                if(territorioAtacante.getUnidades()==1){
                    std::cout << "El territorio " << territorioDefensor.getNombre() << " ya no puede atacar mas al quedarse con solo 1 unidad" << std::endl;
                    break;
                }
            }

        }else{
            std::cout << "El territorio seleccionado no es adyacente al territorio atacante"<<std::endl;
            continue;
        }    
        }else{
            std::cout << "El jugador " << tokens[1] << " no es dueño de este territorio" << std::endl;
        }
        std::cout << "Desea seguir atacando? (si/no)" << std::endl;
        std::cin >> respuesta;
        }while(trim(respuesta) != "no");
    }
}
void EjecutorConfig::fortificar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda fortificar' para ver el uso correcto." << std::endl;
        return;
    }
    std::vector<Jugador> jugadores = estado.getJugadores();
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
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
    }else if(!jugadores[estado.getTurno()].getHaAtacado()){
        std::cout << "El jugador " << tokens[1] << " no ha atacado." << std::endl;
        return;
    }else{
        std::cout << "El jugador " << tokens[1] << " ha terminado de fortificar su posicion." << std::endl;
    }
}
void EjecutorConfig::estadoJuego(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
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
void EjecutorConfig::ayuda(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
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
void EjecutorConfig::guardar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
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
void EjecutorConfig::guardar_comprimido(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
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
void EjecutorConfig::costoConquista(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
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
void EjecutorConfig::conquistaMasBarata(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
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