// PilaHistorial.cpp - Responsable: Mia Sanchez Soto
#include "PilaHistorial.h"
#include <cstdio>

PilaHistorial::PilaHistorial() : tope(nullptr), numAcciones(0) {}

PilaHistorial::~PilaHistorial() {
    vaciar();
}

void PilaHistorial::vaciar() {
    while (tope != nullptr) {
        NodeAccion* temporal = tope;
        tope = tope->siguiente;
        delete temporal;
    }
    numAcciones = 0;
}

bool PilaHistorial::push(const char* texto) {
    if (textoVacio(texto)) return false;
    NodeAccion* nuevo = new NodeAccion;
    copiarTexto(nuevo->texto, texto, MAX_DESCRIPCION);
    nuevo->siguiente = tope;
    tope = nuevo;
    numAcciones++;
    return true;
}

bool PilaHistorial::pop(char* destino, int capacidad) {
    if (tope == nullptr) return false;
    NodeAccion* temporal = tope;
    copiarTexto(destino, temporal->texto, capacidad);
    tope = tope->siguiente;
    delete temporal;
    numAcciones--;
    return true;
}

bool PilaHistorial::peek(char* destino, int capacidad) const {
    if (tope == nullptr) return false;
    copiarTexto(destino, tope->texto, capacidad);
    return true;
}

bool PilaHistorial::vacia() const { return tope == nullptr; }
int PilaHistorial::tamano() const { return numAcciones; }

void PilaHistorial::imprimir() const {
    if (tope == nullptr) {
        printf("(el historial esta vacio)\n");
        return;
    }
    printf("--- Historial de acciones (mas reciente primero) ---\n");
    int posicion = numAcciones;
    for (const NodeAccion* n = tope; n != nullptr; n = n->siguiente) {
        printf("%3d. %s\n", posicion--, n->texto);
    }
}
