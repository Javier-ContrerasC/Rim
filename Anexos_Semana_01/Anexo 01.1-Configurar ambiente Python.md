# CC5213 - Recuperación de Información Multimedia
## Profesor: Juan Manuel Barrios
## Fecha: 10 de marzo de 2025

-----------------------------------------------
En el curso se mostrarán distintos ejemplos de código fuente en Python y se deberán programar algunas tareas, por lo que es necesario configurar un ambiente para poder ejecutar programas en python.

# 1. Configurar ambiente Python

Existen varias formas de instalar Python. Se puede descargar directamente o usar una distribución.

**MiniForge** https://conda-forge.org/ es una distribución de python que ofrece un administrador de paquetes (`conda`) y que usa un repositorio de paquetes libre (llamado `conda-forge`) para descargar librerías compiladas para Windows, Linux y MacOS.

Existen otras distribuciones de python basadas en `conda` (como *Anaconda* y *MiniConda*) que son gratuitos para usar pero que utilizan repositorios de paquetes que no son de libre de uso. Más detalles en https://www.anaconda.com/blog/is-conda-free

Al crear un ambiente con `conda` se instala el intérprete de python y también se instala `pip` para descargar librerías. A veces se vuelve difícil lograr compatibilidad entre todas las librerías, por lo que hay que probar instalando con `pip` o con `conda` o cambiando python a una versión más antigua.

## Instalar MiniForge en Windows

1. En el panel de configuración de Windows entrar a "Aplicaciones" y "Alias de ejecución de aplicaciones". Aparece un listado de programas, desactivar "python.exe" y "python3.exe" que está asociado a "Instalador de aplicación". Si no se hace esto, al escribir "python" Windows abrirá el Microsoft Store.

2. Entrar a https://conda-forge.org/download/ y descargar el instalador para Windows.

3. Doble click en el instalador `Miniforge3-Windows-x86_64.exe`

4. Seleccionar instalación recomendada (usar "Just me") y ruta de instalación por defecto (propone `%USERPROFILE%\miniconda3`)

5. Aceptar configuración propuesta (crear accesos directos, no agregar al PATH, no registrar como intérprete python por defecto)

5. Para abrir un terminal python, en el menú de inicio de Windows buscar "Miniforge Prompt"


## Instalar MiniForge en Linux o MacOS

1. Entrar a https://conda-forge.org/download/ y descargar el instalador.

2. Abrir un terminal y ejecutar el script descargado: `bash Miniforge3-Linux-x86_64.sh`

Aparece este mensaje:
```
In order to continue the installation process, please review the license agreement.
Please, press ENTER to continue
```
Presionar la tecla ENTER.

Aparece la licencia (avanzar página o presionar 'q')
````
Do you accept the license terms? [yes|no]
````
Escribir "yes".

Pregunta la ruta de instalación:
````
Miniforge3 will now be installed into this location:
[$HOME/miniforge3] >>>
````

Está bien esa ruta la ruta por defecto `$HOME/miniforge3`.

Presionar la tecla ENTER.

A continuación descarga e instala varios paquetes.

Al finalizar pregunta si modifica el script de inicio (`$HOME/.bashrc`):

````
Do you wish to update your shell profile to automatically initialize conda?
````
Escribir "yes".

3. Ejecutar `source %HOME/.bashrc` o abrir un nuevo terminal para usar la nueva configuración.

4. Si al abrir un terminal dice `(base)` es porque conda siempre está activado. Se recomienda no dejarlo siempre activo y activar manualmente el ambiente que uno quiere. Para eso ejecutar el comando: `conda config --set auto_activate_base false` y abrir un nuevo terminal.


## Configurar ambiente para el curso

1. Abrir un terminal de **MiniForge**.

2. **Actualizar conda**: En el terminal escribir `conda update --all`.

3. **Listar ambientes disponibles**: Se pueden mostrar los ambientes configurados `conda env list`

4. **Crear un ambiente para el curso**: Crear un nuevo ambiente para el curso con:
```
 conda create -n cc5213
 conda activate cc5213
 conda install python
```

5. **Instalar OpenCV**: El package `opencv-contrib-python` contiene opencv más las contribuciones de terceros. Actualmente instala OpenCV 4.11.0.
```
 pip install opencv-contrib-python
```

**Nota**: Al usar la última versión de python a veces no están todas las librerías o pueden ser incompatibles. Usualmente esto se corrige usando una versión de python más antigua. Para crear un ambiente con un python más antiguo:
```
 conda env remove -n cc5213
 conda create -n cc5213 python=3.11
 conda activate cc5213
```


## Jupyter Notebooks

En el curso se publicarán archivos `.ipynb` con código fuente de ejemplo. Para abrirlos usar **Jupyter Notebook** que es un IDE web que puede abrir este tipo de archivos. Se puede instalar con:
```
 pip install jupyter
```

Para iniciarlo, abrir un terminal, cambiar a la ubicación de los archivos y escribir `jupyter notebook`
```
 cd C:\[ruta]\CC5213\
 jupyter notebook
```

Es posible evitar que abra el navegador o cambiar el puerto por defecto (útil cuando se usa un terminal remoto):
```
 jupyter notebook --no-browser --port=8890
```


## Entorno de desarrollo (IDE) para Python

Para ejecutar un archivo `.py` basta ejecutarlo con el comando python. Por ejemplo: `python ejemplo1.py`

Para crear programas `.py` se puede usar un editor de archivos de texto (como "Notepad++" o "Sublime"), aunque para las tareas de programación se recomienda usar un ambiente de programación (IDE).

  * **Spyder** es un IDE libre y simple. Se puede instalar con `pip install spyder` o descargar desde https://www.spyder-ide.org/

  * **PyCharm Community Edition** es un IDE libre que es más avanzado. Notar que algunas funcionalidades sólo están en la versión Professional (pagada) https://www.jetbrains.com/pycharm/download/

  * **Visual Studio Code** es un IDE libre multi-lenguaje extensible por medio de plugins. Descargar desde https://code.visualstudio.com/ y descargar los plugins para Python.

Notar que todos los IDE será necesario configurarlo para que use python desde nuestro ambiente conda. Dentro de las opciones del IDE se debe buscar alguna opción que diga "Python interpreter" y congiurar que use el ambiente conda `cc5213`.

Al implementar una tarea es útil formatear el código:
  * En PyCharm usar "Code" -> Reformat Code (o Ctrl+Alt+L)
  * En VSCode se debe instalar un plugin (File->Preferences->Extensions->buscar "Black Formatter"). Una vez instalado, click derecho en el código fuente y escoger el formateador por defecto. Luego formatear con Shift+Alt+F.


---------------------------------------------
:computer:
