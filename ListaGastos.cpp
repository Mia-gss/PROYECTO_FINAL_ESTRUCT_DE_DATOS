// ListaGastos.cpp - Responsable: Edson Ccopa Mamani
#include "ListaGastos.h"
#include <cstdio>

ListaGastos::ListaGastos() : cabeza(nullptr), cola(nullptr), numGastos(0), siguienteId(1) {}

ListaGastos::~ListaGastos() {
    vaciar();
}

void ListaGastos::vaciar() {
    while (cabeza != nullptr) {
        NodeGasto* temporal = cabeza;
        cabeza = cabeza->siguiente;
        delete temporal;
    }
    cola = nullptr;
    numGastos = 0;
}

Resultado ListaGastos::registrar(double monto, const char* categoria,
                                 const char* descripcion, int* idNuevo) {
    // La forma negada tambien rechaza NaN.
    if (!(monto > 0.0 && monto <= MAX_MONTO) || textoVacio(categoria)) {
        return RES_DATO_INVALIDO;
    }
    NodeGasto* nuevo = new NodeGasto;
    nuevo->datos.id = siguienteId++;
    nuevo->datos.monto = monto;
    copiarTexto(nuevo->datos.categoria, categoria, MAX_NOMBRE);
    copiarTexto(nuevo->datos.descripcion, descripcion, MAX_DESCRIPCION);
    nuevo->siguiente = nullptr;

    if (cola == nullptr) {
        cabeza = nuevo;
    } else {
        cola->siguiente = nuevo;
    }
    cola = nuevo;
    numGastos++;
    if (idNuevo != nullptr) *idNuevo = nuevo->datos.id;
    return RES_OK;
}

Resultado ListaGastos::eliminar(int id) {
    NodeGasto* anterior = nullptr;
    NodeGasto* actual = cabeza;
    while (actual != nullptr && actual->datos.id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }
    if (actual == nullptr) return RES_NO_ENCONTRADO;

    if (anterior != nullptr) {
        anterior->siguiente = actual->siguiente;
    } else {
        cabeza = actual->siguiente;
    }
    if (actual == cola) cola = anterior;   // se elimino el ultimo
    delete actual;
    numGastos--;
    return RES_OK;
}

const NodeGasto* ListaGastos::buscarPorId(int id) const {
    for (const NodeGasto* n = cabeza; n != nullptr; n = n->siguiente) {
        if (n->datos.id == id) return n;
    }
    return nullptr;
}

const NodeGasto* ListaGastos::primero() const { return cabeza; }
int ListaGastos::cantidad() const { return numGastos; }

double ListaGastos::total() const {
    double suma = 0.0;
    for (const NodeGasto* n = cabeza; n != nullptr; n = n->siguiente) {
        suma += n->datos.monto;
    }
    return suma;
}

double ListaGastos::disponible(double presupuesto) const {
    return presupuesto - total();
}

EstadoPresupuesto ListaGastos::evaluar(double presupuesto, double umbral) const {
    double gastado = total();
    if (gastado > presupuesto) return PRESUPUESTO_EXCEDIDO;
    if (presupuesto > 0.0 && gastado > presupuesto * umbral) return PRESUPUESTO_ALERTA;
    return PRESUPUESTO_OK;
}

void ListaGastos::imprimir() const {
    if (cabeza == nullptr) {
        printf("(no hay gastos registrados)\n");
        return;
    }
    printf("%-4s %12s  %-18s %s\n", "ID", "Monto", "Categoria", "Descripcion");
    for (const NodeGasto* n = cabeza; n != nullptr; n = n->siguiente) {
        printf("%-4d %12.2f  %-18s %s\n", n->datos.id, n->datos.monto,
               n->datos.categoria, n->datos.descripcion);
    }
}

void ListaGastos::imprimirResumen(double presupuesto, double umbral) const {
    double gastado = total();
    double porcentaje = (presupuesto > 0.0) ? (gastado / presupuesto) * 100.0 : 0.0;
    printf("Presupuesto inicial : %12.2f\n", presupuesto);
    printf("Total gastado       : %12.2f  (%d gasto(s))\n", gastado, numGastos);
    printf("Presupuesto restante: %12.2f\n", disponible(presupuesto));
    printf("Porcentaje usado    : %11.1f %%\n", porcentaje);
    imprimirAlerta(presupuesto, umbral);
}

void ListaGastos::imprimirAlerta(double presupuesto, double umbral) const {
    EstadoPresupuesto estado = evaluar(presupuesto, umbral);
    if (estado == PRESUPUESTO_EXCEDIDO) {
        printf("*** ALERTA: el presupuesto fue EXCEDIDO por %.2f ***\n", total() - presupuesto);
    } else if (estado == PRESUPUESTO_ALERTA) {
        printf("*** AVISO: se supero el %.0f%% del presupuesto (gastado %.2f de %.2f) ***\n",
               umbral * 100.0, total(), presupuesto);
    }
}
