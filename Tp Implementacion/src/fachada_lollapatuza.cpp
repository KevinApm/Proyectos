#include "fachada_lollapatuza.h"


FachadaLollapatuza::FachadaLollapatuza(const set<Persona> &personas, const map<IdPuesto, aed2_Puesto> &infoPuestos):
        _lolla(infoPuestos, personas){
}

void FachadaLollapatuza::registrarCompra(Persona persona, Producto producto, Nat cant, IdPuesto idPuesto) {
    _lolla.RegistrarCompra(persona, idPuesto, producto, cant);
}

void FachadaLollapatuza::hackear(Persona persona, Producto producto) {
    _lolla.Hackear(persona, producto);
}

Nat FachadaLollapatuza::gastoTotal(Persona persona) const {
    return _lolla.GastoTotal(persona);
}

Persona FachadaLollapatuza::mayorGastador() const {
    return _lolla.PersonaQueMasGasto();
}

IdPuesto FachadaLollapatuza::menorStock(Producto producto) const {
   return _lolla.MenorStockItem(producto);
}

const set<Persona> &FachadaLollapatuza::personas() const {
    return _lolla.Personas();
}

Nat FachadaLollapatuza::stockEnPuesto(IdPuesto idPuesto, const Producto &producto) const {
    return _lolla.Puestos().at(idPuesto).stockItem(producto);
}

Nat FachadaLollapatuza::descuentoEnPuesto(IdPuesto idPuesto, const Producto &producto, Nat cantidad) const {
    return _lolla.Puestos().at(idPuesto).descuentoItem(producto, cantidad);
}

Nat FachadaLollapatuza::gastoEnPuesto(IdPuesto idPuesto, Persona persona) const {
    return _lolla.Puestos().at(idPuesto).gastoPersona(persona);
}

set<IdPuesto> FachadaLollapatuza::idsDePuestos() const {
    map<IdPuesto, puesto> puestos = _lolla.Puestos();
    set<IdPuesto> res;
    for (pair<IdPuesto, puesto> elem : puestos) {
        res.insert(elem.first);
    }
    return res;
}