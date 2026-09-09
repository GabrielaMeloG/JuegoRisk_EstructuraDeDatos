#include "juego.h"
#include "utilidades.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

EstadoJuego::EstadoJuego() {
 reiniciar();
}

void EstadoJuego::reiniciar() {
    inicializado = false;
    terminado = false;
    jugadores.clear();
    turnos.clear();
}

bool EstadoJuego::existeJugador(std::string nombre) {
    return indiceJugador(nombre) != -1;
}

int EstadoJuego::indiceJugador(std::string nombre) {
    return buscarIndicePorNombre(jugadores, nombre);    
}

bool EstadoJuego::esTurnoDe(std::string nombre) {
    if (turnos.empty()) {
        return false;
    }
    return turnos.front() == nombre;
}

void EstadoJuego::siguienteTurno() {
    if (!turnos.empty()) {
        std::string jugadorActual = turnos.front();
        turnos.pop_front();
        turnos.push_back(jugadorActual);
    }
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
                adyacentes.push_back(tokens[i]);
            }
            Territorio territorio(nombreContinente, codigoTerritorio, adyacentes);
            territorios.push_back(territorio);
        }
        
    }
    
}
std::vector<Continente> Tablero::getContinentes() { 
    return continentes; 
}
std::string Jugador::getNombre(){
    return nombre;
}
std::string Continente::getNombre() { 
    return nombre;
}
std::vector<Territorio> Continente::getTerritorios() {
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
    obtenidoUnidades = false;
    haAtacado = false;
}
std::vector<Jugador> EstadoJuego::getJugadores(){
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
    esMision = false;
}
void Carta::setComodin(bool comodin) {
    esComodin = comodin;
}
void Carta::setMision(bool mision) {
    esMision = mision;
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
        } else if(codigoTerritorio == "0") {
            Carta carta(codigoTerritorio, dibujo);
            carta.setMision(true);
            cartasOrdenadas.push_back(carta);
        } else {
        Carta carta(codigoTerritorio, dibujo);
        cartasOrdenadas.push_back(carta);
        }
    }
    std::random_device rd;
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
void Jugador::eliminarTerritorio(std::string codigo) {
    std::vector<Territorio>::iterator it;
    for(it = territoriosOcupados.begin(); it != territoriosOcupados.end(); ++it) {
        if(it->getCodigo() == codigo) {
            territoriosOcupados.erase(it);
            break;
        }else{
            std::cout << "El jugador no posee el territorio con codigo: " << codigo << std::endl;
        }
    }
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
std::vector<Territorio> Jugador::getTerritoriosOcupados() {
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
        std::vector<Territorio> territorios = itContinente->getTerritorios();
        for(itTerritorio = territorios.begin(); itTerritorio != territorios.end(); itTerritorio++){
            if(itTerritorio->getCodigo() == codigoTerritorio){
                itTerritorio->setUnidades(itTerritorio->getUnidades() + cantidad);
                return;
            }
        }
    }
}
