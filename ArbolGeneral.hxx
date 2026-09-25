#include "ArbolGeneral.h"

template <class T>
ArbolGeneral<T>::ArbolGeneral(){
    this->raiz=NULL;

}

template <class T>
ArbolGeneral<T>::ArbolGeneral(T val){
    NodoGeneral<T>* nodo = new NodoGeneral<T>;
    nodo->fijarDato(val);
    this->raiz =nodo;

}

template <class T>
ArbolGeneral<T>::~ArbolGeneral(){ 
    delete this->raiz;
    this->raiz = NULL;

}

template <class T>
bool ArbolGeneral<T>::esVacio(){
    return this->raiz ==NULL;

}

template <class T>
NodoGeneral<T>* ArbolGeneral<T>::obtenerRaiz(){
    return this->raiz;

}

template <class T>
void ArbolGeneral<T>::fijarRaiz(NodoGeneral<T>* nraiz){
    this->raiz = nraiz;

} 

template <class T>
bool ArbolGeneral<T>::insertarNodo(T padre, T n){
    //revisar si el arbol es vacio:
    //crear nuevo nodo, asignar dato, poner ese nodo como raiz
    if(this->esVacio()){
        NodoGeneral<T>* nuevo = new NodoGeneral<T>;
        nuevo->fijarDato(n);
        this->raiz = nuevo;
        return true;


    //si hay al menos un nodo el arbol 
    // revisar nodo donde estoy para ver se coincide con padre
    // si es padre, insertar ahi el nuevo hijo
    //si no es el padre, revisar cada nodo hijo y llamar a insertar alli
    return this->insertarNodo(this->raiz, padre, n);

}

template <class T>
bool ArbolGeneral<T>::insertarNodo(NodoGeneral<T>* actual, T padre, T n){
    bool insertado = false;

    if(actual->obtenerDato() == padre){
        NodoGeneral<T>* nuevo = new NodoGeneral<T>;
        nuevo->fijarDato(n);
        actual->desc.push_back(nuevo);
        insertado = true;
    } else {
        std::list< NodoGeneral<T>* >::iterator it;
        for(it = actual->desc.begin(); it != actual->desc.end() && !insertado; it++){
            insertado = this->insertarNodo(*it, padre, n);
        }
    }

    return insertado;
}


template <class T>
bool ArbolGeneral<T>::eliminarNodo(T n){
    //si es arbol vacio;
    //retornar
    if(this->esVacio()){
        return false;
    }

    //si es la raiz la que quiero eliminar;
    //hacer delete a raiz
    //poner raiz en nulo
     if(this->raiz->obtenerDato() == n){
        delete this->raiz;
        this->raiz = NULL;
        return true;
     }   

    //si hay al menos un nodo en el arbol; 
    //revisasr el nodo donde estoy para ver si es el que quiero eliminar
    // si ninguno de los hijos es el que quiero eliminar;
    //revisar cada nodo hijo y llamar a eliminar alli 
     return this->eliminarNodo(this->raiz, n);
}


template <class T>
bool ArbolGeneral<T>::eliminarNodo(NodoGeneral<T>* actual, T n){
    bool eliminado = false;
    std::list< NodoGeneral<T>* >::iterator it;

    for(it = actual->desc.begin(); it != actual->desc.end() && !eliminado; it++){
        if((*it)->obtenerDato() == n){
            delete *it;
            actual->desc.erase(it);
            eliminado = true;
        } else {
            eliminado = this->eliminarNodo(*it, n);
        }
    }

    return eliminado;
}


template <class T>
bool ArbolGeneral<T>::buscar(T n){
    // si el arbol no esta vacio:
    if(this->esVacio()){
        return false;
    }

     return this->buscar(this->raiz, n);
}


template <class T>
bool ArbolGeneral<T>::buscar(NodoGeneral<T>* nodo, T n){
    //comparo dato en el nodo actual con dato parametro
    //si es este, retorno que lo encontre
    if(nodo->obtenerDato() == n){
    return true;
    }

    //si no, para cada nodo hijo hacer el llamado a buscar
    bool encontrado = false;
    std::list< NodoGeneral<T>* >::iterator it;
    for(it = nodo->desc.begin(); it != nodo->desc.end() && !encontrado; it++){
        encontrado = this->buscar(*it, n);
    }

    return encontrado;

}

template <class T>
int ArbolGeneral<T>::altura(){
    if (this->esVacio()){
        return -1;
    } else {
        return this->altura(this->raiz);
    }

}

template <class T>
int ArbolGeneral<T>::altura(NodoGeneral<T>* nodo){
    int alt = -1;

    if (nodo->EsHoja()){
        alt = 0;
    }else {
        int alth;
        std::list< NodoGeneral<T>* >::iterador it;
        for (it =nodo->desc.begin(); it != nodo->desc.end(); it++){
            alth = this->altura(*it);
            if(alt < alth+1)
                alt = alth+1;
        }
    }

    return alt;
}


template <class T>
unsigned int ArbolGeneral<T>::tamano(){
    //si es vacio, retorno 0
    if(this->esVacio()){
        return 0;
    }

    return this->tamano(this->raiz);
}

template <class T>
unsigned int ArbolGeneral<T>::tamano(NodoGeneral<T>* nodo){
    //para cada uno de los hijos, llamo a tamano
    //acumulo esos tamanos en una variable
    unsigned int tam = 0;
    std::list< NodoGeneral<T>* >::iterator it;
    for(it = nodo->desc.begin(); it != nodo->desc.end(); it++){
        tam = tam + this->tamano(*it);
    }

    //retorno ese valor acumulado mas 1
    return tam + 1;

}

template <class T>
void ArbolGeneral<T>::preOrden(){
    if (!this->esVacio())
        this->preOrden(this->raiz);

}

template <class T>
void ArbolGeneral<T>::preOrden(NodoGeneral<T>* nodo){
    std::cout << nodo->obtenerDato() << "";

    std::list< NodoGeneral<T>* >::iterator it;
    for(it = nodo->Desc.begin(); it != nodo->desc.end(); it++){
        this->preOrden(*it);
    }
}    



template <class T>
void ArbolGeneral<T>::posOrden(){
    if(!this->esVacio())
        this->posOrden(this->raiz);
}

template <class T>
void ArbolGeneral<T>::posOrden(NodoGeneral<T>* nodo){
    //llamar a posOrden sobre cada hijo
    std::list< NodoGeneral<T>* >::iterator it;
    for(it = nodo->desc.begin(); it != nodo->desc.end(); it++){
        this->posOrden(*it);
    }

    //imprimo en pantalla el dato del nodo actual
     std::cout << nodo->obtenerDato() << " ";

}

template <class T>
void ArbolGeneral<T>::nivelOrden(){
    //NO ES RECURRENTE (0 RECURSIVO)
    if(this->esVacio())
        return;

    //ubicarme en la raiz
    //poner la raiz en una cola 
    std::queue< NodoGeneral<T>* > cola;
    cola.push(this->raiz);

    //hacer un ciclo mientras haya algo en la cola
    while(!cola.empty()){
        //saco el primer disponible en la cola
        NodoGeneral<T>* actual = cola.front();
        cola.pop();

        //imprimo su dato 
        std::cout << actual->obtenerDato() << " ";

        //inserto en la cola todos sus hijos
         std::list< NodoGeneral<T>* >::iterator it;
        for(it = actual->desc.begin(); it != actual->desc.end(); it++){
            cola.push(*it);
        }
    }

}

