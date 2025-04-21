# CC5213 - Recuperación de Información Multimedia
# Profesor: Juan Manuel Barrios
# Fecha: 10 de marzo de 2025

# parametros para compilación: usar C++ 11 y con todos los warnings
CFLAGS += -std=c++11 -Wall -Wextra

#compilar con optimizaciones. Para debug poner -O0 -ggdb
CFLAGS += -O3

#usar opencv para compilación y linkeo
CFLAGS  += $(shell pkg-config --cflags opencv4)
LDFLAGS += $(shell pkg-config --libs   opencv4)

########## archivos a compilar ##########

MAIN_CPP        := $(sort $(wildcard ejemplo*.cpp))

########## archivos a generar ##########

MAIN_BIN  := $(patsubst %,bin/%,$(basename $(MAIN_CPP)))

########## reglas de compilación ##########

#reglas all y clean no corresponden a archivos
.PHONY: all clean

#no borrar archivos intermedios
.PRECIOUS: build/%.o

#por defecto se generan todos los ejecutables de los ejemplos
all: $(MAIN_BIN)

#para cada ejecutable se requiere el object correspondiente más los helpers
bin/%: build/%.o
	mkdir -p "$(@D)"
	g++ $< -o $@ $(LDFLAGS)

#para generar un object se usa el fuente cpp correspondiente más los headers
build/%.o: %.cpp
	mkdir -p "$(@D)"
	g++ -c $(CFLAGS) -o $@ $<

#limpiar todos los archivos de compilación
clean:
	rm -rf bin/ build/
