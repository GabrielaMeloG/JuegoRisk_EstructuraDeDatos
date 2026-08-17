#ifndef UTILIDADES_H
#define UTILIDADES_H
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
 
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

inline std::string trim(std::string str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}
inline std::string pasarAMinusculas(std::string str) {
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = std::tolower(str[i]);
    }
    return str;
}
template <typename T>
inline void agregarPrimeros2(std::vector<T> &vect1, std::vector<T> &vect2){
    vect2.clear();
    vect2.push_back(vect1[0]);
    vect2.push_back(vect1[1]);
}
template <typename T>
inline void agregarPrimeros3(std::vector<T> &vect1, std::vector<T> &vect2){
    vect2.clear();
    vect2.push_back(vect1[0]);
    vect2.push_back(vect1[1]);
    vect2.push_back(vect1[2]);
}
#endif