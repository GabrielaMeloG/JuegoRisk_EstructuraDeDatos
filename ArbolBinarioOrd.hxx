#include "ArbolBinarioOrd.h"
#include <queue>

template< class T >
ArbolBinarioOrd<T>::ArbolBinarioOrd() {
    this->raiz = NULL;
}

template< class T >
ArbolBinarioOrd<T>::~ArbolBinarioOrd() {
    if (this->raiz != NULL) {
        delete this->raiz;
        this->raiz = NULL;
    }
}

template< class T >
bool ArbolBinarioOrd<T>::esVacio() {
    return this->raiz == NULL;
    
}

template< class T >
T ArbolBinarioOrd<T>::datoRaiz() {
    return (this->raiz)->obtenerDato();
    
}

//recurrente
template< class T >
int ArbolBinarioOrd<T>::altura() {
    //recurrente en el arbol
    if(this->esVacio())
        return -1;
    else
        return this->altura(this->raiz);
}

template< class T >
int ArbolBinarioOrd<T>::altura(NodoBinario<T>* nodo) {
    int valt;

    if(nodo->esHoja()){
        valt=0;
    }else {
        int valt_izq = -1;
        int valt_der = -1;
        if (nodo->obtenerHijoIzq() != NULL)
            valt_izq = this->altura(nodo->obtenerHijoIzq());
        if (nodo->obtenerHijoDer() != NULL)
            valt_der = this->altura(nodo->obtenerHijoDer());
        if (valt_izq > valt_der)
            valt = valt_izq + 1;
        else
            valt = valt_der + 1;
    }
    return valt; 
}

//recurrente
template< class T >
int ArbolBinarioOrd<T>::tamano() {
    //recurrente en el arbol
    if(this->esVacio())
        return 0;
    else
        return this->tamano(this->raiz);
}

template< class T >
int ArbolBinarioOrd<T>::tamano(NodoBinario<T>* nodo) {
    int tam = 1;

    if (nodo->obtenerHijoIzq() != NULL)
        tam = tam + this->tamano(nodo->obtenerHijoIzq());
    if (nodo->obtenerHijoDer() != NULL)
        tam = tam + this->tamano(nodo->obtenerHijoDer());

    return tam;
}
    

//iterativo
template< class T >
bool ArbolBinarioOrd<T>::insertar(T val) {
    NodoBinario<T>* nodo = this->raiz;
    NodoBinario<T>* padre = this->raiz;
    bool insertado = false;
    bool duplicado = false;

    while(nodo != NULL){
        padre = nodo;
        if (val < nodo->obtenerDato()) {
            nodo = nodo->obtenerHijoIzq();
        } else if (val > nodo->obtenerDato()) {
            nodo = nodo->obtenerHijoDer();
        } else {
            duplicado = true;
            break;
        }
    }

    if (!duplicado) {
        NodoBinario<T>* nuevo = new NodoBinario<T>(val);
        if (nuevo != NULL) {
            if (val < padre->obtenerDato())
                padre->fijarHijoIzq(nuevo);
            else
                padre->fijarHijoDer(nuevo);    
        }
        insertado = true;
    }
    return insertado;
}

