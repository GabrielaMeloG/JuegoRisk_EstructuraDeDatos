#include "ejecutor_config.h"
#include "utilidades.h"
#include "ArbolHuffman.h"
#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <ctime> 
#include <limits> //esta libreria esta aqui para todas las partes en donde se utiliza std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n') , esto fue propuesto por chat para solucionar el problema de que al usar el cin, quedaba un \n en el buffer 

// Prototipos de las funciones auxiliares de guardar 
static bool terminaEnBin(const std::string& nombre);
static std::string generarTextoPartida(EstadoJuego &estado, Tablero &tablero);
static bool leerEntero(std::istream& in, uint64_t& valor, int bytes);
bool leerComprimido(std::string& nombreArchivo, std::string& contenido);
bool guardarComprimido(std::string& nombreArchivo, std::string& contenido);
void contarFrecuencias(const std::string& texto, uint64_t frecuencias[256]);




//Esta funcion de validar estructura se le pidio a claude utilizando un prompt en el que tomaba en cuenta la estructura del documento de 
//inicializacion para generar una funcion que validara su estructura al completo.
bool EjecutorConfig::validarEstructuraArchivo(std::istream& archivo, Tablero& tablero, std::string& errorMsg) {
    int cantidadJugadores;
    if (!(archivo >> cantidadJugadores)) {
        errorMsg = "La primera linea debe ser un numero entero (cantidad de jugadores).";
        return false;
    }
    if (cantidadJugadores < 3 || cantidadJugadores > 6) {
        errorMsg = "Cantidad de jugadores invalida (debe ser 3-6). Se encontro: " + std::to_string(cantidadJugadores);
        return false;
    }
    std::vector<std::string> coloresUsados;
    std::vector<std::string> coloresPermitidos = {"rojo", "azul", "verde", "amarillo", "morado", "negro"};
    for (int i = 0; i < cantidadJugadores; i++) {
        std::string nombre, color;
        if (!(archivo >> nombre >> color)) {
            errorMsg = "Linea de jugador " + std::to_string(i + 1) + " incompleta o mal formada.";
            return false;
        }
        bool colorValido = false;
        for (auto& c : coloresPermitidos) if (c == color) colorValido = true;
        if (!colorValido) {
            errorMsg = "Color invalido para jugador '" + nombre + "': " + color;
            return false;
        }
        for (auto& c : coloresUsados) {
            if (c == color) {
                errorMsg = "Color repetido entre jugadores: " + color;
                return false;
            }
        }
        coloresUsados.push_back(color);
    }
    std::vector<std::string> codigosVistos;
    std::vector<int> unidadesPorJugador(cantidadJugadores, 0);

    for (int i = 0; i < 42; i++) {
        std::string codigo, color;
        int unidades;
        if (!(archivo >> codigo >> color >> unidades)) {
            errorMsg = "Territorio #" + std::to_string(i + 1) + " incompleto o el archivo tiene menos de 42 territorios.";
            return false;
        }
        if (unidades <= 0) {
            errorMsg = "Unidades invalidas (<=0) en territorio " + codigo;
            return false;
        }
        if (!tablero.existeTerritorioEnMapa(codigo)) {
            errorMsg = "El codigo de territorio '" + codigo + "' no existe en el mapa del tablero.";
            return false;
        }
        int idxColor = -1;
        for (size_t j = 0; j < coloresUsados.size(); j++) {
            if (coloresUsados[j] == color) { idxColor = (int)j; break; }
        }
        if (idxColor == -1) {
            errorMsg = "Territorio " + codigo + " tiene color '" + color + "' que no pertenece a ningun jugador.";
            return false;
        }
        unidadesPorJugador[idxColor] += unidades;
        for (auto& c : codigosVistos) {
            if (c == codigo) {
                errorMsg = "Codigo de territorio repetido: " + codigo;
                return false;
            }
        }
        codigosVistos.push_back(codigo);
    }
    if (!tablero.todosLosTerritoriosCubiertos(codigosVistos)) {
        errorMsg = "Faltan territorios del mapa por asignar en el archivo.";
        return false;
    }
    int esperado = -1;
    switch (cantidadJugadores) {
        case 3: esperado = 35; break;
        case 4: esperado = 30; break;
        case 5: esperado = 25; break;
        case 6: esperado = 20; break;
    }
    for (int i = 0; i < cantidadJugadores; i++) {
        if (unidadesPorJugador[i] != esperado) {
            errorMsg = "El jugador con color '" + coloresUsados[i] + "' tiene " +
                       std::to_string(unidadesPorJugador[i]) + " unidades, pero deberia tener " +
                       std::to_string(esperado) + ".";
            return false;
        }
    }
    std::string sobrante;
    if (archivo >> sobrante) {
        errorMsg = "El archivo tiene contenido extra despues de los 42 territorios esperados.";
        return false;
    }

    return true;
}

