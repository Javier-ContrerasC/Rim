# CC5213 - Recuperación de Información Multimedia
# Profesor: Juan Manuel Barrios
# Fecha: 15 de abril de 2025

# parametros para compilación: usar C++17 y con todos los warnings
CFLAGS += -std=c++17 -Wall -Wextra

#compilar con optimizaciones. Para debug poner -O0 -ggdb
CFLAGS += -O3

#usar opencv para compilación y linkeo
CFLAGS  += $(shell pkg-config --cflags opencv4 fftw3)
LDFLAGS += $(shell pkg-config --libs   opencv4 fftw3)

########## archivos a compilar ##########

MAIN_CPP        := $(sort $(wildcard ejemplo*.cpp))
HELPERS_CPP     := $(wildcard helper.cpp Mfcc.cpp)
HELPERS_HEADERS := $(wildcard helper.hpp Mfcc.hpp)

########## archivos a generar ##########

MAIN_BIN  := $(patsubst %,bin/%,$(basename $(MAIN_CPP)))
HELPERS_O := $(patsubst %,build/%,$(HELPERS_CPP:.cpp=.o))

########## reglas de compilación ##########

#reglas all y clean no corresponden a archivos
.PHONY: all clean

#no borrar archivos intermedios
.PRECIOUS: build/%.o

#por defecto se generan todos los ejecutables de los ejemplos
all: $(MAIN_BIN)

#para cada ejecutable se requiere el object correspondiente más los helpers
bin/%: build/%.o $(HELPERS_HEADERS) $(HELPERS_O)
	mkdir -p "$(@D)"
	g++ $< -o $@ $(HELPERS_O) $(LDFLAGS)

#para generar un object se usa el fuente cpp correspondiente más los headers
build/%.o: %.cpp $(HELPERS_HEADERS)
	mkdir -p "$(@D)"
	g++ -c $(CFLAGS) -o $@ $<

#limpiar todos los archivos de compilación
clean:
	rm -rf bin/ build/
