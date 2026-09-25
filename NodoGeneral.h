#ifndef _NODOGENERAL_H_
#define _NODOGENERAL_H_

#include <list> 

template <class T>
class NodoGeneral {
    protected:
        T dato;
        std::list<NodoGeneral<T>* > desc;
    public:
        NodoGeneral();
        ~NodoGeneral();
        T& obtenerDato();
        void fijarDato(T& val);
        void limpiarLista();
        void adicionarDesc(T& nval);
        bool eliminarDesc(T& val);
        bool esHoja();
};

#include "NodoGeneral.hxx"

#endif