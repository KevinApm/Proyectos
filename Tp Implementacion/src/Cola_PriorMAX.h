//
// Created by clinux01 on 16/06/23.
//

#ifndef COLA_PRIORIDADTP3_COLA_PRIORMAX_H
#define COLA_PRIORIDADTP3_COLA_PRIORMAX_H
#include "list"
#include "vector"
#include "ostream"
using namespace std;

//Antes de usar la coal de prioridad , tenemos que reemplazar T por un struct , que tenga lo que queremos que tenga
// y dentro de ese struict dfefinir la opertacion de orden .


template<typename T>
class cola_PriorMAX{
public:
    typedef T value_type;
    class iterador;
    cola_PriorMAX(int limite);
    void desencolar();
    iterador encolar(T objeto);
    int tamanio() const ;
    int limite() const;
    bool vacia()const;
    T proximo() const;
    void mostrar(ostream& o);
    friend ostream& operator<<(ostream& os, cola_PriorMAX<T>& cola) {
        cola.mostrar(os);
        return os;
    }
/** Parte privada de la cola de prioridad */
private:
    struct tuplaAlmacen{
        tuplaAlmacen(T cosa, int pos): objeto(cosa), itPosicion(pos){}
        T objeto;
        int itPosicion;
    };
    void swapHeap(int i, int j);
    int padre(int i);
    void eliminar(int i);
    void Heapify( int i);



    list<tuplaAlmacen> _listaAlmacen;
    vector<typename list<tuplaAlmacen>::iterator> _colaPrior;
    int _tamanioCola ;
    int _limite;

/** Declaracion fortmal de la clase de iterador*/
public:
    class iterador{
    public:
        typedef T value_type;
        iterador();
        iterador(typename list<tuplaAlmacen>::iterator itDeAlmacen, cola_PriorMAX* dirCola);
        value_type operator*() const;
        void eliminarSiguiente();
        friend class cola_PriorMAX;
        iterador& operator=(typename cola_PriorMAX<T>::iterador aCopiar);
    private:

        typename list<tuplaAlmacen>::iterator _itDeAlmacen;
        cola_PriorMAX* _dirCola;
    };
};




#endif //COLA_PRIORIDADTP3_COLA_PRIORMAX_H
