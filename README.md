# Organizador Inteligente de Tareas y Gestion de Presupuesto
Segundo Entregable (avance integrado) - Grupo 8 - Estructura de Datos I

C++11, sin STL en las estructuras de datos (solo punteros, clases y structs).
Temas usados: registros, lista doble, lista simple, pila, Quicksort, busqueda
secuencial/binaria y arbol N-ario. No hay grafos ni dependencias.

## Compilar y ejecutar (consola)
    make          # genera ./organizador
    ./organizador # (en Windows con MinGW: mingw32-make)

Sin make:
    g++ -std=c++11 -Wall -Wextra -pedantic -o organizador src/Comun.cpp src/ListaGastos.cpp src/ArbolTareas.cpp src/ListaProyectos.cpp src/PilaHistorial.cpp src/Ordenamiento.cpp src/Persistencia.cpp src/Sistema.cpp src/Entrada.cpp src/Menu.cpp src/main.cpp

Prueba rapida: opcion 16 (datos de demostracion), luego 8, 10, 11, 12 y 13.

## Archivos por responsable
| Responsable | Archivos |
|---|---|
| Kevin Bolo Lopez (Proyectos, lista doble) | NodeProyecto.h, ListaProyectos.h/.cpp |
| Esteban Caballero Maravi (Tareas, arbol N-ario) | NodeTarea.h, ArbolTareas.h/.cpp |
| Edson Ccopa Mamani (Gastos, lista simple) | NodeGasto.h, ListaGastos.h/.cpp |
| Mia Sanchez Soto (Utilidades) | PilaHistorial.h/.cpp, Ordenamiento.h/.cpp |
| Mateo Silva Gamero (Integracion) | Comun.h/.cpp, Sistema.h/.cpp, Persistencia.h/.cpp, Entrada.h/.cpp, Menu.h/.cpp, main.cpp, wasm_api.cpp |

## Arquitectura pensada para el Entregable 3 (WebAssembly)
- El nucleo (todo salvo Entrada, Menu y main) no lee del teclado: solo recibe
  parametros y escribe con printf. Por eso se reutiliza igual en consola y en web.
- `Sistema` es la unica fachada. `wasm_api.cpp` la expone con funciones C planas
  (prefijo `ed_`), con retorno 0/ID = OK y negativo = -(codigo de Resultado).
- Compilacion a WASM (requiere Emscripten instalado):
      make wasm     # genera web/organizador.js y web/organizador.wasm
  Uso desde JS:  const crear = Module.cwrap('ed_crear_proyecto','number',['number','string','number']);
- Los archivos .txt dentro del navegador usan el sistema de archivos virtual de
  Emscripten (en memoria). Para persistir entre recargas habra que montar IDBFS.

## Limitaciones de esta version
- El .txt guarda solo proyectos (ID|nombre|presupuesto); tareas y gastos no se persisten aun.
- Los IDs de tareas y gastos se autogeneran por proyecto.
- Nombres de proyecto no pueden contener el caracter '|' (separador del archivo).
