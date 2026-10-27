// Menu.cpp - Responsable: Mateo Silva Gamero
#include "Menu.h"
#include "Entrada.h"
#include "Persistencia.h"
#include <cstdio>

static void mostrarResultado(Resultado r) {
    printf("%s\n", mensajeResultado(r));
}

static void imprimirMenu() {
    printf("\n========= ORGANIZADOR DE TAREAS Y PRESUPUESTO =========\n");
    printf(" 1. Crear proyecto\n");
    printf(" 2. Modificar proyecto\n");
    printf(" 3. Eliminar proyecto\n");
    printf(" 4. Listar proyectos\n");
    printf(" 5. Agregar tarea / subtarea\n");
    printf(" 6. Cambiar estado de una tarea\n");
    printf(" 7. Eliminar tarea (con sus subtareas)\n");
    printf(" 8. Ver arbol de tareas\n");
    printf(" 9. Registrar gasto\n");
    printf("10. Ver gastos y presupuesto\n");
    printf("11. Ordenar gastos por monto (Quicksort)\n");
    printf("12. Buscar gastos\n");
    printf("13. Ver historial de acciones\n");
    printf("14. Guardar proyectos en archivo\n");
    printf("15. Cargar proyectos desde archivo\n");
    printf("16. Cargar datos de demostracion\n");
    printf(" 0. Salir\n");
    printf("=======================================================\n");
}

static void opCrearProyecto(Sistema& sistema) {
    int id = 0;
    char nombre[MAX_NOMBRE];
    double presupuesto = 0.0;
    if (!leerEntero("ID del proyecto (entero positivo): ", id)) return;
    if (!leerLinea("Nombre: ", nombre, MAX_NOMBRE)) return;
    if (!leerReal("Presupuesto inicial: ", presupuesto)) return;
    mostrarResultado(sistema.crearProyecto(id, nombre, presupuesto));
}

static void opModificarProyecto(Sistema& sistema) {
    int id = 0;
    char nombre[MAX_NOMBRE];
    double presupuesto = NO_CAMBIAR_PRESUPUESTO;
    bool hayPresupuesto = false;
    if (!leerEntero("ID del proyecto a modificar: ", id)) return;
    if (!leerLinea("Nuevo nombre (Enter = sin cambio): ", nombre, MAX_NOMBRE)) return;
    if (!leerRealOpcional("Nuevo presupuesto (Enter = sin cambio): ", presupuesto, hayPresupuesto)) return;
    if (!hayPresupuesto) presupuesto = NO_CAMBIAR_PRESUPUESTO;
    mostrarResultado(sistema.modificarProyecto(id, nombre, presupuesto));
}

static void opEliminarProyecto(Sistema& sistema) {
    int id = 0;
    if (!leerEntero("ID del proyecto a eliminar: ", id)) return;
    mostrarResultado(sistema.eliminarProyecto(id));
}

static void opListarProyectos(const Sistema& sistema) {
    int orden = 1;
    if (!leerEnteroEnRango("Orden: 1) inicio a fin  2) fin a inicio: ", 1, 2, orden)) return;
    sistema.listarProyectos(orden == 1);
}

static void opAgregarTarea(Sistema& sistema) {
    int idProyecto = 0, idPadre = 0, prioridad = 2;
    char nombre[MAX_NOMBRE];
    char fecha[32];
    if (!leerEntero("ID del proyecto: ", idProyecto)) return;
    if (!leerEntero("ID de la tarea padre (0 = tarea de primer nivel): ", idPadre)) return;
    if (!leerLinea("Nombre de la tarea: ", nombre, MAX_NOMBRE)) return;
    if (!leerEnteroEnRango("Prioridad: 1) Alta  2) Media  3) Baja: ", 1, 3, prioridad)) return;
    if (!leerLinea("Fecha limite AAAA-MM-DD (Enter = sin fecha): ", fecha, static_cast<int>(sizeof(fecha)))) return;

    int idNueva = 0;
    Resultado r = sistema.agregarTarea(idProyecto, idPadre, nombre,
                                       static_cast<PrioridadTarea>(prioridad - 1), fecha, &idNueva);
    mostrarResultado(r);
    if (r == RES_OK) printf("ID asignado a la tarea: %d\n", idNueva);
}

static void opCambiarEstado(Sistema& sistema) {
    int idProyecto = 0, idTarea = 0, estado = 1;
    if (!leerEntero("ID del proyecto: ", idProyecto)) return;
    if (!leerEntero("ID de la tarea: ", idTarea)) return;
    if (!leerEnteroEnRango("Estado: 1) Pendiente  2) En progreso  3) Completada: ", 1, 3, estado)) return;
    mostrarResultado(sistema.cambiarEstadoTarea(idProyecto, idTarea,
                                                static_cast<EstadoTarea>(estado - 1)));
}

static void opEliminarTarea(Sistema& sistema) {
    int idProyecto = 0, idTarea = 0;
    if (!leerEntero("ID del proyecto: ", idProyecto)) return;
    if (!leerEntero("ID de la tarea a eliminar: ", idTarea)) return;
    mostrarResultado(sistema.eliminarTarea(idProyecto, idTarea));
}

static void opVerArbol(const Sistema& sistema) {
    int idProyecto = 0;
    if (!leerEntero("ID del proyecto: ", idProyecto)) return;
    Resultado r = sistema.imprimirArbol(idProyecto);
    if (r != RES_OK) mostrarResultado(r);
}

