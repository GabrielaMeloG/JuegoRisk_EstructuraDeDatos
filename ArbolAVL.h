#ifndef _ARBOLAVL_H_
#define _ARBOLAVL_H_

#include "NodoBinario.h"

template< class T >
class ArbolAVL {
    protected:
        NodoBinario<T>* raiz;
    public:
        ArbolAVL();
        ~ArbolAVL();
        bool esVacio();
        T datoRaiz();
        int altura();
        int altura(NodoBinario<T>* nodo);  
        int tamano();
        int tamano(NodoBinario<T>* nodo);
        bool insertar(T val);
        bool eliminar(T val);
        NodoBinario<T>* insertar(NodoBinario<T>* nodo, T val, bool& insertado);
        NodoBinario<T>* eliminar(NodoBinario<T>* nodo, T val, bool& eliminado); 
        bool buscar(T val);
        void preOrden();
        void preOrden(NodoBinario<T>* nodo);
        void inOrden();
        void inOrden(NodoBinario<T>* nodo);
        void posOrden();
        void posOrden(NodoBinario<T>* nodo);
        void nivelOrden();
        NodoBinario<T>* balanceo(NodoBinario<T>* nodo);
        NodoBinario<T>* rotacionIzq(NodoBinario<T>* nodo);
        NodoBinario<T>* rotacionDer(NodoBinario<T>* nodo);
        NodoBinario<T>* rotacionIzqDer(NodoBinario<T>* nodo);
        NodoBinario<T>* rotacionDerIzq(NodoBinario<T>* nodo);
        bool verificar();
        bool verificar(NodoBinario<T>* nodo);
};
    
#include "ArbolAVL.hxx"

#endif