//iterativo
template< class T >
bool ArbolBinarioOrd<T>::eliminar(T val) {
    NodoBinario<T>* nodo = this->raiz;
    NodoBinario<T>* padre = NULL;
    bool encontrado = false;

    // comparar con dato en nodo para bajar por izquierda o derecha
    // y para saber si val esta en el arbol
    while (nodo != NULL && !encontrado) {
        if (val < nodo->obtenerDato()) {
            padre = nodo;
            nodo = nodo->obtenerHijoIzq();
        } else if (val > nodo->obtenerDato()) {
            padre = nodo;
            nodo = nodo->obtenerHijoDer();
        } else {
            encontrado = true;
        }
    }

    // si val esta en el arbol
    if (encontrado) {

        // 1. nodo hoja, borrarlo
        if (nodo->esHoja()) {
            if (padre == NULL) {
                this->raiz = NULL;
            } else if (padre->obtenerHijoIzq() == nodo) {
                padre->fijarHijoIzq(NULL);
            } else {
                padre->fijarHijoDer(NULL);
            }
            delete nodo;

        // 2. nodo con un solo hijo, usar hijo para reemplazar nodo
        } else if (nodo->obtenerHijoIzq() == NULL || nodo->obtenerHijoDer() == NULL) {
            NodoBinario<T>* hijo;
            if (nodo->obtenerHijoIzq() != NULL)
                hijo = nodo->obtenerHijoIzq();
            else
                hijo = nodo->obtenerHijoDer();

            if (padre == NULL) {
                this->raiz = hijo;
            } else if (padre->obtenerHijoIzq() == nodo) {
                padre->fijarHijoIzq(hijo);
            } else {
                padre->fijarHijoDer(hijo);
            }
            delete nodo;

        // 3. nodo con dos hijos, usar maximo del subarbol izquierdo
        // para reemplazar nodo
        } else {
            NodoBinario<T>* padreMax = nodo;
            NodoBinario<T>* max = nodo->obtenerHijoIzq();

            while (max->obtenerHijoDer() != NULL) {
                padreMax = max;
                max = max->obtenerHijoDer();
            }

            nodo->fijarDato(max->obtenerDato());

            if (padreMax->obtenerHijoIzq() == max)
                padreMax->fijarHijoIzq(max->obtenerHijoIzq());
            else
                padreMax->fijarHijoDer(max->obtenerHijoIzq());

            delete max;
        }
    }

    return encontrado;
}

//iterativo
template< class T >
bool ArbolBinarioOrd<T>::buscar(T val) {
    NodoBinario<T>* nodo = this->raiz;
    bool encontrado = false;

    while(nodo != NULL && !encontrado){
        if (val < nodo->obtenerDato()) {
            nodo = nodo->obtenerHijoIzq();
        } else if (val > nodo->obtenerDato()) {
            nodo = nodo->obtenerHijoDer();
        } else {
            encontrado = true;
        }
    }

    return encontrado;
}

//recurrente
template< class T >
void ArbolBinarioOrd<T>::preOrden() {
    if (!this->esVacio())
        this->preOrden(this->raiz);
}

template< class T >
void ArbolBinarioOrd<T>::preOrden(NodoBinario<T>* nodo) {
    if (nodo != NULL) {
        std::cout << nodo->obtenerDato() << " ";
        this->preOrden(nodo->obtenerHijoIzq());
        this->preOrden(nodo->obtenerHijoDer());
    }
}

//recurrente
template< class T >
void ArbolBinarioOrd<T>::inOrden() {
    if (!this->esVacio())
        this->inOrden(this->raiz);

}

template< class T >
void ArbolBinarioOrd<T>::inOrden(NodoBinario<T>* nodo) {
    if (nodo != NULL){
    this->inOrden(nodo->obtenerHijoIzq());
    std::cout << nodo->obtenerDato() << " ";
    this->inOrden(nodo->obtenerHijoDer());
    }
}


//recurrente
template< class T >
void ArbolBinarioOrd<T>::posOrden() {
    if (!this->esVacio())
        this->posOrden(this->raiz);   
}

template< class T >
void ArbolBinarioOrd<T>::posOrden(NodoBinario<T>* nodo) {
    if (nodo != NULL) {
        this->posOrden(nodo->obtenerHijoIzq());
        this->posOrden(nodo->obtenerHijoDer());
        std::cout << nodo->obtenerDato() << " ";
    }
}

//iterativa, cola
template< class T >
void ArbolBinarioOrd<T>::nivelOrden() {
    if (!this->esVacio()) {
        std::queue< NodoBinario<T>*> cola;
        cola.push(this->raiz);
        NodoBinario<T>* nodo;
        while (!cola.empty()) {
            nodo = cola.front();
            cola.pop();
            std::cout << nodo->obtenerDato() << " ";
            if (nodo->obtenerHijoIzq() != NULL)
                cola.push(nodo->obtenerHijoIzq());
            if (nodo->obtenerHijoDer() != NULL)
                cola.push(nodo->obtenerHijoDer());
        }

    }
}
