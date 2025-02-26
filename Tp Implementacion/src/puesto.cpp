#include "puesto.h"

puesto::puesto(const Menu& p, const Stock& s, const Promociones& d):stockYPrecio(), descuento(),
venta(),itCompraSinDescuento(){
    // recorremos todos los productos para definir el stock y precio en un unico diccionario
    for (const pair<Producto, Nat> elem : p) {
        Producto prod = elem.first;
        Nat precio = elem.second;
        stockYPrecio.insert(make_pair(prod, stockPrecio(precio, s.at(prod))));
    }
    // Creamos un vector de tamaño cant + 1 (para poder indexar con la cantidad) donde cant es la mayor cantidad
    // para la cual ese producto tiene descuento.
    // luego llenamos el vector con los descuentos que corresponde.
    for (const pair<Producto, map<Nat, Nat>> elem : d){
        Nat cant = prev(elem.second.end())->first;
        vector<Nat> descuentos(cant + 1, 0);

        for (pair<Nat, Nat> e : elem.second) {
            descuentos[e.first] = e.second;
        }
        // recorremos el vector y vamos cambiando los descuentos segun las cantidades
        Nat dAplicable = 0;
        for (int i = 0; i < descuentos.size(); ++i) {
            if (descuentos[i] != 0){
                dAplicable = descuentos[i];
            }else{
                descuentos[i] = dAplicable;
            }
        }
        descuento.insert(make_pair(elem.first, descuentos));
    }

}

const set<Producto>& puesto::menu(){
    set<Producto> claves;
    for (pair<Producto, stockPrecio> elem : stockYPrecio){
        claves.insert(elem.first);
    }
    return claves;
}

Nat puesto::stockItem(Producto i) const {
    return stockYPrecio.at(i).stock;
}

Nat puesto::descuentoItem(Producto i, Nat k) const{
    if (descuento.count(i) > 0){
        Nat maximaCant = descuento.at(i).size() - 1;
        if (k > maximaCant){
            return descuento.at(i)[maximaCant];
        }else{
            return descuento.at(i)[k];
        }
    }else{
        return 0;
    }
}

Nat puesto::gastoPersona(Persona per) const{
    if (venta.count(per) > 0){
        return venta.at(per).gasto;
    }else{
        return 0;
    }
}

Nat puesto::precio(Producto i) const{
    return stockYPrecio.at(i).precio;
}

const list<compra>& puesto::ventas(Persona per) const{
    if (venta.count(per) > 0){
        return venta.at(per).consumos;
    }else{
        list<compra> vacia;
        return vacia;
    }
}
#ifndef APLICARDESCUENTO
#define APLICARDESCUENTO
Nat division(Nat n, Nat k){
    Nat N = n;
    Nat K = k;
    Nat Div = 0;
    while (N >= K){
        Div = 1 + Div;
        N = N - K;
    }
    return Div;
}

Nat AplicarDescuento(Nat p, Nat d){
    return division(p * (100 - d), 100);
}
#endif
void puesto::vender(Persona per, Producto i, Nat k) {
    Nat precio = stockYPrecio.at(i).precio * k;
    Nat des = descuentoItem(i, k);
    Nat precioTotal = AplicarDescuento(precio, des);
    if (des != 0){
        if (venta.count(per) > 0){
            venta.at(per).gasto = venta.at(per).gasto + precioTotal;
            venta.at(per).consumos.push_back(compra(i, k));
        }else{
            list<compra> consumos;
            consumos.push_back(compra(i,k));
            venta.insert(make_pair(per, infoPersona(precioTotal, consumos)));
        }
    }else{
        if (venta.count(per) > 0){
            venta.at(per).gasto = venta.at(per).gasto + precioTotal;
            venta.at(per).consumos.push_back(compra(i, k));
            //guardo el iterador que apunta a la compra realizada sin descuento.
            auto it = prev(venta.at(per).consumos.end());
            if (itCompraSinDescuento.count(per) > 0){
                if (itCompraSinDescuento.at(per).count(i) > 0){
                    itCompraSinDescuento.at(per).at(i).push_back(it);
                }else{
                    list<list<compra>::iterator> itcompras;
                    itcompras.push_back(it);
                    itCompraSinDescuento.at(per).insert(make_pair(i, itcompras));
                }
            }else{
                map<Producto, list<list<compra>::iterator>> dic;
                list<list<compra>::iterator> itcompras;
                itcompras.push_back(it);
                dic.insert(make_pair(i, itcompras));
                itCompraSinDescuento.insert(make_pair(per, dic));
            }
        }else{
            list<compra> consumo;
            consumo.push_back(compra(i, k));
            // Esta parte difiere del diseño dado que teniamos problemas al pedir el iterador
            // de la lista "consumos" antes de agregarla al diccionario
            venta.insert(make_pair(per, infoPersona(precioTotal, consumo)));
            auto it = prev(venta.at(per).consumos.end());
            map<Producto, list<list<compra>::iterator>> dic;
            list<list<compra>::iterator> itcompras;
            itcompras.push_back(it);
            dic.insert(make_pair(i, itcompras));
            itCompraSinDescuento.insert(make_pair(per, dic));
        }
    }
    stockYPrecio.at(i).stock = stockYPrecio.at(i).stock - k;
}

void puesto::olvidarItem(Persona per, Producto i) {
    auto compraAModificar = itCompraSinDescuento.at(per).at(i).front();
    if (compraAModificar->cant == 1){
        venta.at(per).consumos.erase(compraAModificar);
        itCompraSinDescuento.at(per).at(i).pop_front();
    }else{
        compraAModificar->cant = compraAModificar->cant -1;
    }
    stockYPrecio.at(i).stock = stockYPrecio.at(i).stock + 1;
    Nat precio = stockYPrecio.at(i).precio;
    venta.at(per).gasto = venta.at(per).gasto - precio;
}

bool puesto::consumioSinPromo(Persona per, Producto i) {
    if (itCompraSinDescuento.count(per) > 0){
        if (itCompraSinDescuento.at(per).count(i) > 0){
            if (itCompraSinDescuento.at(per).at(i).empty()){
                return false;
            }else{
                return true;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }
}

puesto &puesto::operator=(const puesto& Acopiar) {
    map<Producto, stockPrecio> vacio;
    stockYPrecio = vacio;
    for (auto elem : Acopiar.stockYPrecio){
        stockYPrecio.insert(elem);
    }

    map<Producto, vector<Nat>> vaciod;
    descuento = vaciod;
    for (auto elem : Acopiar.descuento) {
        descuento.insert(elem);
    }

    map<Persona, infoPersona> vaciov;
    venta = vaciov;
    for (auto elem : Acopiar.venta) {
        venta.insert(elem);
    }

    map<Persona, map<Producto, list<list<compra>::iterator>>> vacioi;
    itCompraSinDescuento = vacioi;
    for (auto elem : Acopiar.itCompraSinDescuento) {
        itCompraSinDescuento.insert(elem);
    }
    return *this;
}

puesto::puesto(const puesto &Acopiar){
    *this = Acopiar;
}