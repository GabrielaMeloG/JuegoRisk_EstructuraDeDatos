#include "ArbolAVL.h"
#include <queue>
#include <iostream>
template< class T >
ArbolAVL<T>::ArbolAVL() {
    this->raiz = NULL;
}

template< class T >
ArbolAVL<T>::~ArbolAVL() {
    if (this->raiz != NULL) {
        delete this->raiz;
        this->raiz = NULL;
    }
}

template< class T >
bool ArbolAVL<T>::esVacio() {
    return this->raiz == NULL;
    
}

template< class T >
T ArbolAVL<T>::datoRaiz() {
    return (this->raiz)->obtenerDato();
    
}

//recurrente
template< class T >
int ArbolAVL<T>::altura() {
    return this->altura(this->raiz);
}

template< class T >
int ArbolAVL<T>::altura(NodoBinario<T>* nodo) {
    return (nodo == NULL) ? -1 : nodo->obtenerAltura();
}

//recurrente
template< class T >
int ArbolAVL<T>::tamano() {
    //recurrente en el arbol
    if(this->esVacio())
        return 0;
    else
        return this->tamano(this->raiz);
}

template< class T >
int ArbolAVL<T>::tamano(NodoBinario<T>* nodo) {
    int tam = 1;

    if (nodo->obtenerHijoIzq() != NULL)
        tam = tam + this->tamano(nodo->obtenerHijoIzq());
    if (nodo->obtenerHijoDer() != NULL)
        tam = tam + this->tamano(nodo->obtenerHijoDer());

    return tam;
}
    
template< class T >
bool ArbolAVL<T>::insertar(T val) {
    bool insertado = false;
    this->raiz = this->insertar(this->raiz, val, insertado); 
    return insertado;
}

template< class T >
NodoBinario<T>* ArbolAVL<T>::insertar(NodoBinario<T>* nodo, T val, bool& insertado) {
    if (nodo == NULL) {                   
        insertado = true;
        return new NodoBinario<T>(val);
    }

    if (val < nodo->obtenerDato())
        nodo->fijarHijoIzq(this->insertar(nodo->obtenerHijoIzq(), val, insertado));
    else if (val > nodo->obtenerDato())
        nodo->fijarHijoDer(this->insertar(nodo->obtenerHijoDer(), val, insertado));
    else
        return nodo; 

    return this->balanceo(nodo);
}

/*iterativo
template< class T >
bool ArbolAVL<T>::insertar(T val) {
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
    balanceo(padre);
    return insertado;
}*/

template< class T >
bool ArbolAVL<T>::eliminar(T val) {
    bool eliminado = false;
    this->raiz = this->eliminar(this->raiz, val, eliminado);
    return eliminado;
}

template< class T >
NodoBinario<T>* ArbolAVL<T>::eliminar(NodoBinario<T>* nodo, T val, bool& eliminado) {
    if (nodo == NULL)
        return NULL;       

    if (val < nodo->obtenerDato()) {
        nodo->fijarHijoIzq(this->eliminar(nodo->obtenerHijoIzq(), val, eliminado));
    } else if (val > nodo->obtenerDato()) {
        nodo->fijarHijoDer(this->eliminar(nodo->obtenerHijoDer(), val, eliminado));
    } else {
        eliminado = true;
        NodoBinario<T>* izq = nodo->obtenerHijoIzq();
        NodoBinario<T>* der = nodo->obtenerHijoDer();

        if (izq == NULL || der == NULL) { 
            NodoBinario<T>* hijo = (izq != NULL) ? izq : der;
            nodo->fijarHijoIzq(NULL);       
            nodo->fijarHijoDer(NULL);
            delete nodo;
            return hijo;          
        }

  
        NodoBinario<T>* max = izq;
        while (max->obtenerHijoDer() != NULL)
            max = max->obtenerHijoDer();
        nodo->fijarDato(max->obtenerDato());

        bool aux;                          
        nodo->fijarHijoIzq(this->eliminar(izq, nodo->obtenerDato(), aux));
    }

    return this->balanceo(nodo);
}
/*iterativo
template< class T >
bool ArbolAVL<T>::eliminar(T val) {
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
    balanceo(padre);
    return encontrado;
}*/

