#include "Grafo.h"
#include <string>
#include <iostream>

// Definicion de la constante (necesaria si se compila con C++11/C++14)
template <typename T>
constexpr double Grafo<T>::SIN_ARISTA;

template <typename T>
Grafo<T>::Grafo() {
    this->dirigido = false;
}

template <typename T>
Grafo<T>::Grafo(bool dirigido) {
    this->dirigido = dirigido;
}

template <typename T>
int Grafo<T>::cantidadNodos(){
    return (int)nodos.size();
}

template <typename T>
bool Grafo<T>::indiceValido(int i){
    return i >= 0 && i < (int)nodos.size();
}

// Agrega un vertice: crece la lista, y la matriz con una fila y una columna nuevas
template <typename T>
int Grafo<T>::insertarNodo(T& dato) {
    nodos.push_back(dato);
    int n = (int)nodos.size();
    for (int i = 0; i < n - 1; i++) {
        matriz[i].push_back(SIN_ARISTA);
    }
    matriz.push_back(std::vector<double>(n, SIN_ARISTA));
    return n - 1;
}

template <typename T>
T& Grafo<T>::getNodo(int i) {
    return nodos[i];
}

template <typename T>
bool Grafo<T>::insertarArista(int origen, int destino, double costo) {
    if (!indiceValido(origen) || !indiceValido(destino)) return false;
    matriz[origen][destino] = costo;
    if (!dirigido) matriz[destino][origen] = costo;
    return true;
}


template <typename T>
bool Grafo<T>::eliminarArista(int origen, int destino) {
    if (!indiceValido(origen) || !indiceValido(destino)) return false;
    matriz[origen][destino] = SIN_ARISTA;
    if (!dirigido) matriz[destino][origen] = SIN_ARISTA;
    return true;
}

template <typename T>
bool Grafo<T>::existeArista(int origen, int destino){
    if (!indiceValido(origen) || !indiceValido(destino)) return false;
    return matriz[origen][destino] != SIN_ARISTA;
}

template <typename T>
double Grafo<T>::costoArista(int origen, int destino){
    if (!indiceValido(origen) || !indiceValido(destino)) return SIN_ARISTA;
    return matriz[origen][destino];
}

template <typename T>
std::vector<int> Grafo<T>::vecinos(int nodo){
    std::vector<int> r;
    if (!indiceValido(nodo)) return r;
    for (int j = 0; j < (int)nodos.size(); j++) {
        if (matriz[nodo][j] != SIN_ARISTA) r.push_back(j);
    }
    return r;
}

template <typename T>
std::vector<T>& Grafo<T>::getNodos() {
    return nodos;
}

// Reconstruye el camino desde el origen hasta destino usando el vector padre (padre[origen] = -1)
template <typename T>
std::vector<int> Grafo<T>::camino(std::vector<int> padre, int destino){
    std::vector<int> ruta;
    int v = destino;
    while(v != -1){
        ruta.insert(ruta.begin(), v);
        v = padre[v];
    }
    return ruta;
}

// Instanciacion explicita: genera el codigo de Grafo<std::string> en este .cpp
template class Grafo<std::string>;