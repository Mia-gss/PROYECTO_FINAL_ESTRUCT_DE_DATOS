// Ordenamiento.cpp - Responsable: Mia Sanchez Soto
#include "Ordenamiento.h"
#include <cstdio>

RegistroGasto* copiarGastosAArreglo(const ListaGastos& lista, int& cantidad) {
    cantidad = 0;
    int n = lista.cantidad();
    if (n <= 0) return nullptr;

    RegistroGasto* arreglo = new RegistroGasto[n];
    int i = 0;
    for (const NodeGasto* nodo = lista.primero(); nodo != nullptr && i < n; nodo = nodo->siguiente) {
        arreglo[i++] = nodo->datos;
    }
    cantidad = i;
    return arreglo;
}

static void intercambiar(RegistroGasto& a, RegistroGasto& b) {
    RegistroGasto temporal = a;
    a = b;
    b = temporal;
}

static int particionar(RegistroGasto* arreglo, int bajo, int alto, bool ascendente) {
    int medio = bajo + (alto - bajo) / 2;
    intercambiar(arreglo[medio], arreglo[alto]);      // pivote central -> al final
    const double pivote = arreglo[alto].monto;

    int i = bajo - 1;
    for (int j = bajo; j < alto; j++) {
        bool va_antes = ascendente ? (arreglo[j].monto < pivote)
                                   : (arreglo[j].monto > pivote);
        if (va_antes) {
            i++;
            intercambiar(arreglo[i], arreglo[j]);
        }
    }
    intercambiar(arreglo[i + 1], arreglo[alto]);
    return i + 1;
}

static void quicksortRec(RegistroGasto* arreglo, int bajo, int alto, bool ascendente) {
    if (bajo >= alto) return;
    int posicionPivote = particionar(arreglo, bajo, alto, ascendente);
    quicksortRec(arreglo, bajo, posicionPivote - 1, ascendente);
    quicksortRec(arreglo, posicionPivote + 1, alto, ascendente);
}

void quicksortGastos(RegistroGasto* arreglo, int n, bool ascendente) {
    if (arreglo == nullptr || n < 2) return;
    quicksortRec(arreglo, 0, n - 1, ascendente);
}

int busquedaBinariaPorMonto(const RegistroGasto* arreglo, int n, double monto) {
    const double TOLERANCIA = 0.005;
    int bajo = 0;
    int alto = n - 1;
    while (bajo <= alto) {
        int medio = bajo + (alto - bajo) / 2;
        double actual = arreglo[medio].monto;
        if (absoluto(actual - monto) < TOLERANCIA) return medio;
        if (actual < monto) {
            bajo = medio + 1;
        } else {
            alto = medio - 1;
        }
    }
    return -1;
}

int busquedaSecuencialPorCategoria(const ListaGastos& lista, const char* categoria,
                                   bool imprimirCoincidencias) {
    int coincidencias = 0;
    for (const NodeGasto* n = lista.primero(); n != nullptr; n = n->siguiente) {
        if (contieneIgnorandoMayusculas(n->datos.categoria, categoria)) {
            coincidencias++;
            if (imprimirCoincidencias) {
                printf("%-4d %12.2f  %-18s %s\n", n->datos.id, n->datos.monto,
                       n->datos.categoria, n->datos.descripcion);
            }
        }
    }
    return coincidencias;
}

void imprimirArregloGastos(const RegistroGasto* arreglo, int n) {
    if (arreglo == nullptr || n <= 0) {
        printf("(no hay gastos registrados)\n");
        return;
    }
    printf("%-4s %12s  %-18s %s\n", "ID", "Monto", "Categoria", "Descripcion");
    for (int i = 0; i < n; i++) {
        printf("%-4d %12.2f  %-18s %s\n", arreglo[i].id, arreglo[i].monto,
               arreglo[i].categoria, arreglo[i].descripcion);
    }
}