static void opRegistrarGasto(Sistema& sistema) {
    int idProyecto = 0;
    double monto = 0.0;
    char categoria[MAX_NOMBRE];
    char descripcion[MAX_DESCRIPCION];
    if (!leerEntero("ID del proyecto: ", idProyecto)) return;
    if (!leerReal("Monto (mayor que 0): ", monto)) return;
    if (!leerLinea("Categoria: ", categoria, MAX_NOMBRE)) return;
    if (!leerLinea("Descripcion: ", descripcion, MAX_DESCRIPCION)) return;
    mostrarResultado(sistema.registrarGasto(idProyecto, monto, categoria, descripcion));
}

static void opVerGastos(const Sistema& sistema) {
    int idProyecto = 0;
    if (!leerEntero("ID del proyecto: ", idProyecto)) return;
    Resultado r = sistema.mostrarGastos(idProyecto);
    if (r != RES_OK) {
        mostrarResultado(r);
        return;
    }
    printf("\n");
    sistema.mostrarResumenPresupuesto(idProyecto);
}

static void opOrdenarGastos(const Sistema& sistema) {
    int idProyecto = 0, orden = 1;
    if (!leerEntero("ID del proyecto: ", idProyecto)) return;
    if (!leerEnteroEnRango("Orden: 1) ascendente  2) descendente: ", 1, 2, orden)) return;
    Resultado r = sistema.ordenarGastos(idProyecto, orden == 1);
    if (r != RES_OK) mostrarResultado(r);
}

static void opBuscarGastos(const Sistema& sistema) {
    int idProyecto = 0, tipo = 1;
    if (!leerEntero("ID del proyecto: ", idProyecto)) return;
    if (!leerEnteroEnRango("Buscar por: 1) categoria (secuencial)  2) monto exacto (binaria): ", 1, 2, tipo)) return;
    Resultado r = RES_OK;
    if (tipo == 1) {
        char categoria[MAX_NOMBRE];
        if (!leerLinea("Categoria (o parte de ella): ", categoria, MAX_NOMBRE)) return;
        r = sistema.buscarGastosPorCategoria(idProyecto, categoria);
    } else {
        double monto = 0.0;
        if (!leerReal("Monto exacto: ", monto)) return;
        r = sistema.buscarGastoPorMonto(idProyecto, monto);
    }
    if (r != RES_OK) mostrarResultado(r);
}

static void opGuardar(Sistema& sistema) {
    if (sistema.guardar(RUTA_PROYECTOS_DEFECTO)) {
        printf("Proyectos guardados en %s\n", RUTA_PROYECTOS_DEFECTO);
    } else {
        printf("Error: no se pudo escribir %s\n", RUTA_PROYECTOS_DEFECTO);
    }
}

static void opCargar(Sistema& sistema) {
    int cargados = sistema.cargar(RUTA_PROYECTOS_DEFECTO);
    if (cargados < 0) {
        printf("No existe el archivo %s\n", RUTA_PROYECTOS_DEFECTO);
    } else {
        printf("Se cargaron %d proyecto(s) desde %s\n", cargados, RUTA_PROYECTOS_DEFECTO);
    }
}

// Datos de ejemplo para probar rapidamente todos los modulos.
static void opDemostracion(Sistema& sistema) {
    Resultado r = sistema.crearProyecto(1, "Proyecto Demo", 1000.0);
    if (r != RES_OK) {
        printf("No se pudo crear la demostracion (proyecto 1): %s\n", mensajeResultado(r));
        return;
    }
    int idInvestigacion = 0;
    sistema.agregarTarea(1, 0, "Investigacion", PRIORIDAD_ALTA, "2026-11-15", &idInvestigacion);
    sistema.agregarTarea(1, idInvestigacion, "Revisar fuentes", PRIORIDAD_MEDIA, "2026-11-05", nullptr);
    sistema.agregarTarea(1, idInvestigacion, "Redactar resumen", PRIORIDAD_BAJA, "", nullptr);
    sistema.agregarTarea(1, 0, "Diseno", PRIORIDAD_MEDIA, "2026-11-30", nullptr);
    sistema.registrarGasto(1, 350.0, "Materiales", "Libros y papeleria");
    sistema.registrarGasto(1, 120.5, "Transporte", "Movilidad");
    sistema.registrarGasto(1, 400.0, "Software", "Licencia de herramientas");
    printf("Datos de demostracion cargados en el proyecto 1.\n");
}

void ejecutarMenu(Sistema& sistema) {
    bool salir = false;
    while (!salir) {
        imprimirMenu();
        int opcion = 0;
        if (!leerEntero("Opcion: ", opcion)) break;       // fin de la entrada

        switch (opcion) {
            case 1:  opCrearProyecto(sistema);     break;
            case 2:  opModificarProyecto(sistema); break;
            case 3:  opEliminarProyecto(sistema);  break;
            case 4:  opListarProyectos(sistema);   break;
            case 5:  opAgregarTarea(sistema);      break;
            case 6:  opCambiarEstado(sistema);     break;
            case 7:  opEliminarTarea(sistema);     break;
            case 8:  opVerArbol(sistema);          break;
            case 9:  opRegistrarGasto(sistema);    break;
            case 10: opVerGastos(sistema);         break;
            case 11: opOrdenarGastos(sistema);     break;
            case 12: opBuscarGastos(sistema);      break;
            case 13: sistema.mostrarHistorial();   break;
            case 14: opGuardar(sistema);           break;
            case 15: opCargar(sistema);            break;
            case 16: opDemostracion(sistema);      break;
            case 0:  salir = true;                 break;
            default: printf("Opcion no valida.\n"); break;
        }
    }
}
