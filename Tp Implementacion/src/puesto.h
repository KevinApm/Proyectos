#include "list"
#include "map"
#include "set"
#include "vector"
#include "tipos.h"

using namespace std;

struct compra{
    compra(Producto i, Nat c):item(i), cant(c){};
    Producto item;
    Nat cant;
};

class puesto{

    public:

    puesto(const Menu& p,const Stock& s,const Promociones& d);

    // Complejidad: O(I), I = Cant productos.
    const set<Producto>& menu();

    //Complejidad: O(log(I)).
    Nat precio(Producto i) const;

    //Complejidad: O(log(I)).
    Nat stockItem(Producto i) const;

    //Complejidad: O(log(I)).
    Nat descuentoItem(Producto i, Nat k) const;

    //Complejidad: O(log(A)), A = Cant Personas.
    Nat gastoPersona(Persona per) const;

    //Complejidad: O(log(A)).
    const list<compra>& ventas(Persona per) const;

    //Complejidad: O(log(A) + log(I)).
    void vender(Persona per, Producto i, Nat k);

    //Complejidad: O(log(A) + log(I)).
    void olvidarItem(Persona per, Producto i);

    //Complejidad: O(log(A) + log(I)).
    bool consumioSinPromo(Persona per, Producto i);

    puesto& operator=(const puesto& Acopiar);

    puesto(const puesto& Acopiar);


    private:
    struct stockPrecio{
        stockPrecio(Nat p, Nat s):precio(p), stock(s){};
        Nat precio;
        Nat stock;
    };

    struct infoPersona{
        infoPersona(Nat g, list<compra> c):gasto(g), consumos(c){};
        Nat gasto;
        list<compra> consumos;
    };

    map<Producto, stockPrecio> stockYPrecio;
    //aclarar por que aparece un vector
    map<Producto, vector<Nat>> descuento;
    map<Persona, infoPersona> venta;
    map<Persona, map<Producto, list<list<compra>::iterator>>> itCompraSinDescuento;

};