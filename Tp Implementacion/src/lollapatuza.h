#ifndef TP_LOLLA_LOLLAPATUZA_H
#define TP_LOLLA_LOLLAPATUZA_H
#include "puesto.h"
#include "Cola_PriorMAX.hpp"

class Lollapatuza {
    public:

        Lollapatuza(const map<IdPuesto, aed2_Puesto>& puestos, const set<Persona>& per);

        map<IdPuesto, puesto> Puestos() const;

        const set<Persona>& Personas() const;

        void RegistrarCompra(Persona p, IdPuesto pi, Producto i, Nat k);

        void Hackear(Persona per, Producto i);

        Nat GastoTotal(Persona p) const;

        Persona PersonaQueMasGasto() const;

        IdPuesto MenorStockItem(Producto i) const;


    private:
        // item es Producto
        struct gastoYPersona{
            gastoYPersona(Nat g, Persona p):gasto(g), persona(p){};
            bool operator<(gastoYPersona aComparar){
                bool res;
                if (gasto < aComparar.gasto){
                    res = true;
                }else if(gasto == aComparar.gasto) {
                    res = (persona > aComparar.persona);
                }else{
                    res = false;
                }
                return res;
            }
            bool operator>(gastoYPersona aComparar){
                bool res;
                if (gasto > aComparar.gasto){
                    res = true;
                }else if(gasto == aComparar.gasto) {
                    res = (persona < aComparar.persona);
                }else{
                    res = false;
                }
                return res;
            }
            Nat gasto;
            Persona persona;
        };
        map<IdPuesto, puesto> _puestos;
        set<Persona> _personas;
        map<IdPuesto, map<IdPuesto, puesto>::iterator> _itPuestos;
        map<Persona, cola_PriorMAX<gastoYPersona>::iterador> _gastoPersonas;
        cola_PriorMAX<gastoYPersona> _personaQueMasGasto;
        map<Persona, map<Producto, map<IdPuesto, map<IdPuesto, puesto>::iterator>>> _puestoMenorId;
};


#endif //TP_LOLLA_LOLLAPATUZA_H
