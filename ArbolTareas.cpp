// ArbolTareas.cpp - Responsable: Esteban Caballero Maravi
#include "ArbolTareas.h"
#include <cstdio>

const char* textoEstado(EstadoTarea estado) {
    switch (estado) {
        case ESTADO_PENDIENTE:   return "Pendiente";
        case ESTADO_EN_PROGRESO: return "En progreso";
        case ESTADO_COMPLETADA:  return "Completada";
    }
    return "?";
}

const char* textoPrioridad(PrioridadTarea prioridad) {
    switch (prioridad) {
        case PRIORIDAD_ALTA:  return "Alta";
        case PRIORIDAD_MEDIA: return "Media";
        case PRIORIDAD_BAJA:  return "Baja";
    }
    return "?";
}

ArbolTareas::ArbolTareas() : primera(nullptr), numTareas(0), siguienteId(1) {}

ArbolTareas::~ArbolTareas() {
    vaciar();
}

// Libera 'nodo', todos sus hermanos siguientes y todos los descendientes.
void ArbolTareas::liberar(NodeTarea* nodo) {
    while (nodo != nullptr) {
        NodeTarea* siguiente = nodo->siguienteHermano;
        liberar(nodo->primerHijo);
        delete nodo;
        nodo = siguiente;
    }
}

void ArbolTareas::vaciar() {
    liberar(primera);
    primera = nullptr;
    numTareas = 0;
}

NodeTarea* ArbolTareas::buscarNodo(NodeTarea* nodo, int id) {
    while (nodo != nullptr) {
        if (nodo->id == id) return nodo;
        NodeTarea* enHijos = buscarNodo(nodo->primerHijo, id);
        if (enHijos != nullptr) return enHijos;
        nodo = nodo->siguienteHermano;
    }
    return nullptr;
}

// Devuelve la direccion del puntero que apunta al nodo buscado
// (permite desenlazarlo sin guardar al anterior).
NodeTarea** ArbolTareas::buscarEnlace(NodeTarea** enlace, int id) {
    while (*enlace != nullptr) {
        if ((*enlace)->id == id) return enlace;
        NodeTarea** enHijos = buscarEnlace(&((*enlace)->primerHijo), id);
        if (enHijos != nullptr) return enHijos;
        enlace = &((*enlace)->siguienteHermano);
    }
    return nullptr;
}

int ArbolTareas::contarNodos(const NodeTarea* nodo) {
    int cuenta = 0;
    while (nodo != nullptr) {
        cuenta += 1 + contarNodos(nodo->primerHijo);
        nodo = nodo->siguienteHermano;
    }
    return cuenta;
}

Resultado ArbolTareas::agregar(int idPadre, const char* nombre, PrioridadTarea prioridad,
                               const char* fechaLimite, int* idNuevo) {
    if (idPadre < 0 || textoVacio(nombre) || !fechaValida(fechaLimite)) {
        return RES_DATO_INVALIDO;
    }
    NodeTarea** enlace = nullptr;
    if (idPadre == 0) {
        enlace = &primera;
    } else {
        NodeTarea* padre = buscarNodo(primera, idPadre);
        if (padre == nullptr) return RES_NO_ENCONTRADO;
        enlace = &(padre->primerHijo);
    }
    // Se agrega al final de la lista de hermanos para conservar el orden de insercion.
    while (*enlace != nullptr) {
        enlace = &((*enlace)->siguienteHermano);
    }

    NodeTarea* nuevo = new NodeTarea;
    nuevo->id = siguienteId++;
    copiarTexto(nuevo->nombre, nombre, MAX_NOMBRE);
    nuevo->estado = ESTADO_PENDIENTE;
    nuevo->prioridad = prioridad;
    copiarTexto(nuevo->fechaLimite, fechaLimite, LEN_FECHA);
    nuevo->primerHijo = nullptr;
    nuevo->siguienteHermano = nullptr;

    *enlace = nuevo;
    numTareas++;
    if (idNuevo != nullptr) *idNuevo = nuevo->id;
    return RES_OK;
}

Resultado ArbolTareas::cambiarEstado(int id, EstadoTarea nuevoEstado) {
    NodeTarea* nodo = buscarNodo(primera, id);
    if (nodo == nullptr) return RES_NO_ENCONTRADO;
    nodo->estado = nuevoEstado;
    return RES_OK;
}

Resultado ArbolTareas::eliminar(int id) {
    NodeTarea** enlace = buscarEnlace(&primera, id);
    if (enlace == nullptr) return RES_NO_ENCONTRADO;

    NodeTarea* nodo = *enlace;
    *enlace = nodo->siguienteHermano;   // desenlazar del nivel actual
    nodo->siguienteHermano = nullptr;   // aislar: solo se libera su subarbol

    numTareas -= contarNodos(nodo);
    liberar(nodo);
    return RES_OK;
}

const NodeTarea* ArbolTareas::buscar(int id) const {
    return buscarNodo(primera, id);
}

int ArbolTareas::cantidad() const {
    return numTareas;
}

void ArbolTareas::imprimirRec(const NodeTarea* nodo, int nivel) {
    for (; nodo != nullptr; nodo = nodo->siguienteHermano) {
        printf("%*s- [%d] %s | %s | Prioridad: %s | Limite: %s\n",
               nivel * 4, "", nodo->id, nodo->nombre,
               textoEstado(nodo->estado), textoPrioridad(nodo->prioridad),
               nodo->fechaLimite[0] != '\0' ? nodo->fechaLimite : "sin fecha");
        imprimirRec(nodo->primerHijo, nivel + 1);   // preorden
    }
}

void ArbolTareas::imprimir() const {
    if (primera == nullptr) {
        printf("(el proyecto no tiene tareas)\n");
        return;
    }
    imprimirRec(primera, 0);
}
