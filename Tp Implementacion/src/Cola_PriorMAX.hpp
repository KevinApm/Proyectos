//
// Created by clinux01 on 16/06/23.
//
#include "Cola_PriorMAX.h"

using namespace std;
#ifndef COLA_PRIORIDADTP3_COLA_PRIORMAX_HPP
#define COLA_PRIORIDADTP3_COLA_PRIORMAX_HPP

/** AUXILIARES*/
template<typename T>
int cola_PriorMAX<T>::padre(int i) {
    int res = (i/2);
    if (i % 2 == 0){
        res--;
    }
    return res;
}

template<typename T>
void cola_PriorMAX<T>::mostrar(ostream &o) {
    o << "[";
    for (int i = 0; i < _tamanioCola; ++i) {
        o << _colaPrior[i]->objeto << "," ;
    }
    o<< "]";
};

/**ACABA ACA AUZILIARES*/

template<typename T>
cola_PriorMAX<T>::cola_PriorMAX(int limite) {
    _listaAlmacen = list<tuplaAlmacen>();
    vector<typename list<tuplaAlmacen>::iterator> colaP (limite);
    _colaPrior = colaP;
    _limite = limite;
    _tamanioCola = 0;
}

template<typename T>
int cola_PriorMAX<T>::tamanio() const {
    return _tamanioCola;
}

template<typename T>
int cola_PriorMAX<T>::limite() const {
    return _limite;
}

template<typename T>
bool cola_PriorMAX<T>::vacia() const {
    return _tamanioCola == 0;
}

template<typename T>
T cola_PriorMAX<T>::proximo() const {
    return _colaPrior[0]->objeto;
}

template<typename T>
void cola_PriorMAX<T>::swapHeap(int i, int j) {
    auto itGuardarI = _colaPrior[i];
    auto itGuardarJ = _colaPrior[j];
    _colaPrior[i] = itGuardarJ;
    itGuardarJ->itPosicion = i;
    _colaPrior[j] = itGuardarI;
    itGuardarI->itPosicion = j;
};

template<typename T>
typename cola_PriorMAX<T>::iterador cola_PriorMAX<T>::encolar(T objeto) {
    tuplaAlmacen nuevoNodo = tuplaAlmacen(objeto, _tamanioCola);
    _listaAlmacen.push_back(nuevoNodo);
    auto itNuevito = prev(_listaAlmacen.end());
    //Guardo el iterador de haber agregado a la lista mi nuevo nodo

    _colaPrior[_tamanioCola]= itNuevito;
    _tamanioCola++;
    int i = _tamanioCola -1;
    while (i > 0 and _colaPrior[padre(i)]->objeto < _colaPrior[i]->objeto){
        swapHeap(padre(i), i);
        i = padre(i);
    }
    cola_PriorMAX<T>* dirCola = this;
    auto itRes = iterador(itNuevito, dirCola);
    return itRes;
}

template<typename T>
void cola_PriorMAX<T>::Heapify(int i) {
    int izq= 2*i +1;
    int der= 2*i +2;
    int max = i;
    if (izq < _tamanioCola and _colaPrior[izq]->objeto > _colaPrior[i]->objeto){
        max = izq;
    }
    if (der < _tamanioCola and _colaPrior[der]->objeto > _colaPrior[max]->objeto){
        max = izq;
    }
    if (max != i ){
        swapHeap(i, max);
        Heapify(max);
    }
};

template<typename T>
void cola_PriorMAX<T>::eliminar(int i) {
    if(i== 0){
        if (_tamanioCola==1){
            _listaAlmacen.erase(_colaPrior[0]);
            _tamanioCola--;
        }
        else{
            swapHeap(i, _tamanioCola - 1);
            _listaAlmacen.erase(_colaPrior[_tamanioCola - 1]);
            _tamanioCola--;
            Heapify(0);
        }
    }
    else{
        swapHeap(i, _tamanioCola - 1);
        _tamanioCola--;
        //Aca tengo que tocar cosas
        if ( _colaPrior[padre(i)]->objeto > _colaPrior[i]->objeto  ){
            Heapify(i);
        }
        else{
            int aSubir = i;
            while (aSubir > 0 and _colaPrior[padre(aSubir)]->objeto < _colaPrior[aSubir]->objeto ){
                swapHeap(aSubir, padre(aSubir));
                aSubir = padre(aSubir);
            }
        }
        _listaAlmacen.erase(_colaPrior[_tamanioCola]);
    }

}

template<typename T>
void cola_PriorMAX<T>::desencolar() {
    eliminar(0);
}


/** Funciones de ITERADOR */

template<typename T>
cola_PriorMAX<T>::iterador::iterador(): _dirCola(nullptr), _itDeAlmacen() {
    //_dirCola= nullptr;
}

template<typename T>
cola_PriorMAX<T>::iterador::iterador(typename list<tuplaAlmacen>::iterator itDeAlmacen, cola_PriorMAX<T>* dirCola) {
    _dirCola = dirCola;
    _itDeAlmacen = itDeAlmacen;
}

template<typename T>
typename cola_PriorMAX<T>::iterador& cola_PriorMAX<T>::iterador::operator=(typename cola_PriorMAX<T>::iterador aCopiar) {
    _itDeAlmacen = aCopiar._itDeAlmacen;
    _dirCola = aCopiar._dirCola;
    return *this;

    /*T objAcopiar = aCopiar._itDeAlmacen->objeto;
    int indiceItAcopiar = aCopiar->itPosicion;
    _itDeAlmacen = tuplaAlmacen(objAcopiar, indiceItAcopiar);*/
}

template<typename T>
void cola_PriorMAX<T>::iterador::eliminarSiguiente() {
    int indiceAEliminar = _itDeAlmacen->itPosicion ;
    (*_dirCola).eliminar(indiceAEliminar);
};

template<typename T>
typename cola_PriorMAX<T>::iterador::value_type cola_PriorMAX<T>::iterador::operator*() const {
    //aca tengo que devolver algo del mismo tipo que los nodos que alberga la cola
    return _itDeAlmacen->objeto ;
}

#endif //COLA_PRIORIDADTP3_COLA_PRIORMAX_HPP
