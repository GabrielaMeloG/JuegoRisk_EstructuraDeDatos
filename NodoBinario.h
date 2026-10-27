#ifndef _NODOBINAIRO_H_
#define _NODOBINAIRO_H_

template< class T >
class NodoBinario {
    protected:
        T dato;
        int altura;
        NodoBinario<T>* hijoIzq;
        NodoBinario<T>* hijoDer;
    public:
        NodoBinario();
        NodoBinario(T val);
        ~NodoBinario();
        T obtenerDato();
        void fijarDato(T val);
        NodoBinario<T>* obtenerHijoIzq();
        NodoBinario<T>* obtenerHijoDer();
        void fijarHijoIzq(NodoBinario<T>* izq);
        void fijarHijoDer(NodoBinario<T>* der);
        bool esHoja();
        int obtenerAltura();
        void fijarAltura();
        int diferenciaAltura();
};

#include "NodoBinario.hxx"
#endif