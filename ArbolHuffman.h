#ifndef _ARBOLHUFFMAN_H_
#define _ARBOLHUFFMAN_H_
 
#include <string>
#include <vector>
#include <stdint.h>
#include "NodoHuffman.h"

class ArbolHuffman {
    private:
        NodoHuffman* raiz;
        std::string codigos[256];
    public:
        ArbolHuffman();
        ~ArbolHuffman();
        ArbolHuffman(ArbolHuffman&);           
        ArbolHuffman& operator=(ArbolHuffman&);
        void generarCodigos(NodoHuffman* nodo, const std::string& camino);
        bool construir(uint64_t frecuencias[256]);
        std::string codigoDe(unsigned char simbolo);
        bool codificar(std::string& texto, std::string& bytes);
        bool decodificar(std::string& bytes, uint64_t w, std::string& texto);
 
};

#endif