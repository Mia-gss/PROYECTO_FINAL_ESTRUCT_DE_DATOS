// ListaProyectos.cpp - Responsable: Kevin Bolo Lopez
#include "ListaProyectos.h"
#include <cstdio>

static void imprimirUno(const NodeProyecto* p) {
    printf("[%d] %-25s Presupuesto: %10.2f | Tareas: %d | Gastos: %d\n",
           p->id, p->nombre, p->presupuestoInicial,
           p->tareas.cantidad(), p->gastos.cantidad());
}

ListaProyectos::ListaProyectos() : cabeza(nullptr), cola(nullptr), numProyectos(0) {}

ListaProyectos::~ListaProyectos() {
    vaciar();
}

void ListaProyectos::vaciar() {
    while (cabeza != nullptr) {
        NodeProyecto* temporal = cabeza;
        cabeza = cabeza->siguiente;
        delete temporal;     // libera tambien sus tareas y gastos
    }
    cola = nullptr;
    numProyectos = 0;
}

Resultado ListaProyectos::crear(int id, const char* nombre, double presupuestoInicial) {
    // '|' esta reservado como separador del archivo .txt (ver Persistencia).
    if (id <= 0 || textoVacio(nombre) || textoContiene(nombre, '|') ||
        !(presupuestoInicial >= 0.0 && presupuestoInicial <= MAX_MONTO)) {
        return RES_DATO_INVALIDO;
    }
    if (buscar(id) != nullptr) return RES_ID_DUPLICADO;

    NodeProyecto* nuevo = new NodeProyecto(id, nombre, presupuestoInicial);
    if (cola == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        nuevo->anterior = cola;
        cola->siguiente = nuevo;
        cola = nuevo;
    }
    numProyectos++;
    return RES_OK;
}

Resultado ListaProyectos::modificar(int id, const char* nuevoNombre, double nuevoPresupuesto) {
    NodeProyecto* proyecto = buscar(id);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;

    bool cambiaNombre = !textoVacio(nuevoNombre);
    bool cambiaPresupuesto = (nuevoPresupuesto >= 0.0);

    if (cambiaNombre && textoContiene(nuevoNombre, '|')) return RES_DATO_INVALIDO;
    if (cambiaPresupuesto && nuevoPresupuesto > MAX_MONTO) return RES_DATO_INVALIDO;

    if (cambiaNombre) copiarTexto(proyecto->nombre, nuevoNombre, MAX_NOMBRE);
    if (cambiaPresupuesto) proyecto->presupuestoInicial = nuevoPresupuesto;
    return RES_OK;
}

Resultado ListaProyectos::eliminar(int id) {
    NodeProyecto* proyecto = buscar(id);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;

    if (proyecto->anterior != nullptr) {
        proyecto->anterior->siguiente = proyecto->siguiente;
    } else {
        cabeza = proyecto->siguiente;      // era la cabeza
    }
    if (proyecto->siguiente != nullptr) {
        proyecto->siguiente->anterior = proyecto->anterior;
    } else {
        cola = proyecto->anterior;         // era la cola
    }
    delete proyecto;
    numProyectos--;
    return RES_OK;
}

NodeProyecto* ListaProyectos::buscar(int id) {
    for (NodeProyecto* n = cabeza; n != nullptr; n = n->siguiente) {
        if (n->id == id) return n;
    }
    return nullptr;
}

const NodeProyecto* ListaProyectos::buscar(int id) const {
    for (const NodeProyecto* n = cabeza; n != nullptr; n = n->siguiente) {
        if (n->id == id) return n;
    }
    return nullptr;
}

const NodeProyecto* ListaProyectos::primero() const { return cabeza; }
int ListaProyectos::cantidad() const { return numProyectos; }

void ListaProyectos::listarAdelante() const {
    if (cabeza == nullptr) {
        printf("(no hay proyectos registrados)\n");
        return;
    }
    printf("--- Proyectos (de inicio a fin) ---\n");
    for (const NodeProyecto* n = cabeza; n != nullptr; n = n->siguiente) imprimirUno(n);
}

void ListaProyectos::listarAtras() const {
    if (cola == nullptr) {
        printf("(no hay proyectos registrados)\n");
        return;
    }
    printf("--- Proyectos (de fin a inicio) ---\n");
    for (const NodeProyecto* n = cola; n != nullptr; n = n->anterior) imprimirUno(n);
}
