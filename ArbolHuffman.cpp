
#include "ArbolHuffman.h"
 #include <cstddef>
#include <fstream>
#include <algorithm>
#include <deque>

struct ComparadorHuffman {
  bool operator()(NodoHuffman* a, NodoHuffman* b) {
    if (a->obtenerFrecuencia() != b->obtenerFrecuencia())
        return a->obtenerFrecuencia() > b->obtenerFrecuencia();
    return a->obtenerOrden() > b->obtenerOrden();
  }
};

ArbolHuffman::ArbolHuffman(){
    raiz=NULL;
}
 
ArbolHuffman::~ArbolHuffman() {
  delete raiz;
}
 
bool ArbolHuffman::construir(uint64_t frecuencias[256]) {
  delete raiz;
  raiz = NULL;
  for (int i = 0; i < 256; ++i) 
    codigos[i].clear();

  std::deque<NodoHuffman*> Q;
  ComparadorHuffman comparador;
  for (int s = 0; s < 256; ++s)
    if (frecuencias[s] > 0) {
        Q.push_back(new NodoHuffman(static_cast<unsigned char>(s), frecuencias[s], static_cast<unsigned int>(s)));
        std::push_heap(Q.begin(), Q.end(), comparador);  
    }
  if (Q.empty())
    return false;

  unsigned int siguiente = 256;
  if (Q.size() == 1) {
    NodoHuffman* hoja = Q.front();
    raiz = new NodoHuffman(hoja, NULL, siguiente);
  } else {
    while (Q.size() > 1) {
      std::pop_heap(Q.begin(), Q.end(), comparador);                   // extraer_min(Q)
      NodoHuffman* a = Q.back(); 
      Q.pop_back();
      std::pop_heap(Q.begin(), Q.end(), comparador);                   // extraer_min(Q)
      NodoHuffman* b = Q.back(); 
      Q.pop_back();
      Q.push_back(new NodoHuffman(a, b, siguiente++));           // z
      std::push_heap(Q.begin(), Q.end(), comparador);                  // insertar(Q, z)
    }
    raiz = Q.front();                                            // raiz
  }

  generarCodigos(raiz, "");
  return true;
}
 
void ArbolHuffman::generarCodigos(NodoHuffman* nodo, const std::string& camino) {
  if (nodo == NULL) 
    return;
  if (nodo->esHoja()) {
    codigos[nodo->obtenerSimbolo()] = camino;
  } else {
    generarCodigos(nodo->obtenerIzq(), camino + "0");
    generarCodigos(nodo->obtenerDer(), camino + "1");
  }
}
 
std::string ArbolHuffman::codigoDe(unsigned char simbolo){
  return codigos[simbolo];
}
 
bool ArbolHuffman::codificar(std::string& texto, std::string& bits) {
  bits = "";
  for (std::size_t i = 0; i < texto.size(); i++) {
    unsigned char c = static_cast<unsigned char>(texto[i]);
    if (codigos[c] == "") return false;       // caracter que no esta en el arbol
    bits += codigos[c];
  }
  return true;
}
 
bool ArbolHuffman::decodificar(std::string& bits, uint64_t w, std::string& texto) {
  texto = "";
  if (raiz == NULL) return w == 0;
 
  NodoHuffman* nodo = raiz;
  uint64_t contados = 0;
  for (std::size_t i = 0; i < bits.size() && contados < w; i++) {
    if (bits[i] == '0')
      nodo = nodo->obtenerIzq();
    else
      nodo = nodo->obtenerDer();
 
    if (nodo == NULL) return false;           // camino que no existe: archivo danado
    if (nodo->esHoja()) {
      texto += static_cast<char>(nodo->obtenerSimbolo());
      contados++;
      nodo = raiz;
    }
  }
  return contados == w;                       // los bits de relleno se ignoran
}
