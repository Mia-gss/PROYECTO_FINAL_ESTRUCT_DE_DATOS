CXX      ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic -O2
BIN      = organizador

SRC_NUCLEO = src/Comun.cpp src/ListaGastos.cpp src/ArbolTareas.cpp src/ListaProyectos.cpp \
             src/PilaHistorial.cpp src/Ordenamiento.cpp src/Persistencia.cpp src/Sistema.cpp
SRC_CONSOLA = $(SRC_NUCLEO) src/Entrada.cpp src/Menu.cpp src/main.cpp
SRC_WASM    = $(SRC_NUCLEO) src/wasm_api.cpp

EXPORTADAS = ["_ed_iniciar","_ed_cerrar","_ed_crear_proyecto","_ed_eliminar_proyecto","_ed_listar_proyectos","_ed_agregar_tarea","_ed_cambiar_estado_tarea","_ed_eliminar_tarea","_ed_imprimir_arbol","_ed_registrar_gasto","_ed_listar_gastos","_ed_resumen_presupuesto","_ed_ordenar_gastos","_ed_mostrar_historial"]

all: $(BIN)

$(BIN): $(SRC_CONSOLA)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_CONSOLA)

# Entregable 3 (requiere Emscripten instalado):
wasm: $(SRC_WASM)
	mkdir -p web
	emcc -std=c++11 -O2 $(SRC_WASM) -o web/organizador.js \
	  -s EXPORTED_FUNCTIONS=\x27$(EXPORTADAS)\x27 \
	  -s EXPORTED_RUNTIME_METHODS=\x27["ccall","cwrap"]\x27

clean:
	rm -f $(BIN) proyectos.txt

run: $(BIN)
	./$(BIN)

.PHONY: all wasm clean run