void EjecutorConfig::inicializarJuego(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda inicializar' para ver el uso correcto." << std::endl;
        return;
    }
    if(estado.getInicializado()){
        std::cout << "El juego ya ha sido inicializado." << std::endl;
        return;
    }
    std::string ruta=tokens[1];
    std::string contenido;
    if(terminaEnBin(ruta)){
        if(!leerComprimido(ruta, contenido)){
            std::cout << "Error al leer el archivo: " << ruta << std::endl;
            return;
        }
    }else{
        std::ifstream archivoTexto(ruta);
        if(!archivoTexto.is_open()){
            std::cout << ruta << " no se encuentra o no puede leerse." << std::endl;
            return;
        }
        std::stringstream buffer;
        buffer << archivoTexto.rdbuf();
        contenido = buffer.str();
        archivoTexto.close();
    }
    if(contenido.empty()){
        std::cout << ruta << " no contiene información." << std::endl;
        return;
    }
    std::string errorMsg;
    std::istringstream validacion(contenido);
    if(!validarEstructuraArchivo(validacion, tablero, errorMsg)){
        std::cout << "Archivo invalido: " << errorMsg << std::endl;
        return;
    }
    std::istringstream archivo(contenido);
    int cantidadJugadores;
    archivo >> cantidadJugadores;
    if(cantidadJugadores < 3 || cantidadJugadores > 6){
        std::cout << "Cantidad de jugadores no valida. Debe ser entre 3 y 6." << std::endl;
        return;
    }
    for(int i = 0 ; i < cantidadJugadores; i++){
        std::string nombreJugador;
        std::string colorJugador;
        archivo >> nombreJugador >> colorJugador;
        Jugador jugador = Jugador(nombreJugador, colorJugador);
        estado.getJugadores().push_back(jugador);
    }
    for(int i = 0 ; i < 42 ; i++){
        std::string codigoTerritorio;
        std::string colorOcupante;
        int unidades;
        archivo >> codigoTerritorio >> colorOcupante >> unidades;
        std::vector<Continente>& continentes = tablero.getContinentes();
        std::vector<Continente>::iterator itContinente;
        std::vector<Territorio>::iterator itTerritorio;
        for(itContinente = continentes.begin(); itContinente != continentes.end();itContinente++){
            std::vector<Territorio>& territorios = itContinente->getTerritorios();
            for(itTerritorio = territorios.begin(); itTerritorio != territorios.end(); itTerritorio++){
                if(itTerritorio->getCodigo() == codigoTerritorio){
                    itTerritorio->setColorOcupante(colorOcupante);
                    itTerritorio->setUnidades(unidades);
                }
            }
        }
    }
    std::vector<Jugador>& jugadores = estado.getJugadores();
    for(Jugador& j : jugadores){
        std::vector<Continente>& continentes = tablero.getContinentes();
        for(Continente& continente : continentes){
            std::vector<Territorio>& territorios = continente.getTerritorios();
            for(Territorio& territorio : territorios){
                if(territorio.getColorOcupante() == j.getColor()){
                    j.agegarTerritorio(territorio);
                    j.agregarUnidades(territorio.getUnidades());
                }
            }
        }
    }

    switch(cantidadJugadores){
        case 3:
            for(Jugador& j : estado.getJugadores()){
                if(j.getUnidades() != 35){
                    std::cout << "Error: Hay jugadores que no tienen 35 unidades." << std::endl;
                    estado.reiniciar();
                    return;
                }
            }
            break;
        case 4:
            for(Jugador& j : estado.getJugadores()){
                if(j.getUnidades() != 30){
                    std::cout << "Error: Hay jugadores que no tienen 30 unidades." << std::endl;
                    estado.reiniciar();
                    return;
                }
            }
            break;
        case 5:
            for(Jugador& j : estado.getJugadores()){
                if(j.getUnidades() != 25){
                    std::cout << "Error: Hay jugadores que no tienen 25 unidades." << std::endl;
                    estado.reiniciar();
                    return;
                }
            }
            break;
        case 6:
            for(Jugador& j : estado.getJugadores()){
                if(j.getUnidades() != 20){
                    std::cout << "Error: Hay jugadores que no tienen 20 unidades." << std::endl;
                    estado.reiniciar();
                    return;
                }
            }
            break;
    }
    std::vector<Jugador>::iterator itJugador;
    for(itJugador = estado.getJugadores().begin(); itJugador != estado.getJugadores().end(); itJugador++){
        for(int i = 0; i < 3; i++){
            Carta carta = baraja.tomarCarta();
            itJugador->agregarCarta(carta);
        }
    }
    estado.setInicializado(true);
    estado.setTurno(0);
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
        std::vector<Jugador>& jugadores = estado.getJugadores();
        std::vector<Territorio> territorios = jugadores[estado.getTurno()].getTerritoriosOcupados();
        int unidadesAgregadas = territorios.size() / 3;
        if(unidadesAgregadas < 3){
            unidadesAgregadas = 3;
            std::cout<< "El jugador " << tokens[1] << " puede reclamar " << unidadesAgregadas << " unidades." << std::endl;
        }else{
            std::cout<< "El jugador " << tokens[1] << " puede reclamar " << unidadesAgregadas << " unidades." << std::endl;
        }
        if(jugadores[estado.getTurno()].getCartas().size() >= 3){
            std::cout << "Desea utilizar cartas para obtener unidades adicionales? (si/no)" << std::endl;
            std::string respuesta;
            std::cin >> respuesta;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if(trim(respuesta) == "si"){
                do{ 
                std::vector<Jugador>& jugadores = estado.getJugadores();
                Jugador& jugador = jugadores[estado.getTurno()];
                std::vector<Carta>& cartasJugador = jugador.getCartas();
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
                    continue;
                }
                int indices[3];
                bool valido = true;
                for(int i = 0; i < 3; i++){
                    indices[i] = std::stoi(tokensCartas[i]) - 1;
                    if(indices[i] < 0 || indices[i] >= cartasJugador.size()){
                        std::cout << "Indice de carta no valido: " << tokensCartas[i] << std::endl;
                        valido = false;
                    }
                }
                if(!valido){
                    continue;
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
                std::cout << "Desea volver utilizar cartas? (si/no)" << std::endl;
                std::cin >> respuesta;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }while(pasarAMinusculas(respuesta) != "no"); 
            }
        }
        if(estado.calcularBonoContinentes(jugadores[estado.getTurno()], tablero) > 0){ 
            unidadesAgregadas += estado.calcularBonoContinentes(jugadores[estado.getTurno()], tablero);
            std::cout << "El jugador "<< jugadores[estado.getTurno()].getNombre() <<" a recibido unidades adicionales por tener continentes enteros dandole un total de " << unidadesAgregadas << " unidades para ubicar" << std::endl;
        }else{
            std::cout << "El jugador " << jugadores[estado.getTurno()].getNombre() << " tiene un total de " << unidadesAgregadas << " unidades para ubicar" << std::endl;
        }
        std::cout<<"presione ENTER para continuar....";
        std::cin.get();
        estado.mostrarCodigosYNombres(tablero);
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
    std::vector<Jugador>& jugadores = estado.getJugadores();
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
        std::cout << "Desea realizar un ataque? (si/no)" << std::endl;
        std::cin>>respuesta;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        do{ 
        if(pasarAMinusculas(respuesta) == "no" ){
            jugadores[estado.getTurno()].setHaAtacado(true);
            break;
        }
        if(!jugadores[estado.getTurno()].tieneTerritorioParaAtacar()){
            std::cout << "No tienes territorios con suficientes unidades para realizar ataques" << std::endl;
            jugadores[estado.getTurno()].setHaAtacado(true);
            break;
        }
        estado.mostrarCodigosYNombres(tablero);
        std::cout << "Desde que territorio desea atacar? (Ingrese el codigo del territorio)" << std::endl;
        std::cin >> territorioId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(jugadores[estado.getTurno()].poseeTerritorio(territorioId)){
            std::vector<Territorio>& territorios = jugadores[estado.getTurno()].getTerritoriosOcupados();
            std::vector<Territorio>::iterator it;
            Territorio& territorioAtacante = tablero.getTerritorio(territorioId);
            int unidades = territorioAtacante.getUnidades();
            if(territorioAtacante.getUnidades() == 1){
                std::cout << "El territorio seleccionado no tiene suficientes unidades para atacar" << std::endl;
                continue;
            }
        
        std::cout << "Que territorio desea atacar? (Ingrese el codigo del territorio)" << std::endl;
        std::cin >> territorioId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      
        while(!tablero.existeTerritorioEnMapa(territorioId)){
            std::cout << "El territorio " << territorioId << " no existe." << std::endl;
            std::cout << "Que territorio desea atacar? (Ingrese el codigo del territorio)" << std::endl;
            std::cin >> territorioId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        Territorio& territorioDefensor = tablero.getTerritorio(territorioId);
        if(jugadores[estado.getTurno()].poseeTerritorio(territorioId)){
            std::cout << " no se puede atacar un territorio que ya esta conquistado" << std::endl;
            continue;
        }else if(territorioAtacante.esAdyacente(territorioId)){
            int dadosAtacante[3];
            int dadosDefensor[2];
            for(int i = 0 ; i < 3; i++){
                dadosAtacante[i] = std::rand() % 6 + 1;
            }
            for(int i = 0 ; i < 2; i++){
                dadosDefensor[i] = std::rand() % 6 + 1;
            }
            std::cout << "Dados del atacante: ";
            for(int i = 0; i < 3; i++){
                std::cout << dadosAtacante[i] << " ";
            }
            std::cout << std::endl;
            std::cout << "Dados del defensor: ";
            for(int i = 0; i < 2; i++){
                std::cout << dadosDefensor[i] << " ";
            }
            std::cout << std::endl;
            bool ganadorAtacante = estado.compararDados(dadosAtacante, dadosDefensor);
            if(ganadorAtacante){
                std::cout << "Resultado: el atacante gana el enfrentamiento. El defensor pierde 1 unidad." << std::endl;
                territorioDefensor.setUnidades(territorioDefensor.getUnidades()-1);
                if(territorioDefensor.getUnidades()==0){
                    
                    std::cout << "El territorio " << territorioDefensor.getNombre() << " ha sido conquistado por " << tokens[1] << std::endl;
                        int maximo = territorioAtacante.getUnidades() - 1;
                        int cantidad = 0;
                        do{
                            std::cout << "Cuantas unidades desea mover a ese territorio? (entre 1 y " << maximo << ")" << std::endl;
                            std::cin >> cantidad;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            if(cantidad < 1 || cantidad > maximo){
                                std::cout << "Cantidad no valida. Debe mover entre 1 y " << maximo << " unidades." << std::endl;
                            }
                        }while(cantidad < 1 || cantidad > maximo);

                        Jugador* defensor = nullptr; //Esta solucion fue dada por claude al preguntarle sobre un error de compilacion, propuso esta solucion para utilizar la variable como referencia y no como copia
                        for(Jugador& j : jugadores){
                            if(territorioDefensor.getColorOcupante() == j.getColor()){
                                defensor = &j;
                                break;
                            }   
                        }
                        if(defensor != nullptr){ 
                            defensor->eliminarTerritorio(territorioDefensor.getCodigo());
                        }
                        territorioDefensor.setColorOcupante(jugadores[estado.getTurno()].getColor());
                        territorioDefensor.setUnidades(territorioDefensor.getUnidades()+cantidad);
                        territorioAtacante.setUnidades(territorioAtacante.getUnidades()-cantidad);
                        jugadores[estado.getTurno()].agegarTerritorio(territorioDefensor);
                        jugadores[estado.getTurno()].agregarCarta(baraja.tomarCarta());
                        std::cout << "El territorio " << territorioDefensor.getNombre() << " ahora es de " << tokens[1] << " con " << territorioDefensor.getUnidades() << " unidades. " << territorioAtacante.getNombre() << " queda con " << territorioAtacante.getUnidades() << " unidades." << std::endl;
                        std::cout << tokens[1] << " recibe una carta por conquistar un territorio." << std::endl;
                        if (estado.Victoria(jugadores[estado.getTurno()])) {
                            std::cout << jugadores[estado.getTurno()].getNombre() << " ha conquistado todos los territorios y GANA LA PARTIDA" << std::endl;
                            estado.setTerminado(true);
                            return; 
                        }
                    }
            }else{
                std::cout << "Resultado: el defensor gana el enfrentamiento. El atacante pierde 1 unidad." << std::endl;
                territorioAtacante.setUnidades(territorioAtacante.getUnidades()-1);
                if(territorioAtacante.getUnidades()==1){
                    std::cout << "El territorio " << territorioAtacante.getNombre() << " ya no puede atacar mas al quedarse con solo 1 unidad" << std::endl;
                }
            }

        }else{
            std::cout << "El territorio seleccionado no es adyacente al territorio atacante"<<std::endl;
            continue;
        }    
        }else if(!tablero.existeTerritorioEnMapa(territorioId)){
            std::cout << "El territorio " << territorioId << " no existe." << std::endl;
        }else{
            std::cout << "El jugador " << tokens[1] << " no es dueño de este territorio" << std::endl;
        }
        std::cout << "Desea seguir atacando? (si/no)" << std::endl;
        std::cin >> respuesta;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }while(pasarAMinusculas(respuesta) != "no");
        jugadores[estado.getTurno()].setHaAtacado(true);
        std::cout << "El jugador " << tokens[1] << " ha terminado de atacar." << std::endl;
    }
    
}
void EjecutorConfig::fortificar(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda fortificar' para ver el uso correcto." << std::endl;
        return;
    }
    std::vector<Jugador>& jugadores = estado.getJugadores();
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
        bool salir = false;
        std::string territorioId;
        std::string respuesta; 
        std::cout << "Desea fortificar su posicion? (si/no)" << std::endl;
        std::cin>>respuesta;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(pasarAMinusculas(respuesta) == "no" ){
            estado.siguienteTurno(estado.getJugadores().size());
            return;
        }
        if(!jugadores[estado.getTurno()].tieneTerritorioParaAtacar()){
            std::cout << "No tienes territorios con suficientes unidades para fortificar a otros" << std::endl;
            estado.siguienteTurno(estado.getJugadores().size());
            return;
        }
        estado.mostrarCodigosYNombres(tablero);
        do{
        std::cout << "Que territorio desea fortificar? (Ingrese el codigo del territorio)" << std::endl;
        std::cin >> territorioId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        Territorio& fortificado = tablero.getTerritorio(territorioId);
        if(!jugadores[estado.getTurno()].poseeTerritorio(territorioId)){ 
            std::cout << "Usted no es dueño de este territorio" << std::endl;
            continue;
        }
        std::cout << "Desde que territorio desea fortificar? (Ingrese el codigo del territorio)" << std::endl;
        std::cin >> territorioId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(jugadores[estado.getTurno()].poseeTerritorio(territorioId) && fortificado.esAdyacente(territorioId)){
            std::vector<Territorio> territorios = jugadores[estado.getTurno()].getTerritoriosOcupados();
            std::vector<Territorio>::iterator it;
            int unidades;
            Territorio fortificador = Territorio();
            for(it = territorios.begin();it!=territorios.end();it++){
                if(it->getCodigo()==territorioId){
                    unidades = it->getUnidades();
                    fortificador = *it;
                    if(unidades == 1){
                        std::cout <<"El territorio seleccionado no tiene suficientes unidades para fortificar a otro"<<std::endl;
                        continue;
                    }
                }
            }
                int cantidad;
                do{
                std::cout << "Cuantas unidades desea mover a ese territorio?"<<std::endl;
                std::cin>>cantidad;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }while(cantidad < fortificador.getUnidades());
                fortificado.setUnidades(fortificado.getUnidades()+cantidad);
                fortificador.setUnidades(fortificador.getUnidades()-cantidad);
                std::cout << "Se ha fortificado el territorio " << fortificado.getNombre() << " con " << cantidad << " unidades provenientes del territorio " << fortificador.getNombre() << std::endl;
                estado.siguienteTurno(estado.getJugadores().size());
                salir = true;
            }else{
            if(!jugadores[estado.getTurno()].poseeTerritorio(territorioId)){
            std::cout << "Usted no es dueño de este territorio" << std::endl;

            }else{
                std::cout << "El territorio seleccionado no es adyacente al que se quiere fortificar " << std::endl;
            }
            continue;
            }
        }while(!salir);
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
        std::vector<Jugador>& jugadores=estado.getJugadores();
        std::cout << "Cantidad de jugadores: " << jugadores.size() <<std::endl;
        std::cout << "Turno actual del jugador: " << jugadores[estado.getTurno()].getNombre() << std::endl;
        std::cout << "Jugadores: " << std::endl;
        for(Jugador j : jugadores){
            std::cout << "Nombre: "<<j.getNombre() << " Color: " << j.getColor()<<std::endl;
        }
        std::cout << "Territorios: " << std::endl;
        std::vector<Continente>& continentes = tablero.getContinentes();
        std::vector<Continente>::iterator itContinente;
        std::vector<Territorio>::iterator itTerritorio;
        for(itContinente = continentes.begin(); itContinente != continentes.end(); itContinente++){
            std::cout << std::endl << itContinente->getNombre() << std::endl;
            std::vector<Territorio>& territorios = itContinente->getTerritorios();
        for(itTerritorio = territorios.begin(); itTerritorio != territorios.end(); itTerritorio++){
            std::cout << itTerritorio->getNombre() << " " << " ocupado por el color " << itTerritorio->getColorOcupante() << std::endl;
        }
    }
        
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
    }
    std::ofstream archivo(tokens[1]);
    if(!archivo.is_open()){
        std::cout << "La partida no ha sido guardada correctamente." << std::endl;
        return;
    }
    archivo << generarTextoPartida(estado, tablero);
    archivo.close();
    if(archivo.fail()){
        std::cout << "La partida no ha sido guardada correctamente." << std::endl;
        return;
    }
    std::cout << "La partida ha sido guardada correctamente." << std::endl;
}
void EjecutorConfig::guardar_comprimido(EstadoJuego &estado, std::vector<std::string> &tokens, Tablero &tablero, Baraja &baraja){
    if(tokens.size() != 2){
        std::cout << "Parametros no validos. 'ayuda guardar_comprimido' para ver el uso correcto." << std::endl;
        return;
    }
    if(!estado.getInicializado()){
        std::cout << "El juego no ha sido inicializado." << std::endl;
        return;
    }
    std::string nombre = tokens[1];
    if(!terminaEnBin(nombre)){
        nombre = nombre + ".bin";   // el archivo comprimido siempre lleva extension .bin
    }
    std::string texto = generarTextoPartida(estado, tablero);
    if(guardarComprimido(nombre, texto)){
        std::cout << "La partida ha sido codificada y guardada correctamente." << std::endl;
        if(nombre != tokens[1]){
            std::cout << "Archivo: " << nombre << std::endl;
        }
    }else{
        std::cout << "La partida no ha sido codificada ni guardada correctamente." << std::endl;
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
// Dice si el nombre del archivo termina en ".bin"
static bool terminaEnBin(const std::string& nombre){
    return nombre.size() >= 4 && nombre.compare(nombre.size() - 4, 4, ".bin") == 0;
}

// Arma el texto de la partida con el mismo formato del archivo de inicializacion:
// N, una linea por jugador (nombre color) y una linea por territorio (codigo color unidades).
// Lo usan guardar y guardar_comprimido, asi los dos archivos tienen exactamente el mismo contenido.
static std::string generarTextoPartida(EstadoJuego &estado, Tablero &tablero){
    std::ostringstream texto;
    std::vector<Jugador>& jugadores = estado.getJugadores();
    texto << jugadores.size() << "\n";
    for(size_t i = 0; i < jugadores.size(); i++){
        texto << jugadores[i].getNombre() << " " << jugadores[i].getColor() << "\n";
    }
    std::vector<Continente>& continentes = tablero.getContinentes();
    for(size_t i = 0; i < continentes.size(); i++){
        std::vector<Territorio>& territorios = continentes[i].getTerritorios();
        for(size_t j = 0; j < territorios.size(); j++){
            texto << territorios[j].getCodigo() << " " << territorios[j].getColorOcupante() << " " << territorios[j].getUnidades() << "\n";
        }
    }
    return texto.str();
}
// Pasa los bytes a una cadena de '0' y '1': cada byte da 8 caracteres
static std::string desempaquetar(std::vector<unsigned char>& bytes){
  std::string bits = "";
  for (std::size_t i = 0; i < bytes.size(); i++) {
    int valor = bytes[i];
    std::string ocho = "00000000";
    for (int j = 7; j >= 0; j--) {            // de derecha a izquierda: valor % 2 es el bit
      if (valor % 2 == 1) ocho[j] = '1';
      valor = valor / 2;
    }
    bits += ocho;
  }
  return bits;
}
 
static void escribirEntero(std::ostream& out, uint64_t valor, int bytes) {
  for (int i = 0; i < bytes; i++) {           // primero el byte menos significativo
    out.put(static_cast<char>(valor % 256));
    valor = valor / 256;
  }
}
bool leerComprimido(std::string& nombreArchivo, std::string& contenido) {
  contenido = "";
  std::ifstream in(nombreArchivo.c_str(), std::ios::binary);
  if (!in) return false;
 
  uint64_t n = 0, w = 0;
  if (!leerEntero(in, n, 2)) return false;
  if (n > 256) return false;
 
  uint64_t frec[256];
  for (int c = 0; c < 256; c++) frec[c] = 0;
 
  uint64_t suma = 0;
  int anterior = -1;                                      // los caracteres vienen en orden ascendente
  for (uint64_t i = 0; i < n; i++) {
    uint64_t c = 0, f = 0;
    if (!leerEntero(in, c, 1)) return false;
    if (!leerEntero(in, f, 8)) return false;
    if (f == 0 || static_cast<int>(c) <= anterior) return false;
    anterior = static_cast<int>(c);
    frec[c] = f;
    suma = suma + f;
  }
  if (!leerEntero(in, w, 8)) return false;
  if (suma != w) return false;                            // las frecuencias deben sumar w
 
  std::vector<unsigned char> bytes;
  int c = in.get();
  while (in) {
    bytes.push_back(static_cast<unsigned char>(c));
    c = in.get();
  }
 
  if (n == 0) return w == 0;
  ArbolHuffman arbol;
  if (!arbol.construir(frec)) return false;
  std::string bits = desempaquetar(bytes);
  return arbol.decodificar(bits, w, contenido);
}
static void empaquetar(const std::string& bits, std::vector<unsigned char>& bytes) {
  bytes.clear();
  int actual = 0;
  int usados = 0;
  for (std::size_t i = 0; i < bits.size(); i++) {
    actual = actual * 2;
    if (bits[i] == '1') actual = actual + 1;
    usados++;
    if (usados == 8) {
      bytes.push_back(static_cast<unsigned char>(actual));
      actual = 0;
      usados = 0;
    }
  }
  if (usados > 0) {                           // completar el ultimo byte con ceros
    while (usados < 8) {
      actual = actual * 2;
      usados++;
    }
    bytes.push_back(static_cast<unsigned char>(actual));
  }
}
 
static bool leerEntero(std::istream& in, uint64_t& valor, int bytes) {
  valor = 0;
  uint64_t peso = 1;
  for (int i = 0; i < bytes; i++) {
    int c = in.get();
    if (!in) return false;                    // se acabo el archivo antes de tiempo
    valor = valor + c * peso;
    peso = peso * 256;
  }
  return true;
}
bool guardarComprimido(std::string& nombreArchivo, std::string& contenido) {
  uint64_t frec[256];
  contarFrecuencias(contenido, frec);
 
  int n = 0;
  for (int c = 0; c < 256; c++)
    if (frec[c] > 0) n++;
 
  std::vector<unsigned char> bytes;
  if (n > 0) {
    ArbolHuffman arbol;
    std::string bits;
    if (!arbol.construir(frec)) return false;
    if (!arbol.codificar(contenido, bits)) return false;
    empaquetar(bits, bytes);
  }
 
  std::ofstream out(nombreArchivo.c_str(), std::ios::binary);
  if (!out) return false;
 
  escribirEntero(out, static_cast<uint64_t>(n), 2);                              // n
  for (int c = 0; c < 256; c++) {
    if (frec[c] > 0) {
      escribirEntero(out, static_cast<uint64_t>(c), 1);                          // ci
      escribirEntero(out, frec[c], 8);                    // fi
    }
  }
  escribirEntero(out, static_cast<uint64_t>(contenido.size()), 8);               // w
  for (std::size_t i = 0; i < bytes.size(); i++)
    out.put(static_cast<char>(bytes[i]));                                    // binary_code
 
  out.close();
  return !out.fail();
}
void contarFrecuencias(const std::string& texto, uint64_t frecuencias[256]) {
  for (int i = 0; i < 256; i++) frecuencias[i] = 0;
  for (std::size_t i = 0; i < texto.size(); i++) {
    unsigned char c = static_cast<unsigned char>(texto[i]);   // asi el caracter queda entre 0 y 255
    frecuencias[c]++;
  }
}
 