//iterativo
template< class T >
bool ArbolAVL<T>::buscar(T val) {
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
void ArbolAVL<T>::preOrden() {
    if (!this->esVacio())
        this->preOrden(this->raiz);
}

template< class T >
void ArbolAVL<T>::preOrden(NodoBinario<T>* nodo) {
    if (nodo != NULL) {
        std::cout << nodo->obtenerDato() << " ";
        this->preOrden(nodo->obtenerHijoIzq());
        this->preOrden(nodo->obtenerHijoDer());
    }
}

//recurrente
template< class T >
void ArbolAVL<T>::inOrden() {
    if (!this->esVacio())
        this->inOrden(this->raiz);

}

template< class T >
void ArbolAVL<T>::inOrden(NodoBinario<T>* nodo) {
    if (nodo != NULL){
    this->inOrden(nodo->obtenerHijoIzq());
    std::cout << nodo->obtenerDato() << " ";
    this->inOrden(nodo->obtenerHijoDer());
    }
}


//recurrente
template< class T >
void ArbolAVL<T>::posOrden() {
    if (!this->esVacio())
        this->posOrden(this->raiz);   
}

template< class T >
void ArbolAVL<T>::posOrden(NodoBinario<T>* nodo) {
    if (nodo != NULL) {
        this->posOrden(nodo->obtenerHijoIzq());
        this->posOrden(nodo->obtenerHijoDer());
        std::cout << nodo->obtenerDato() << " ";
    }
}

//iterativa, cola
template< class T >
void ArbolAVL<T>::nivelOrden() {
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
template< class T >
NodoBinario<T>* ArbolAVL<T>::rotacionIzq(NodoBinario<T>* nodo) {
    NodoBinario<T>* padre = nodo->obtenerHijoDer();
    nodo->fijarHijoDer(padre->obtenerHijoIzq());
    padre->fijarHijoIzq(nodo);
    nodo->fijarAltura();
    padre->fijarAltura(); 
    return padre;
}
template< class T >
NodoBinario<T>* ArbolAVL<T>::rotacionDer(NodoBinario<T>* nodo) {
    NodoBinario<T>* padre = nodo->obtenerHijoIzq();
    nodo->fijarHijoIzq(padre->obtenerHijoDer());
    padre->fijarHijoDer(nodo);
    nodo->fijarAltura();
    padre->fijarAltura(); 
    return padre;
}
template< class T >
NodoBinario<T>* ArbolAVL<T>::rotacionIzqDer(NodoBinario<T>* nodo) {
    NodoBinario<T>* aux = rotacionIzq(nodo->obtenerHijoIzq());
    nodo->fijarHijoIzq(aux);
    NodoBinario<T>* padre = rotacionDer(nodo);
    return padre;
}
template< class T >
NodoBinario<T>* ArbolAVL<T>::rotacionDerIzq(NodoBinario<T>* nodo) {
    NodoBinario<T>* aux = rotacionDer(nodo->obtenerHijoDer());
    nodo->fijarHijoDer(aux);
    NodoBinario<T>* padre = rotacionIzq(nodo);
    return padre;
}
template< class T >
NodoBinario<T>* ArbolAVL<T>::balanceo(NodoBinario<T>* nodo) {
    nodo->fijarAltura();
    if(nodo->diferenciaAltura() == 2){
        if(nodo->obtenerHijoIzq()->diferenciaAltura() >= 0){
            return this->rotacionDer(nodo);
        }else{
            return this->rotacionIzqDer(nodo);
        }
    }else if(nodo->diferenciaAltura() == -2){
        if(nodo->obtenerHijoDer()->diferenciaAltura() <= 0){
            return this->rotacionIzq(nodo);
        }else{
            return this->rotacionDerIzq(nodo);
        }
    }
    return nodo;
}
template< class T >
bool ArbolAVL<T>::verificar() { return this->verificar(this->raiz); }

template< class T >
bool ArbolAVL<T>::verificar(NodoBinario<T>* n) {
    if (n == NULL) return true;
    int hi = this->altura(n->obtenerHijoIzq());
    int hd = this->altura(n->obtenerHijoDer());
    int esperada = 1 + (hi > hd ? hi : hd);
    int fb = hi - hd;
    return n->obtenerAltura() == esperada      // la altura guardada es correcta
        && fb >= -1 && fb <= 1                 // cumple la propiedad AVL
        && this->verificar(n->obtenerHijoIzq())
        && this->verificar(n->obtenerHijoDer());
}
