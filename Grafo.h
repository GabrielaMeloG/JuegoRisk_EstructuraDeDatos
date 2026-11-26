#ifndef GRAFO_H
#define GRAFO_H
#include <vector>

template <typename T>
class Grafo {
private:
    std::vector<T> nodos;
    std::vector<std::vector<double> > matriz;
    bool dirigido;
public:
    static constexpr double SIN_ARISTA = -1.0;

    Grafo();
    Grafo(bool dirigido);
    int cantidadNodos();
    bool indiceValido(int i);
    int insertarNodo(T& dato);
    T& getNodo(int i);
    std::vector<T>& getNodos();
    bool insertarArista(int origen, int destino, double costo);
    bool eliminarArista(int origen, int destino);
    bool existeArista(int origen, int destino);
    double costoArista(int origen, int destino);
    std::vector<int> vecinos(int nodo);
    std::vector<int> camino(std::vector<int> padre, int destino);
};

#endif