// Sistema.cpp - Responsable: Mateo Silva Gamero
#include "Sistema.h"
#include "Ordenamiento.h"
#include "Persistencia.h"
#include <cstdio>

Sistema::Sistema() {}

// ---------------------------- Proyectos ----------------------------
Resultado Sistema::crearProyecto(int id, const char* nombre, double presupuesto) {
    Resultado r = proyectos.crear(id, nombre, presupuesto);
    if (r == RES_OK) {
        char mensaje[MAX_DESCRIPCION];
        snprintf(mensaje, sizeof(mensaje), "Proyecto creado: [%d] %s", id, nombre);
        historial.push(mensaje);
    }
    return r;
}

Resultado Sistema::modificarProyecto(int id, const char* nuevoNombre, double nuevoPresupuesto) {
    Resultado r = proyectos.modificar(id, nuevoNombre, nuevoPresupuesto);
    if (r == RES_OK) {
        char mensaje[MAX_DESCRIPCION];
        snprintf(mensaje, sizeof(mensaje), "Proyecto modificado: [%d]", id);
        historial.push(mensaje);
    }
    return r;
}

Resultado Sistema::eliminarProyecto(int id) {
    Resultado r = proyectos.eliminar(id);
    if (r == RES_OK) {
        char mensaje[MAX_DESCRIPCION];
        snprintf(mensaje, sizeof(mensaje), "Proyecto eliminado: [%d]", id);
        historial.push(mensaje);
    }
    return r;
}

void Sistema::listarProyectos(bool adelante) const {
    if (adelante) {
        proyectos.listarAdelante();
    } else {
        proyectos.listarAtras();
    }
}

// ----------------------------- Tareas ------------------------------
Resultado Sistema::agregarTarea(int idProyecto, int idPadre, const char* nombre,
                                PrioridadTarea prioridad, const char* fechaLimite, int* idNuevo) {
    NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;

    int id = 0;
    Resultado r = proyecto->tareas.agregar(idPadre, nombre, prioridad, fechaLimite, &id);
    if (r != RES_OK) return r;
    if (idNuevo != nullptr) *idNuevo = id;

    char mensaje[MAX_DESCRIPCION];
    if (idPadre == 0) {
        snprintf(mensaje, sizeof(mensaje), "Tarea anadida: [%d] %s (proyecto %d)",
                 id, nombre, idProyecto);
    } else {
        snprintf(mensaje, sizeof(mensaje), "Subtarea anadida: [%d] %s bajo tarea %d (proyecto %d)",
                 id, nombre, idPadre, idProyecto);
    }
    historial.push(mensaje);
    return RES_OK;
}

Resultado Sistema::cambiarEstadoTarea(int idProyecto, int idTarea, EstadoTarea estado) {
    NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;

    Resultado r = proyecto->tareas.cambiarEstado(idTarea, estado);
    if (r == RES_OK) {
        char mensaje[MAX_DESCRIPCION];
        snprintf(mensaje, sizeof(mensaje), "Estado de tarea [%d] cambiado a %s (proyecto %d)",
                 idTarea, textoEstado(estado), idProyecto);
        historial.push(mensaje);
    }
    return r;
}

Resultado Sistema::eliminarTarea(int idProyecto, int idTarea) {
    NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;

    Resultado r = proyecto->tareas.eliminar(idTarea);
    if (r == RES_OK) {
        char mensaje[MAX_DESCRIPCION];
        snprintf(mensaje, sizeof(mensaje), "Tarea eliminada con su subarbol: [%d] (proyecto %d)",
                 idTarea, idProyecto);
        historial.push(mensaje);
    }
    return r;
}

Resultado Sistema::imprimirArbol(int idProyecto) const {
    const NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;
    printf("--- Tareas del proyecto [%d] %s ---\n", proyecto->id, proyecto->nombre);
    proyecto->tareas.imprimir();
    return RES_OK;
}

// ----------------------------- Gastos ------------------------------
Resultado Sistema::registrarGasto(int idProyecto, double monto, const char* categoria,
                                  const char* descripcion) {
    NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;

    int id = 0;
    Resultado r = proyecto->gastos.registrar(monto, categoria, descripcion, &id);
    if (r != RES_OK) return r;

    char mensaje[MAX_DESCRIPCION];
    snprintf(mensaje, sizeof(mensaje), "Gasto anadido: [%d] %.2f en %s (proyecto %d)",
             id, monto, categoria, idProyecto);
    historial.push(mensaje);

    proyecto->gastos.imprimirAlerta(proyecto->presupuestoInicial);   // alerta de umbral
    return RES_OK;
}

Resultado Sistema::mostrarGastos(int idProyecto) const {
    const NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;
    printf("--- Gastos del proyecto [%d] %s ---\n", proyecto->id, proyecto->nombre);
    proyecto->gastos.imprimir();
    return RES_OK;
}

Resultado Sistema::mostrarResumenPresupuesto(int idProyecto) const {
    const NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;
    printf("--- Presupuesto del proyecto [%d] %s ---\n", proyecto->id, proyecto->nombre);
    proyecto->gastos.imprimirResumen(proyecto->presupuestoInicial);
    return RES_OK;
}

// --------------------------- Utilidades ----------------------------
Resultado Sistema::ordenarGastos(int idProyecto, bool ascendente) const {
    const NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;

    int n = 0;
    RegistroGasto* arreglo = copiarGastosAArreglo(proyecto->gastos, n);
    quicksortGastos(arreglo, n, ascendente);
    printf("--- Gastos ordenados por monto (%s) ---\n", ascendente ? "ascendente" : "descendente");
    imprimirArregloGastos(arreglo, n);
    delete[] arreglo;           // delete[] sobre nullptr es seguro
    return RES_OK;
}

Resultado Sistema::buscarGastosPorCategoria(int idProyecto, const char* categoria) const {
    const NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;
    if (textoVacio(categoria)) return RES_DATO_INVALIDO;

    printf("--- Busqueda secuencial por categoria \"%s\" ---\n", categoria);
    int encontrados = busquedaSecuencialPorCategoria(proyecto->gastos, categoria, true);
    if (encontrados == 0) printf("(sin coincidencias)\n");
    return RES_OK;
}

Resultado Sistema::buscarGastoPorMonto(int idProyecto, double monto) const {
    const NodeProyecto* proyecto = proyectos.buscar(idProyecto);
    if (proyecto == nullptr) return RES_NO_ENCONTRADO;

    int n = 0;
    RegistroGasto* arreglo = copiarGastosAArreglo(proyecto->gastos, n);
    quicksortGastos(arreglo, n, true);                  // la binaria exige orden ascendente
    int indice = busquedaBinariaPorMonto(arreglo, n, monto);
    printf("--- Busqueda binaria por monto %.2f ---\n", monto);
    if (indice < 0) {
        printf("(no existe un gasto con ese monto)\n");
    } else {
        printf("Encontrado en la posicion %d del arreglo ordenado:\n", indice);
        imprimirArregloGastos(&arreglo[indice], 1);
    }
    delete[] arreglo;
    return RES_OK;
}

void Sistema::mostrarHistorial() const {
    historial.imprimir();
}

// ----------------------------- Archivos ----------------------------
bool Sistema::guardar(const char* ruta) {
    bool ok = guardarProyectos(proyectos, ruta);
    if (ok) {
        char mensaje[MAX_DESCRIPCION];
        snprintf(mensaje, sizeof(mensaje), "Proyectos guardados en %s", ruta);
        historial.push(mensaje);
    }
    return ok;
}

int Sistema::cargar(const char* ruta) {
    int invalidas = 0;
    int cargados = cargarProyectos(proyectos, ruta, &invalidas);
    if (cargados >= 0) {
        char mensaje[MAX_DESCRIPCION];
        snprintf(mensaje, sizeof(mensaje), "Proyectos cargados desde %s: %d", ruta, cargados);
        historial.push(mensaje);
        if (invalidas > 0) {
            printf("Aviso: %d linea(s) invalida(s) o repetida(s) fueron ignoradas.\n", invalidas);
        }
    }
    return cargados;
}
