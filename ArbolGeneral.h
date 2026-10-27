#ifndef _ARBOLGENERAL_H_ 
#define _ARBOLGENERAL_H_

#include "NodoGeneral.h"

template <class T>
class ArbolGeneral {
    protected:
        NodoGeneral<T>* raiz;
    public:
        ArbolGeneral();
        ArbolGeneral(T val);
        ~ArbolGeneral();
        bool esVacio();
        NodoGeneral<T>* obtenerRaiz();
        void fijarRaiz(NodoGeneral<T>* nraiz);
        bool insertarNodo(T padre, T n);
        bool insertarNodo(NodoGeneral<T>* actual, T padre, T n);
        bool eliminarNodo(T n);
        bool eliminarNodo(NodoGeneral<T>* actual, T n);
        bool buscar(T n);
        bool buscar(NodoGeneral<T>* nodo, T n);
        unsigned int altura();
        unsigned int altura(NodoGeneral<T>* nodo);
        unsigned int tamano();
        unsigned int tamano(NodoGeneral<T>* nodo);  
        void preOrden();
        void preOrden(NodoGeneral<T>* nodo);
        void posOrden();
        void posOrden(NodoGeneral<T>* nodo);
        void nivelOrden();

};
#endif