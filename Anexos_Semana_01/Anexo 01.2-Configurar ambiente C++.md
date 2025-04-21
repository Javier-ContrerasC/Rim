# CC5213 - Recuperación de Información Multimedia
## Profesor: Juan Manuel Barrios
## Fecha: 10 de marzo de 2025

-----------------------------------------------
# Configurar ambiente C++

Para usar C++ la primera decisión es qué compilador usar. En Linux lo más común es usar **`g++`** de GNU, mientras que en Windows se puede usar `g++` (existen distintas implementaciones como Cygwin, MinGW y MinGW-w64) y `cl` (Visual Studio de Microsoft). También están los compiladores de IBM y de Intel.

Cada compilador de C++ implementa su propio "name mangling" para almacenar las funciones en una librería (necesario para soportar clases, overloading, etc.), por tanto no es posible linkear librerías de C++ generada por distintos compiladores. En particular, al compilar con `g++` NO se pueden usar librerías generadas por `cl`. Notar que sí es posible mezclar librerías de C generadas por distintos compiladores porque en C no existe name mangling.

## Ubuntu: Instalar compilador g++

En Ubuntu para instalar `g++` y todo el ambiente basta ejecutar el comando:

```
sudo apt install build-essential
```

Para instalar la librería de OpenCV ya compilada basta ejecutar:

```
sudo apt install libopencv-dev
```

Para instalar algunos clasificadores que vienen incluidos con opencv (quedan en `/usr/share/opencv4/haarcascades/`)
```
sudo apt install opencv-data
```


## Windows: (Alternativa 1, recomendado) Instalar MSYS2 y MinGW-w64

**MinGW-w64** es un compilador `g++` que genera binarios nativos de Windows. Con pocas modificaciones se puede lograr que el mismo código fuente compile en Windows y Linux.

Las diferencias Windows y Linux son funciones de acceso al filesystem (`scandir`, `mkdir`, `stat`, `realpath`, aunque desde C++17 se resuelve con `std::filesystem`), funciones de manejo de procesos (en Windows no existe `fork`, `waitpid`, `ioctl`) y una función de strings (`vsnprintf`).

Lamentablemente al usar **MinGW-w64** no se puede usar directamente GPU porque las DLL de cuda están compiladas con Visual Studio y como se mencionó anteriormente no se pueden linkear librerías C++ de diferentes compiladores. Una opción factible es usar las librerías de cuda para C.

Además del compilador se necesitan herramientas básicas para compilar (como `bash`, `make`, `find`, `grep`, `make`). **MSYS2** es un proyecto que proporciona estos comandos compilados para Windows. Incluye un administrador de paquetes (`pacman`) que permite descargar versiones compiladas de todas las librerías.

MSYS2 tiene distintos ambientes (msys, ucrt64, clang64, mingw32, mingw64) que son configuraciones de variables de entorno, compilador, librerías, etc. Usaremos el ambiente "mingw64".

Los pasos para instalar el ambiente MSYS2 usando MinGW-w64 son:

  1. **Descargar MSYS2**: Entrar a https://www.msys2.org/ descargar (actualmente `msys2-x86_64-20250221.exe`) e instalar en `C:\msys64`.
  2. **Actualizar**: Abrir un terminal "MSYS2 MINGW64" y escribir `pacman -Syu` hasta que diga que está todo actualizado.
  3. **Comandos para development**: `pacman -S base-devel msys2-devel`
  4. **Compilador MinGW-w64**: `pacman -S mingw-w64-x86_64-toolchain` (grupo de packages, aceptar todo)
  5. **OpenCV**: `pacman -S mingw-w64-x86_64-opencv` actualmente instala OpenCV 4.10.0.
  6. **Otras librerías**: `pacman -S zip unzip mingw-w64-x86_64-ffmpeg mingw-w64-x86_64-fftw mingw-w64-x86_64-imagemagick mingw-w64-x86_64-qt5 mingw-w64-x86_64-qt6`.
	  
En pacman agregar el parámetro `--needed` para evitar reinstalar los packages que ya estén instalados. Con `pacman -Ss nombre` es posible buscar packages para instalar.

Para compilar con MinGW-w64 hay dos opciones:

