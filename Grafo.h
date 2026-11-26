#ifndef GRAFO_H
#define GRAFO_H
#include <vector>

// Grafo ponderado: lista de nodos + matriz de adyacencia.
// Los metodos se implementan en grafo.cpp (ahi se instancia para cada tipo T que se use).
template <typename T>
class Grafo {
private:
    std::vector<T> nodos;                          // datos de cada vertice
    std::vector<std::vector<double> > matriz;      // matriz[i][j] = costo de i->j, o SIN_ARISTA
    bool dirigido;
public:
    static constexpr double SIN_ARISTA = -1.0;

    Grafo();
    Grafo(bool dirigido);
    int cantidadNodos();
    bool indiceValido(int i);
    int insertarNodo(T& dato);               // devuelve el indice del nodo nuevo
    T& getNodo(int i);
    std::vector<T>& getNodos();
    bool insertarArista(int origen, int destino, double costo);
    bool eliminarArista(int origen, int destino);
    bool existeArista(int origen, int destino);
    double costoArista(int origen, int destino);
    std::vector<int> vecinos(int nodo);
    void imprimirMatriz();
    std::vector<int> camino(std::vector<int> padre, int destino);
};

#endif