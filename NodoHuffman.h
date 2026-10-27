#ifndef _NODOHUFFMAN_H_
#define _NODOHUFFMAN_H_
 
#include <string>
#include <vector>
#include <stdint.h>

class NodoHuffman {
    private:
        unsigned char simbolo;      // solo tiene sentido en las hojas
        uint64_t frecuencia;        // en un nodo interno: suma de las frecuencias de sus hijos
        int orden;         // desempata nodos de igual frecuencia (arbol reproducible)
        NodoHuffman* izq;           // rama "0"
        NodoHuffman* der;           // rama "1"
    public:
        NodoHuffman(unsigned char s, uint64_t f, int o);       // hoja
        NodoHuffman(NodoHuffman* i, NodoHuffman* d, int o);    // nodo interno
        ~NodoHuffman();
        bool esHoja() const;
        uint64_t obtenerFrecuencia();
        int obtenerOrden();
        unsigned char obtenerSimbolo();
        NodoHuffman* obtenerIzq();
        NodoHuffman* obtenerDer();
};
#endif