1. Abrir una ventana "MSYS2 MINGW64" que inicia un terminal `bash` con las rutas ya configuradas.

    * Hay que tener cuidado de no usar backslash \ en las rutas, porque en bash se usa para formar caracteres especiales (\n, \t, etc.).

    * El directorio raíz `/` está apuntando a `C:\msys64`

    * El directorio `/home/usuario` está en `C:\msys64\home\usuario`

    * El disco C: está en `/c/` y si hay un disco D: estará en `/d/`

2. Abrir un terminal de Windows `cmd.exe`. Se deben configurar variables de ambiente apuntando a las rutas de msys2.

    *  Al abrir el terminal hay que agregar al `PATH` las rutas a los binarios de msys2:

```
set PATH=C:\msys64\mingw64\bin;C:\msys64\mingw64\x86_64-w64-mingw32\bin;C:\msys64\usr\bin;%PATH%
```

    * Con eso se pueden usar los comandos de msys2 (g++, make, grep, bash) dentro del terminal de Windows.

	* Al compilar con c++ es muy útil el comando `pkg-config` que ayuda con los parámetros para compilar con una librería. Se debe definir la variable `PKG_CONFIG_PATH` a los directorios donde están ubicados los archivos .pc de configuración.

```
set PKG_CONFIG_PATH=C:\msys64\mingw64\lib\pkgconfig;C:\msys64\usr\lib\pkgconfig
```
    
	* Para no tener que configurar estas variable cada vez que se abre un terminal, se puede crear un archivo que hace esta configuración e inicial la línea de comandos. Para esto crear un archivo llamado `msys2.bat` con el siguiente texto:

```
@echo off
set PATH=C:\msys64\mingw64\bin;C:\msys64\mingw64\x86_64-w64-mingw32\bin;C:\msys64\usr\bin;%PATH%
set PKG_CONFIG_PATH=C:\msys64\mingw64\lib\pkgconfig;C:\msys64\usr\lib\pkgconfig
C:\Windows\System32\cmd.exe /K "cd /d C:\Users\..."
```

Al hacer doble click sobre `msys2.bat` se abrirá el terminal ya configurado.


### (Alternativa 2) Instalar Cygwin

**Cygwin** es un port de Linux para Windows que entrega una gran cantidad de comandos (como `bash`, `g++`, `make`, `grep`, etc.).

La principal ventaja es que el código fuente de los programas de Linux pueden ser compilados en Windows prácticamente sin modificaciones.

La desventaja es que los programas usan una DLL de "emulación" de Linux sobre Windows que hace más lenta la ejecución.


### (Alternativa 3) Instalar Visual Studio

No confundir con *Visual Studio Code* que es un editor de código fuente.

**Visual Studio** es el entorno de desarrollo oficial para Windows. Su compilador de C++ se llama `cl`. Se debe descargar Visual Studio del sitio de Microsoft. La versión profesional es pagada pero existe una versión gratuita llamada "Community".

La mayor ventaja de usar Visual Studio es que produce aplicaciones nativas de Windows para 32 y 64 bits con la posibilidad de usar GPU y otras aceleraciones.

Una posible desventaja es que Visual Studio fomenta el uso de la API exclusiva de Microsoft, lo que vuelve difícil compilar ese programa después en Linux. El compilador `cl` es difícil de manejar desde la línea de comandos, por lo que hay que configurarlo a través del IDE. Al parecer es posible usar C++17 (Project > Properties > C/C++ > Language > C++ Language Standard).

Para usar OpenCV con Visual Studio:

  1. Descargar la distribución para Windows de `https://opencv.org/releases/`, descargar `opencv-xxx-windows.exe` y descomprimir en `C:\[ZZZ]\opencv`
  2. La descarga incluye las DLL para Visual Studio por lo que basta agregar al PATH la ruta `C:\[ZZZ]\opencv\build\x64\vc16\bin`.
  3. Además se debe agregar al PATH la ruta `C:\[ZZZ]\opencv\build\bin` para usar la DLL de ffmpeg que contiene distintos codecs. Si no se realiza este paso no se podrán leer archivos `.mp4` ni otros videos.

También es posible compilar los fuentes de OpenCV. Para esto se requiere bajar e instalar CMake desde https://cmake.org/download/. Al ejecutar cmake sobre los fuentes se genera un archivo `.solution` el cual se abre con Visual Studio. En el IDE se presiona sobre el botón compilar y (luego de mucho tiempo) genera las DLLs.

---------------------------------------------
:computer:
