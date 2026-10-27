// Persistencia.cpp - Responsable: Mateo Silva Gamero
#include "Persistencia.h"
#include <cstdio>
#include <cstdlib>
#include <climits>

bool guardarProyectos(const ListaProyectos& lista, const char* ruta) {
    FILE* archivo = fopen(ruta, "w");
    if (archivo == nullptr) return false;

    bool correcto = true;
    for (const NodeProyecto* p = lista.primero(); p != nullptr; p = p->siguiente) {
        if (fprintf(archivo, "%d|%s|%.2f\n", p->id, p->nombre, p->presupuestoInicial) < 0) {
            correcto = false;
        }
    }
    if (fclose(archivo) != 0) correcto = false;
    return correcto;
}

int cargarProyectos(ListaProyectos& lista, const char* ruta, int* lineasInvalidas) {
    if (lineasInvalidas != nullptr) *lineasInvalidas = 0;
    FILE* archivo = fopen(ruta, "r");
    if (archivo == nullptr) return -1;

    int cargados = 0;
    int invalidas = 0;
    char linea[256];

    while (fgets(linea, static_cast<int>(sizeof(linea)), archivo) != nullptr) {
        int largo = 0;
        while (linea[largo] != '\0') largo++;
        while (largo > 0 && (linea[largo - 1] == '\n' || linea[largo - 1] == '\r')) {
            linea[--largo] = '\0';
        }
        if (largo == 0) continue;                    // linea en blanco

        int primera = -1;
        int ultima = -1;
        for (int i = 0; i < largo; i++) {
            if (linea[i] == '|') {
                if (primera < 0) primera = i;
                ultima = i;
            }
        }
        if (primera <= 0 || ultima <= primera) {     // faltan separadores
            invalidas++;
            continue;
        }
        linea[primera] = '\0';
        linea[ultima] = '\0';

        char* finId = nullptr;
        long id = strtol(linea, &finId, 10);
        char* inicioPresupuesto = linea + ultima + 1;
        char* finPresupuesto = nullptr;
        double presupuesto = strtod(inicioPresupuesto, &finPresupuesto);

        bool idValido = (finId != linea && *finId == '\0' && id > 0 && id <= INT_MAX);
        bool presupuestoValido = (finPresupuesto != inicioPresupuesto && *finPresupuesto == '\0');

        if (idValido && presupuestoValido &&
            lista.crear(static_cast<int>(id), linea + primera + 1, presupuesto) == RES_OK) {
            cargados++;
        } else {
            invalidas++;
        }
    }
    fclose(archivo);
    if (lineasInvalidas != nullptr) *lineasInvalidas = invalidas;
    return cargados;
}
