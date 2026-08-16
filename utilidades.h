#ifndef UTILIDADES_H
#define UTILIDADES_H
#include <string>
#include <vector>
#include <sstream>
 
inline std::vector<std::string> tokenizar(std::string linea) {
    std::vector<std::string> tokens;
    std::istringstream flujo(linea);
    std::string palabra;
    while (flujo >> palabra) {
        tokens.push_back(palabra);      
    }
    return tokens;
}

template <typename T>
int buscarIndicePorNombre(std::vector<T> contenedor, std::string nombre) {
    for (size_t i = 0; i < contenedor.size(); i++) {
        if (contenedor[i].nombre == nombre) {
            return i;
        }
    }
    return -1;
}

#endif