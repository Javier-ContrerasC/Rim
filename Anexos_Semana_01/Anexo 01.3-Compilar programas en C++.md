# CC5213 - Recuperación de Información Multimedia
## Profesor: Juan Manuel Barrios
## Fecha: 10 de marzo de 2025

-----------------------------------------------
En el curso se mostrarán algunos ejemplos de código en C++. A continuación se describe cómo compilar código fuente en C++.

## Cómo compilar programas en C++

Para compilar un programa en C++ usualmente se requieren dos etapas:

  1. Paso 1 (`-c`): Convertir código fuente .cpp en código binario .o (object file).
      - Para cada `.cpp` se debe ejecutar `g++ -c -o archivo1.o archivo1.cpp`.
      - El compilador lee el fuente en el `.cpp`, resuelve todas las instrucciones `#include ...`  y si el código no tiene problemas de sintaxis generará un `.o` con la versión binaria del fuente.
      - Parámetros `-Iruta` configuran lugares para buscar headers con definiciones de métodos (además de la ruta por defecto `/usr/include/`)
      - El parámetro `-std=c++17` permite usar C++17 (algo bastante común a estas alturas)
      - Los parámetros `-Wall` para warnings y `-Wextra` para aún más warnings.

  2. Paso 2 (linkeo): Crear el ejecutable o librería.
      - Se reúnen todos los object file `.o` y se asocia cada llamada a un método con su implementación, ya sea entre object files o con librerías externas (linkage).
      - Para crear ejecutable: `g++ -o ejemplo archivo1.o archivo2.o` Notar que entre todos los `.o` debe existir un único método `main()`.
      - Para crear una librería compartida: `g++ -shared -o libejemplo.so archivo1.o archivo2.o` (en linux son `.so`, en windows son `.dll`)
      - Parámetros `-Lruta` configuran lugares para buscar librerías compartidas (además de la ruta por defecto `/usr/lib/`).
      - Parámetros `-lnombre` declaran las librerías externas a buscar (se buscará un archivo `libnombre.so` en las rutas definidas por `-L`).

Notar que para proyectos pequeños, puede ser más fácil compilar y linkear en un solo comando, como `g++ -o ejemplo -Imi_opencv/include *.cpp -Lmi_opencv/lib -lopencv_core -lopencv_imgproc`

El comando `-L` permite linkear librerías `.so` o `.dll` que no están en una ruta por defecto. Luego de usarlo es muy probable que al ejecutar ese binario no se encuentren las librerías (en Linux aparecerá el mensaje `error: cannot open shared object file`). Para solucionarlo:

### En Windows
  - Agregar a la variable de entorno `PATH` las rutas donde están las librerías `.dll` para la ejecución de un binario.
  - Es posible usar la herramienta Dependency Walker (`depends.exe`) para saber qué librerías se están encontrando y usando al ejecutar un programa.

### En Linux
  - Opción 1: Agregar a la variable de entorno `LD_LIBRARY_PATH` las rutas donde están las librerías `.so` para la ejecución de un binario.
  - Opción 2: Es posible compilar `g++` con el parámetro `-Wl,-rpath,ruta/lib` para que el linker incluya el path completo (y no solo el nombre) en la lista de librerías requeridas.
  - El comando `ld` de Linux lista las dependencias declaradas por un binario y cómo las está resolviendo el sistema operativo. Permite saber qué librerías se están encontrando y usando al ejecutar un programa.
  - El comando `pkg-config` entrega los parámetros necesarios para compilar usando librerías externas. Para obtener los parámetros de compilación `pkg-config --cflags opencv`. Para los parámetros de linkeo `pkg-config --libs opencv`. Esos parámetros están definidos en archivos `.pc` ubicados en `/lib/pkgconfig/`. Si opencv se instaló en `$HOME/mi_opencv` se debe agregar la ruta `$HOME/mi_opencv/lib/pkgconfig` a la variable de entorno `PKG_CONFIG_PATH`.

Al desarrollar programas en C++ es común crear un archivo de nombre `Makefile` con las instrucciones de compilación. El comando `make` lee ese archivo y ejecuta las intrucciones necesarias. Para detalles ver el archivo Makefile de los ejemplos.

---------------------------------------------

## Entorno de desarrollo (IDE) para C++

Si se desea hacer las tareas del curso en C++ puede ser suficiente usar un editor de texto y compilar por línea de comandos. Sin embargo, siempre es muy útil programar usando un IDE adecuado. Los más usados para C++ son Codeblocks, Eclipse CDT, QtCreator.

**Visual Studio Code** tiene extensiones para C++. Además, es necesario configurarlo para que utilice MinGW64. Para eso presionnar Control+Shift+P buscar "C++: Select IntelliSense Configuration" y seleccionar el compilador g++ de mingw64.

**Eclipse CDT** se puede descargar desde https://www.eclipse.org/downloads/packages/ versión "Eclipse IDE for C/C++ Developers" para Windows o Linux. Descargar el `.tar.gz` o `.zip` y descomprimir.

Eclipse CDT puede generar automáticamente un Makefile según la configuración dada en las propiedades del proyecto. También se puede usar un Makefile externo para compilar un proyecto. Para esto en propiedades del proyecto escoger "Tool Chain Editor" y en current builder usar "Gnu Make Builder". Luego en "C/C++ Build" desmarcar "Generate Makefiles automatically" y en "Build directory" escoger la carpeta donde está el archivo Makefile. 

Para que Eclipse CDT utilice el autocompletar, es necesario que el Indexer encuentre las librerías externas. En particular, si los .h de OpenCV no están en una ruta por defecto (como `/usr/include/`) hay que agregarlo a la lista de includes del proyecto. En "Properties" entrar a "Path and Symbols", en Includes presionar "Add..." y agregar la ruta (en Windows puede ser `C:/msys64/mingw64/include/opencv4`), marcar all configurations y all languages.

Al usar un makefile externo es útil usar un parser de la salida de la compilación. Así Eclipse lee los comandos que ejecutó el Makefile y los replica en la configuración del proyecto permitiendo que se configure automáticamente. En "Properties"->"C/C++ General"->"Preprocessor Include paths"->Providers->marcar "CDT GCC Build Output Parser".

---------------------------------------------
:computer:
