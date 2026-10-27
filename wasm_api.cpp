// =====================================================================
// wasm_api.cpp  -  Capa C plana para WebAssembly (Entregable 3)
// Responsable: Mateo Silva Gamero (Integracion, Menu y Archivos)
// Reemplaza a main.cpp/Menu.cpp/Entrada.cpp al compilar con Emscripten:
// el navegador llama estas funciones y la salida (printf) se redirige a
// Module.print. Con g++ normal este archivo solo compila (verificacion).
// Convencion de retorno: 0 = OK, negativo = -(codigo de Resultado).
// =====================================================================
#include <cstdio>
#include "Sistema.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define ED_EXPORT EMSCRIPTEN_KEEPALIVE
#else
#define ED_EXPORT
#endif

static Sistema* g_sistema = nullptr;

static int codigo(Resultado r) {
    return -static_cast<int>(r);
}

extern "C" {

ED_EXPORT void ed_iniciar() {
    if (g_sistema == nullptr) g_sistema = new Sistema();
}

ED_EXPORT void ed_cerrar() {
    delete g_sistema;
    g_sistema = nullptr;
}

ED_EXPORT int ed_crear_proyecto(int id, const char* nombre, double presupuesto) {
    if (g_sistema == nullptr) return -99;
    return codigo(g_sistema->crearProyecto(id, nombre, presupuesto));
}

ED_EXPORT int ed_eliminar_proyecto(int id) {
    if (g_sistema == nullptr) return -99;
    return codigo(g_sistema->eliminarProyecto(id));
}

ED_EXPORT void ed_listar_proyectos(int adelante) {
    if (g_sistema != nullptr) g_sistema->listarProyectos(adelante != 0);
}

// prioridad: 1 alta, 2 media, 3 baja. Devuelve el ID de la tarea (>0) o un codigo negativo.
ED_EXPORT int ed_agregar_tarea(int idProyecto, int idPadre, const char* nombre,
                               int prioridad, const char* fecha) {
    if (g_sistema == nullptr) return -99;
    if (prioridad < 1 || prioridad > 3) return codigo(RES_DATO_INVALIDO);
    int idNuevo = 0;
    Resultado r = g_sistema->agregarTarea(idProyecto, idPadre, nombre,
                                          static_cast<PrioridadTarea>(prioridad - 1), fecha, &idNuevo);
    return (r == RES_OK) ? idNuevo : codigo(r);
}

// estado: 1 pendiente, 2 en progreso, 3 completada.
ED_EXPORT int ed_cambiar_estado_tarea(int idProyecto, int idTarea, int estado) {
    if (g_sistema == nullptr) return -99;
    if (estado < 1 || estado > 3) return codigo(RES_DATO_INVALIDO);
    return codigo(g_sistema->cambiarEstadoTarea(idProyecto, idTarea,
                                                static_cast<EstadoTarea>(estado - 1)));
}

ED_EXPORT int ed_eliminar_tarea(int idProyecto, int idTarea) {
    if (g_sistema == nullptr) return -99;
    return codigo(g_sistema->eliminarTarea(idProyecto, idTarea));
}

ED_EXPORT int ed_imprimir_arbol(int idProyecto) {
    if (g_sistema == nullptr) return -99;
    return codigo(g_sistema->imprimirArbol(idProyecto));
}

ED_EXPORT int ed_registrar_gasto(int idProyecto, double monto, const char* categoria,
                                 const char* descripcion) {
    if (g_sistema == nullptr) return -99;
    return codigo(g_sistema->registrarGasto(idProyecto, monto, categoria, descripcion));
}

ED_EXPORT int ed_listar_gastos(int idProyecto) {
    if (g_sistema == nullptr) return -99;
    return codigo(g_sistema->mostrarGastos(idProyecto));
}

ED_EXPORT int ed_resumen_presupuesto(int idProyecto) {
    if (g_sistema == nullptr) return -99;
    return codigo(g_sistema->mostrarResumenPresupuesto(idProyecto));
}

ED_EXPORT int ed_ordenar_gastos(int idProyecto, int ascendente) {
    if (g_sistema == nullptr) return -99;
    return codigo(g_sistema->ordenarGastos(idProyecto, ascendente != 0));
}

ED_EXPORT void ed_mostrar_historial() {
    if (g_sistema != nullptr) g_sistema->mostrarHistorial();
}

}  // extern "C"
