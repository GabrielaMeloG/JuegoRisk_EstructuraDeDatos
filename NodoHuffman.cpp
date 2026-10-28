#include "NodoHuffman.h"
 #include <cstddef>
#include <fstream>
#include <queue>

NodoHuffman::NodoHuffman(unsigned char s, uint64_t f, int o){
simbolo = s;
frecuencia = f;
orden = o;
izq = NULL;
der = NULL;
}
 
NodoHuffman::NodoHuffman(NodoHuffman* i, NodoHuffman* d, int o){
simbolo = 0;
frecuencia = 0;
orden = o;
izq = i;
der = d;
  if (izq != NULL)
    frecuencia += izq->frecuencia;
  if (der != NULL)
    frecuencia += der->frecuencia;
}
 
NodoHuffman::~NodoHuffman() {
  delete izq;
  delete der;
}
uint64_t NodoHuffman::obtenerFrecuencia() {
  return frecuencia;
}
int NodoHuffman::obtenerOrden() {
  return orden;
}
bool NodoHuffman::esHoja() const {
  return izq == NULL && der == NULL;
}
unsigned char NodoHuffman::obtenerSimbolo() {
    return simbolo; 
}
NodoHuffman* NodoHuffman::obtenerIzq() { 
    return izq; 
}
NodoHuffman* NodoHuffman::obtenerDer() { 
    return der; 
}