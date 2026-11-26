#include "juego.h"
#include "utilidades.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>//Esta libreria esta para revolver la baraja de cartas y ordenar de mayor a menor los dados, esa libreria nos la explico la ia, luego se investigo un poco mas sobre las funciones que se estaban utilizando especificamente
#include <random>

EstadoJuego::EstadoJuego() {
 reiniciar();
}

void EstadoJuego::reiniciar() {
    inicializado = false;
    terminado = false;
    jugadores.clear();
}

bool EstadoJuego::existeJugador(std::string nombre) {
    return indiceJugador(nombre) != -1;
}

int EstadoJuego::indiceJugador(std::string nombre) {
    return buscarIndicePorNombre(jugadores, nombre);    
}
std::string Territorio::getCodigo() {
    return codigo;
}
bool EstadoJuego::existeTerritorio(std::string codigo, Tablero tablero) {
    std::vector<Continente> continentes = tablero.getContinentes();
    for (size_t i = 0; i < continentes.size(); i++) {
        std::vector<Territorio> territorios = continentes[i].getTerritorios();
        for (size_t j = 0; j < territorios.size(); j++) {
            if (territorios[j].getCodigo() == codigo) {
                return true;
            }
        }
    }
    return false;
}
void EstadoJuego::setInicializado(bool estado) {
    inicializado = estado;
}
void EstadoJuego::setTerminado(bool estado) {
    terminado = estado;
}
bool EstadoJuego::getInicializado() {
    return inicializado;
}
bool EstadoJuego::getTerminado() {
    return terminado;
}
Territorio::Territorio(std::string nombre, std::string id, std::vector<std::string> adyacentes) {
    this->nombre = nombre;
    codigo = id;
    territoriosAdyacentes = adyacentes;
    colorOcupante = " ";
    unidades = 0;
}
Continente::Continente(int id, std::string nombre, std::vector<Territorio> territorios) {
    codigo = id;
    this->nombre = nombre;
    this->territorios = territorios;
}
Tablero::Tablero(){
    std::ifstream archivo("territorios_risk.txt");
    if(!archivo.is_open()){
        std::cerr << "No se pudo abrir el archivo de territorios." << std::endl;
        return;
    }
    cantidadCartasEntregadas = 0;
    std::string linea;
    std::vector<Territorio> territorios;
    while(std::getline(archivo, linea)){
        std::vector<std::string> tokens = tokenizar(linea);
        if(tokens.size() == 2){
            std::string nombreContinente = tokens[0];
            int id = std::stoi(tokens[1]);
            Continente continente(id, nombreContinente, territorios);
            continentes.push_back(continente);
            territorios.clear();
        }else{
            std::string nombreContinente = tokens[0];
            std::string codigoTerritorio = tokens[1];
            std::vector<std::string> adyacentes;
            for(size_t i = 2; i < tokens.size(); i++){
                std::string codigo = tokens[i];
                if(!codigo.empty() && codigo[codigo.size() - 1] == ','){
                    codigo.erase(codigo.size() - 1);
                }
                adyacentes.push_back(codigo);
            }
            Territorio territorio(nombreContinente, codigoTerritorio, adyacentes);
            territorios.push_back(territorio);
            mapa.insertarNodo(codigoTerritorio);
        }
    }
    construirAristas();
}
std::vector<Continente>& Tablero::getContinentes() { 
    return continentes; 
}
int Tablero::indiceTerritorio(std::string codigo){
    for(size_t i = 0; i < mapa.getNodos().size(); i++){
        if(mapa.getNodo(i) == codigo){
            return i;
        }
    }
    return -1;
}

