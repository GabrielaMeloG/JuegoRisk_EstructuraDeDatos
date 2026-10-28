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
        bool codificar(const std::string& texto, std::vector<unsigned char>& bytes);
        bool decodificar(std::vector<unsigned char>& bytes, uint64_t w, std::string& texto);
        bool leer(const std::string& nombreArchivo, std::string& contenido);
        bool guardar(const std::string& nombreArchivo, const std::string& contenido);
        void escribirEntero(std::ostream& out, uint64_t valor, int bytes);
        bool leerEntero(std::istream& in, uint64_t& valor, int bytes);

};

#endif