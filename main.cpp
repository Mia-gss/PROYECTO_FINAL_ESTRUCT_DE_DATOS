// =====================================================================
// main.cpp  -  Punto de entrada (version de consola)
// Responsable: Mateo Silva Gamero (Integracion, Menu y Archivos)
// =====================================================================
#include <cstdio>
#include "Sistema.h"
#include "Menu.h"
#include "Persistencia.h"

int main() {
    Sistema sistema;

    int cargados = sistema.cargar(RUTA_PROYECTOS_DEFECTO);
    if (cargados >= 0) {
        printf("Se cargaron %d proyecto(s) desde %s\n", cargados, RUTA_PROYECTOS_DEFECTO);
    }

    ejecutarMenu(sistema);

    if (sistema.guardar(RUTA_PROYECTOS_DEFECTO)) {
        printf("Proyectos guardados en %s. Hasta pronto.\n", RUTA_PROYECTOS_DEFECTO);
    } else {
        printf("Aviso: no se pudieron guardar los proyectos.\n");
    }
    return 0;
}
