
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
      std::pop_heap(Q.begin(), Q.end(), comparador);
      NodoHuffman* a = Q.back(); 
      Q.pop_back();
      std::pop_heap(Q.begin(), Q.end(), comparador);
      NodoHuffman* b = Q.back(); 
      Q.pop_back();
      Q.push_back(new NodoHuffman(a, b, siguiente++));
      std::push_heap(Q.begin(), Q.end(), comparador);
    }
    raiz = Q.front();
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
 
bool ArbolHuffman::codificar(const std::string& texto, std::vector<unsigned char>& bytes) {
  bytes.clear();
  int actual = 0;
  int usados = 0;
  for (std::size_t i = 0; i < texto.size(); i++) {
    unsigned char c = static_cast<unsigned char>(texto[i]);
    if (codigos[c] == "")
     return false;
    for (std::size_t k = 0; k < codigos[c].size(); k++) {
      actual = actual * 2;
      if (codigos[c][k] == '1') actual = actual + 1;
      usados++;
      if (usados == 8) {
        bytes.push_back(static_cast<unsigned char>(actual));
        actual = 0;
        usados = 0;
      }
    }
  }
  if (usados > 0) {
    while (usados < 8) {
      actual = actual * 2;
      usados++;
    }
    bytes.push_back(static_cast<unsigned char>(actual));
  }
  return true;
}
 
bool ArbolHuffman::decodificar(std::vector<unsigned char>& bytes, uint64_t w, std::string& texto) {
  texto = "";
  if (raiz == NULL)
   return w == 0;
  NodoHuffman* nodo = raiz;
  uint64_t contados = 0;
  for (std::size_t i = 0; i < bytes.size() && contados < w; i++) {
    int valor = bytes[i];
    for (int j = 7; j >= 0 && contados < w; j--) {
      int bit = (valor / (1 << j)) % 2;
      if (bit == 0)
        nodo = nodo->obtenerIzq();
      else
        nodo = nodo->obtenerDer();

      if (nodo == NULL)
       return false;
      if (nodo->esHoja()) {
        texto += static_cast<char>(nodo->obtenerSimbolo());
        contados++;
        nodo = raiz;
      }
    }
  }
  return contados == w;
}
void ArbolHuffman::escribirEntero(std::ostream& out, uint64_t valor, int bytes) {
  for (int i = 0; i < bytes; i++) {
    out.put(static_cast<char>(valor % 256));
    valor = valor / 256;
  }
}

bool ArbolHuffman::leerEntero(std::istream& in, uint64_t& valor, int bytes) {
  valor = 0;
  uint64_t peso = 1;
  for (int i = 0; i < bytes; i++) {
    int c = in.get();
    if (!in) return false;
    valor = valor + c * peso;
    peso = peso * 256;
  }
  return true;
}

bool ArbolHuffman::guardar(const std::string& nombreArchivo, const std::string& contenido) {
  uint64_t frec[256];
  for (int c = 0; c < 256; c++) 
    frec[c] = 0;
  for (std::size_t i = 0; i < contenido.size(); i++) {
    unsigned char c = static_cast<unsigned char>(contenido[i]);
    frec[c]++;
  }
  int n = 0;
  for (int c = 0; c < 256; c++)
    if (frec[c] > 0)
     n++;

  std::vector<unsigned char> bytes;
  if (n > 0) {
    if (!construir(frec))
     return false;
    if (!codificar(contenido, bytes))
     return false;
  }

  std::ofstream out(nombreArchivo.c_str(), std::ios::binary);
  if (!out)
   return false;
  escribirEntero(out, static_cast<uint64_t>(n), 2);
  for (int c = 0; c < 256; c++) {
    if (frec[c] > 0) {
      escribirEntero(out, static_cast<uint64_t>(c), 1);
      escribirEntero(out, frec[c], 8);
    }
  }
  escribirEntero(out, static_cast<uint64_t>(contenido.size()), 8);
  for (std::size_t i = 0; i < bytes.size(); i++)
    out.put(static_cast<char>(bytes[i]));
  out.close();
  return !out.fail();
}

bool ArbolHuffman::leer(const std::string& nombreArchivo, std::string& contenido) {
  contenido = "";
  std::ifstream in(nombreArchivo.c_str(), std::ios::binary);
  if (!in)
   return false;

  uint64_t n = 0, w = 0;
  if (!leerEntero(in, n, 2))
   return false;
  if (n > 256)
   return false;

  uint64_t frec[256];
  for (int c = 0; c < 256; c++)
   frec[c] = 0;

  uint64_t suma = 0;
  int anterior = -1;
  for (uint64_t i = 0; i < n; i++) {
    uint64_t c = 0, f = 0;
    if (!leerEntero(in, c, 1))
     return false;
    if (!leerEntero(in, f, 8))
     return false;
    if (f == 0 || static_cast<int>(c) <= anterior)
     return false;
    anterior = static_cast<int>(c);
    frec[c] = f;
    suma = suma + f;
  }
  if (!leerEntero(in, w, 8))
   return false;
  if (suma != w)
   return false;

  std::vector<unsigned char> bytes;
  int c = in.get();
  while (in) {
    bytes.push_back(static_cast<unsigned char>(c));
    c = in.get();
  }
  if (!construir(frec))
   return false;
  return decodificar(bytes, w, contenido);
}