void Tablero::construirAristas(){
    for(size_t i = 0; i < mapa.getNodos().size(); i++){
        std::vector<std::string> adyacentes = getTerritorio(mapa.getNodo(i)).getTerritoriosAdyacentes();
        for(size_t k = 0; k < adyacentes.size(); k++){
            int j = indiceTerritorio(adyacentes[k]);
            if(j == -1){
                std::cerr << "Territorio adyacente no encontrado: " << adyacentes[k] << std::endl;
            }else{
                mapa.insertarArista(i, j, 1.0);
            }
        }
    }
}
std::string Jugador::getNombre(){
    return nombre;
}
std::string Continente::getNombre() { 
    return nombre;
}
std::vector<Territorio>& Continente::getTerritorios() {
     return territorios;
}
int Continente::getCodigo(){
    return codigo;
}
std::string Territorio::getNombre(){
    return nombre;
}
Jugador::Jugador(std::string nombre, std::string color) {
    this->nombre = nombre;
    this->color = color;
    unidades = 0;
    obtenidoUnidades = false;
    haAtacado = false;
}
std::vector<Jugador>& EstadoJuego::getJugadores(){
    return jugadores;
}
void Territorio::setColorOcupante(std::string color) {
    colorOcupante = color;
}
void Territorio::setUnidades(int cantidad) {
    unidades = cantidad;
}
Carta::Carta(std::string id, std::string dibujo){
    codigoTerritorio = id;
    this->dibujo = dibujo;
    esComodin = false;
}
void Carta::setComodin(bool comodin) {
    esComodin = comodin;
}
Baraja::Baraja() {
    std::ifstream archivo("cartas.txt");
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo de cartas." << std::endl;
        return;
    }
    std::vector<Carta> cartasOrdenadas;
    for(int i = 0; i < 44; i++) {
        std::string codigoTerritorio;
        std::string dibujo;
        archivo >> codigoTerritorio >> dibujo;
        if(codigoTerritorio == "1") {
            Carta carta(codigoTerritorio, dibujo);
            carta.setComodin(true);
            cartasOrdenadas.push_back(carta);
        }else{
        Carta carta(codigoTerritorio, dibujo);
        cartasOrdenadas.push_back(carta);
        }
    }
    std::random_device rd; //para esta parte se utilizo la libreria algorithm
    std::mt19937 g(rd());
    std::shuffle(cartasOrdenadas.begin(), cartasOrdenadas.end(), g);

    for(int i = 0; i < 44; i++) {
        cartas.push(cartasOrdenadas[i]);
    }
}
void Jugador::agegarTerritorio(Territorio territorio) {
    territoriosOcupados.push_back(territorio);
}
void Jugador::agregarUnidades(int cantidad) {
    unidades += cantidad;
}
void Jugador::eliminarUnidades(int cantidad) {
    unidades -= cantidad;
}

// Corregido con ayuda de Claude ya que antes el mensaje se imprimia por cada territorio revisado; ahora solo se imprime si no lo encuentra en toda la lista
void Jugador::eliminarTerritorio(std::string codigo) {
    std::vector<Territorio>::iterator it;
    for(it = territoriosOcupados.begin(); it != territoriosOcupados.end(); ++it) {
        if(it->getCodigo() == codigo) {
            territoriosOcupados.erase(it);
            return;
        }
    }

    std::cout << "El jugador no posee el territorio con codigo: " << codigo << std::endl;
}
std::string Jugador::getColor() {
    return color;
}
bool Jugador::getObtenidoUnidades() {
    return obtenidoUnidades;
}
bool Jugador::getHaAtacado() {
    return haAtacado;
}
std::vector<Territorio>& Jugador::getTerritoriosOcupados() {
    return territoriosOcupados;
}
int Jugador::getUnidades() {
    return unidades;
}
void Jugador::setObtenidoUnidades(bool estado) {
    obtenidoUnidades = estado;
}
void Jugador::setHaAtacado(bool estado) {
    haAtacado = estado;
}
std::string Territorio::getColorOcupante() {
    return colorOcupante;
}
int Territorio::getUnidades() {
    return unidades;
}
std::vector<std::string> Territorio::getTerritoriosAdyacentes() {
    return territoriosAdyacentes;
}
void EstadoJuego::siguienteTurno(int cantidadJugadores) {
    if(turnoActual == cantidadJugadores - 1) {
        turnoActual = 0;
    } else {
        turnoActual++;
    }
}
void EstadoJuego::setTurno(int turno) {
    turnoActual = turno;
}
int EstadoJuego::getTurno() {
    return turnoActual;
}
void Tablero::agregarUnidadesTerritorio(std::string codigoTerritorio, int cantidad){
    std::vector<Continente>::iterator itContinente;
    std::vector<Territorio>::iterator itTerritorio;
    for(itContinente = continentes.begin(); itContinente != continentes.end(); itContinente++){
        std::vector<Territorio>& territorios = itContinente->getTerritorios();
        for(itTerritorio = territorios.begin(); itTerritorio != territorios.end(); itTerritorio++){
            if(itTerritorio->getCodigo() == codigoTerritorio){
                itTerritorio->setUnidades(itTerritorio->getUnidades() + cantidad);
                return;
            }
        }
    }
}
void Jugador::agregarCarta(Carta carta) {
    mano.push_back(carta);
}
Carta Baraja::tomarCarta() {
    Carta carta = cartas.top();
    cartas.pop();
    return carta;
}
std::vector<Carta>& Jugador::getCartas() {
    return mano;
}
std::string Carta::getIdTerritorio() {
    return codigoTerritorio;
}
std::string Carta::getDibujo() {
    return dibujo;
}
bool Carta::getEsComodin() {
    return esComodin;
}
int Tablero::getCantidadCartasEntregadas() {
    return cantidadCartasEntregadas;
}
void Tablero::cartasEntregadas() {
    cantidadCartasEntregadas++;
}
int EstadoJuego::getUnidadesPorCartas(Tablero tablero) {
    switch (tablero.getCantidadCartasEntregadas()) {
        case 0:
            tablero.cartasEntregadas();
            return 4;
        case 1:
            tablero.cartasEntregadas();
            return 6;
        case 2:
            tablero.cartasEntregadas();
            return 8;
        case 3:
            tablero.cartasEntregadas();
            return 10;
        case 4:
            tablero.cartasEntregadas();
            return 12;
        case 5:
            tablero.cartasEntregadas();
            return 15;
        case 6:
            tablero.cartasEntregadas();
            return 20;
        default:
            tablero.cartasEntregadas();
            return 15 + (tablero.getCantidadCartasEntregadas() - 6) * 5;
    }
}
bool Jugador::poseeTerritorio(std::string codigo) {
    std::vector<Territorio>::iterator it;
    for(it = territoriosOcupados.begin(); it != territoriosOcupados.end(); ++it) {
        if(it->getCodigo() == codigo) {
            return true;
        }
    }
    return false;
}
bool EstadoJuego::compararCartas(Jugador jugador, int indices[3]){
    std::vector<Carta> cartas = jugador.getCartas();
    if(cartas[indices[0]].getDibujo() == cartas[indices[1]].getDibujo() && cartas[indices[1]].getDibujo() == cartas[indices[2]].getDibujo()){
        return true;
    }else if(cartas[indices[0]].getDibujo() == "Comodin" && cartas[indices[1]].getDibujo() == cartas[indices[2]].getDibujo()){
        return true;
    }else if(cartas[indices[1]].getDibujo() == "Comodin" && cartas[indices[0]].getDibujo() == cartas[indices[2]].getDibujo()){
        return true;
    }else if(cartas[indices[2]].getDibujo() == "Comodin" && cartas[indices[0]].getDibujo() == cartas[indices[1]].getDibujo()){
        return true;
    }else if(cartas[indices[0]].getDibujo() == "Comodin" && cartas[indices[1]].getDibujo() == "Comodin"){
        return true;
    }else if(cartas[indices[0]].getDibujo() == "Comodin" && cartas[indices[2]].getDibujo() == "Comodin"){
        return true;
    }else if(cartas[indices[1]].getDibujo() == "Comodin" && cartas[indices[2]].getDibujo() == "Comodin"){
        return true;
    }else{
        return false;
    }
}
void Jugador::quitarCartas(int indices[3]) {
    std::vector<Carta> nuevasCartas;
    for(int i = 0; i < mano.size(); i++) {
        if(i != indices[0] && i != indices[1] && i != indices[2]) {
            nuevasCartas.push_back(mano[i]);
        }
    }
    mano = nuevasCartas;
}
Territorio::Territorio(){
    this->nombre = " ";
    codigo = " ";
    colorOcupante = " ";
    unidades = 0;
}
 bool Territorio::esAdyacente(std::string codigo){
    std::vector<std::string>::iterator it;
    for(it = territoriosAdyacentes.begin(); it != territoriosAdyacentes.end(); ++it) {
        if(*it == codigo) {
            return true;
        }
    }
    return false;
 }
