#include "NodoGeneral"

template< class T >
NodoGeneral<T>::NodoGeneral(){
    this ->desc.clear()

}

template< class T >
NodoGeneral<T>::NodoGeneral(){
    std::list<NodoGeneral<T> >::iterador it;
        for (it= this->desc.begin(); it != this->desc.end(); it;)
            delete *it;
        this->desc.clear()

}

template< class T >
T& NodoGeneral<T>::obtenerDato(){
    return this->dato;

}

template< class T >
void NodoGeneral<T>::fijarDato(){
    this->dato = val;

}

template< class T >
void NodoGeneral<T>::limpiarLista(){
    this->desc.clear();

}

template< class T >
void NodoGeneral<T>::adicionarDesc(T& nval){
    NodoGeneral<T> *nodo = new NodoGeneral <T>;
    nodo->fijarDato(nval);
    this->desc.push_back(nodo);

}

template < class T >
bool NodoGeneral<T>::eliminarDesc(T& val){
    //buscar el nodo con el valor dado, saber si el valor que nos estan dando si es uno de los hijos que tenemos dentro de este nodo
    std::list<NodoGeneral<T>* >::iterator it;
    NodoGeneral<T> *aux;
    bool eliminado = false;

    for (it = this->desc.begin(); it != this->desc.end(); it++) {
        aux= *it;
        if (aux->obtenerDato() == val)
            break;
    }

    // si lo encontramos, lo eliminamos
    if (it != this->desc.end()) {
        delete *it;
        this->desc.erase(it);
        eliminado = true;
    }

    return eliminado;

}

template < class T >
bool NodoGeneral<T>::esHoja(){
     return this->desc.size() = 0;
}




