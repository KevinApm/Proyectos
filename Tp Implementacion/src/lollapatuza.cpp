#include "lollapatuza.h"

Lollapatuza::Lollapatuza(const map<IdPuesto, aed2_Puesto>& dicPuestos, const set<Persona>& per): _puestos(), _personas(), _itPuestos(),
_gastoPersonas(), _personaQueMasGasto(per.size()), _puestoMenorId() {
    //Haciendo el for de iteradores, insertamos todos los elementos de per en personas
    for (Persona p: per) {
        _personas.insert(p);
    }
    for (auto elem: dicPuestos) {
        //Como el insert del map devuelve una tupla <iterador, bool>, pongo .first para obtener el iterador
        puesto puestito(elem.second.menu, elem.second.stock,elem.second.promociones);
        IdPuesto id = elem.first;
        _puestos.insert(make_pair(id, puestito));
        auto it = _puestos.find(id);
        _itPuestos.insert(make_pair(id, it));
    }
}

map<IdPuesto, puesto> Lollapatuza::Puestos() const{
    return _puestos;
}

const set<Persona>& Lollapatuza::Personas() const{
    return _personas;
}
#ifndef APLICARDESCUENTO
#define APLICARDESCUENTO
Nat Division(Nat n, Nat k){
    Nat N = n;
    Nat K = k;
    Nat Div = 0;
    while (N >= K){
        Div = 1 + Div;
        N = N - K;
    }
    return Div;
}

Nat APLICARDescuento(Nat p, Nat d){
    return Division(p * (100 - d), 100);
}
#endif
void Lollapatuza::RegistrarCompra(Persona p, IdPuesto pi, Producto i, Nat k) {
    auto it = _itPuestos.at(pi);
    puesto& puestito = it->second;
    // nos falta el puesto .c
    puestito.vender(p, i, k);
    Nat descuento = puestito.descuentoItem(i,k);
    Nat precio = puestito.precio(i)*k;
    //Veo si la compra tiene descuento o no
    if (descuento != 0){
        // calculo el gasto de la compra
        Nat gasto = APLICARDescuento(precio, descuento);
        if (_gastoPersonas.count(p) == 1){
            //Actualizo el gasto de la persona usando el it a la cola
            auto itGasto = _gastoPersonas.at(p);
            auto gastoviejo = *itGasto;
            //Actualizo el gasto
            Nat nuevoGasto = gasto + gastoviejo.gasto;
            //Elimino del heap a la persona con el gasto viejo usando el iterador
            itGasto.eliminarSiguiente();
            //Vuelvo a encolar a la persona con el gasto actualizado
            auto nuevoIt = _personaQueMasGasto.encolar(gastoYPersona(nuevoGasto, p));
            //Redefino con el nuevo iterador
            //El map de c++ no permite redefinir por lo tanto tenemos qu eliminarlo
            //Para volver a definirlo
            _gastoPersonas.erase(p);
            _gastoPersonas.insert(make_pair(p, nuevoIt));
        } else{
            auto nuevoIt = _personaQueMasGasto.encolar(gastoYPersona(gasto, p));
            _gastoPersonas.insert(make_pair(p, nuevoIt));
        }
    } else {
            if (_gastoPersonas.count(p) == 1){
            auto itGasto = _gastoPersonas.at(p);
            auto gastoViejo = *itGasto;
            Nat nuevoGasto = precio + gastoViejo.gasto;
            itGasto.eliminarSiguiente();
            auto nuevoIt = _personaQueMasGasto.encolar(gastoYPersona(nuevoGasto, p));
            // Redefino p en e.gastoPersonas.
            _gastoPersonas.erase(p);
            _gastoPersonas.insert(make_pair(p, nuevoIt));
            }else{
                auto nuevoIt = _personaQueMasGasto.encolar(gastoYPersona(precio, p));
                _gastoPersonas.insert(make_pair(p, nuevoIt));
            }
            // Ahora hay que actualizar e.puestoMenorId.
            // Veo si la persona ya compro si descuento
            if (_puestoMenorId.count(p) == 1){
                //Veo si la persona ya compro ese item sin descuento
                if (_puestoMenorId.at(p).count(i) == 1){
                    //Veo si la persona ya compro en ese puesto sin descuento
                    if (_puestoMenorId.at(p).at(i).count(pi) == 0){
                        _puestoMenorId.at(p).at(i).insert(make_pair(pi,it));
                    }
                }else{
                    //creo un diccionario vacio para insertar un puesto id y un iterador, para insertar en (_puestoMenorID, p)
                    map<IdPuesto, map<IdPuesto, puesto>::iterator> vacio;
                    vacio.insert(make_pair(pi, it));
                    _puestoMenorId.at(p).insert(make_pair(i,vacio));
                }
            }else{
                map<IdPuesto, map<IdPuesto, puesto>::iterator> vacio;
                vacio.insert(make_pair(pi, it));
                map<Producto, map<IdPuesto,map<IdPuesto, puesto>::iterator>> blanco;
                blanco.insert(make_pair(i, vacio));
                _puestoMenorId.insert(make_pair(p, blanco));
            }
    }
}

void Lollapatuza::Hackear(Persona per, Producto i) {
    // Busco el puesto a hackear creando un iterador a la primer clave de
    // _puestoMenorId.at(per).at(i) dado que esta ordenador de menor a mayor
    auto it = _puestoMenorId.at(per).at(i).begin();
    puesto& puestito = it->second->second;
    puestito.olvidarItem(per,i);
    //Ajusto el gasto de la persona como ya sabemos (eliminando y encolando).
    auto itGasto = _gastoPersonas.at(per);
    auto viejoGasto = *itGasto;
    Nat nuevoGasto = viejoGasto.gasto - puestito.precio(i);
    itGasto.eliminarSiguiente();
    auto nuevoIt = _personaQueMasGasto.encolar(gastoYPersona(nuevoGasto, per));
    _gastoPersonas.erase(per);
    _gastoPersonas.insert(make_pair(per, nuevoIt));
    //Chequeo si era la ultima compra de la persona en ese puesto
    if (not puestito.consumioSinPromo(per, i)){
        _puestoMenorId.at(per).at(i).erase(it->first);
    }
}

Nat Lollapatuza::GastoTotal(Persona p) const{
    if (_gastoPersonas.count(p) == 1){
        auto info = *_gastoPersonas.at(p);
        return info.gasto;
    } else{
        return 0;
    }
}

Persona Lollapatuza::PersonaQueMasGasto() const {
    if (_personaQueMasGasto.vacia()){
        // En caso de que nadie haya realizado una compra
        // todas las personas estan empatadas -> devuelvo la de menor DNI
        auto it = _personas.begin();
        return *it;
    }else{
        return _personaQueMasGasto.proximo().persona;
    }

}

IdPuesto Lollapatuza::MenorStockItem(Producto i) const{
    // Iteramos todos los puestos para encontrar el que posea el menor stock.
    // Para el item i.
    //Asumimos que el primer puesto tiene el menor stock del item
    auto it = _puestos.cbegin();
    Nat stockMin = it->second.stockItem(i);
    IdPuesto idMin = it->first;
    while (it != _puestos.end()){
       Nat stock = it->second.stockItem(i);
       IdPuesto idActual = it->first;
       if (stockMin > stock){
           stockMin = stock;
           idMin = idActual;
       } else {
           if (stockMin == stock){
               if (idMin > idActual){
                   idMin = idActual;
               }
           }
       }
       it++;
    }
    return idMin;
}