Territorio& Tablero::getTerritorio(std::string codigoTerritorio){
    std::vector<Continente>::iterator itContinente;
    std::vector<Territorio>::iterator itTerritorio;
    for(itContinente = continentes.begin(); itContinente != continentes.end(); itContinente++){
        std::vector<Territorio>& territorios = itContinente->getTerritorios();
        for(itTerritorio = territorios.begin(); itTerritorio != territorios.end(); itTerritorio++){
            if(itTerritorio->getCodigo() == codigoTerritorio){
                return *itTerritorio;
            }
        }
    }
    throw std::runtime_error("Territorio no encontrado: " + codigoTerritorio);
}
bool EstadoJuego::compararDados(int dadosAtacante[3], int dadosDefensor[2]){
    std::sort(dadosAtacante, dadosAtacante + 3, std::greater<int>());// para esta parte se utilizo la libreria algorithm
    std::sort(dadosDefensor, dadosDefensor + 2, std::greater<int>());

    int perdidasAtacante = 0;
    int perdidasDefensor = 0;

    for(int i = 0; i < 2; i++){
        if(dadosAtacante[i] <= dadosDefensor[i]){ 
            perdidasAtacante++;
        }else{
            perdidasDefensor++;
        }
    }
    if(perdidasAtacante < perdidasDefensor){
        return true;
    }else{
        return false;
    }
}
bool Jugador::tieneTerritorioParaAtacar() {
    for (Territorio t : territoriosOcupados) {
        if (t.getUnidades() > 1) {
            return true;
        }
    }
    return false;
}
bool EstadoJuego::Victoria(Jugador jugador) {
    if(jugador.getTerritoriosOcupados().size() == 42){
        return true;
    }
    return false;
}
int EstadoJuego::calcularBonoContinentes(Jugador jugador, Tablero tablero) {
    int unidadesExtras = 0;
    std::vector<Continente>& continentes = tablero.getContinentes();
    int unidadesExtrasPorContinente[6] = {5, 2, 5, 3, 7, 2}; 
    std::vector<Continente>::iterator itContinente;
    std::vector<Territorio>::iterator itTerritorio;
    for (itContinente = continentes.begin() ; itContinente != continentes.end() ; itContinente++) {
        std::vector<Territorio>& territorios = itContinente->getTerritorios();
        bool tieneTodo = true;
        for (itTerritorio = territorios.begin() ; itTerritorio != territorios.end() ; itTerritorio++) {
            if (itTerritorio->getColorOcupante() != jugador.getColor()) {
                tieneTodo = false;
                break;
            }
        }
        if (tieneTodo) {
            int idContinente = itContinente->getCodigo();
            unidadesExtras += unidadesExtrasPorContinente[idContinente];
        }
    }
    return unidadesExtras;
}
bool Tablero::existeTerritorioEnMapa(std::string codigo) { // implementacion funcion utilizada en validar estructura a partir de la explicacion de claude
    std::vector<Continente>::iterator itContinente;
    std::vector<Territorio>::iterator itTerritorio;
    for (itContinente = continentes.begin(); itContinente != continentes.end(); itContinente++) {
        std::vector<Territorio>& territorios = itContinente->getTerritorios();
        for (itTerritorio = territorios.begin(); itTerritorio != territorios.end(); itTerritorio++) {
            if (itTerritorio->getCodigo() == codigo) {
                return true;
            }
        }
    }
    return false;
}
bool Tablero::todosLosTerritoriosCubiertos(std::vector<std::string>& codigosVistos) { // implementacion funciones utilizadas en validar estructura a partir de la explicacion de claude
    int totalEnMapa = 0;;
    std::vector<Continente>::iterator itContinente;
    for (itContinente = continentes.begin(); itContinente != continentes.end(); itContinente++) {
        totalEnMapa += itContinente->getTerritorios().size();
    }
    if (codigosVistos.size() != totalEnMapa) {
        return false;
    }
    std::vector<Territorio>::iterator itTerritorio;
    std::vector<Continente>::iterator itCont;
    for (itCont = continentes.begin(); itCont != continentes.end(); itCont++) {
        std::vector<Territorio>& territorios = itCont->getTerritorios();
        for (itTerritorio = territorios.begin(); itTerritorio != territorios.end(); itTerritorio++) {
            bool encontrado = false;
            for (std::string codigo : codigosVistos) {
                if (codigo == itTerritorio->getCodigo()) {
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) {
                return false;
            }
        }
    }

    return true;
}
void EstadoJuego::mostrarCodigosYNombres(Tablero& tablero) {
    for (Continente& continente : tablero.getContinentes()) {
        for (Territorio& t : continente.getTerritorios()) {
            std::cout << "  " << t.getCodigo() << " - " << t.getNombre() << std::endl;
        }
    }
}
Grafo<std::string>& Tablero::getMapa(){
    return mapa;
}

void Tablero::dijkstraConquista(std::string color, std::vector<double>& dist, std::vector<int>& padre){
    int n = mapa.cantidadNodos();
    double INF = 1000000000;
    dist.assign(n, INF);
    padre.assign(n, -1);
    std::vector<bool> definitivo(n, false);
    std::vector<std::string> colores(n);
    std::vector<int> unidades(n);
    for(int i = 0; i < n; i++){
        Territorio& territorio = getTerritorio(mapa.getNodo(i));
        colores[i] = territorio.getColorOcupante();
        unidades[i] = territorio.getUnidades();
        if(colores[i] == color){
            dist[i] = 0;
        }
    }
    for(int paso = 0; paso < n; paso++){
        int u = -1;
        double menor = INF;
        for(int v = 0; v < n; v++){
            if(!definitivo[v] && dist[v] < menor){
                menor = dist[v];
                u = v;
            }
        }
        if(u == -1){
            break;
        }
        definitivo[u] = true;
        for(int v = 0; v < n; v++){
            if(mapa.existeArista(u, v) && colores[v] != color){
                double nuevo = dist[u] + unidades[v];
                if(nuevo < dist[v]){
                    dist[v] = nuevo;
                    padre[v] = u;
                }
            }
        }
    }
}
int Tablero::buscarTerritorio(std::string texto){
    std::string buscado = pasarAMinusculas(texto);
    for(int i = 0; i < mapa.cantidadNodos(); i++){
        std::string codigo = mapa.getNodo(i);
        std::string nombre = getTerritorio(codigo).getNombre();
        if(pasarAMinusculas(nombre) == buscado || codigo == texto){
            return i;
        }
    }
    return -1;
}