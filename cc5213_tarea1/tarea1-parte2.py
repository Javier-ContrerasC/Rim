# CC5213 - TAREA 1 - RECUPERACIÓN DE INFORMACIÓN MULTIMEDIA
# Fecha: 27 de marzo de 2025
# Alumno: [nombre]

import sys
import os
import util as util


def tarea1_parte2(dir_input_imagenes_Q, dir_input_descriptores_R, file_output_resultados):
    if not os.path.isdir(dir_input_imagenes_Q):
        print("ERROR: no existe directorio {}".format(dir_input_imagenes_Q))
        sys.exit(1)
    elif not os.path.isdir(dir_input_descriptores_R):
        print("ERROR: no existe directorio {} (¿terminó bien tarea1-parte1.py?)".format(dir_input_descriptores_R))
        sys.exit(1)
    elif os.path.exists(file_output_resultados):
        print("ERROR: ya existe archivo {}".format(file_output_resultados))
        sys.exit(1)
    # Implementar la fase online

    # 1-calcular descriptores de Q para imágenes en dir_input_imagenes_Q
    # ver codigo de ejemplo publicado en el curso

    # 2-leer descriptores de R guardados en dir_input_descriptores_R
    # puede servir la funcion util.leer_objeto() que está definida en util.py

    # 3-para cada descriptor q localizar el mas cercano en R

    # 4-escribir en el archivo file_output_resultados un archivo con tres columnas separado por \t:
    # columna 1: imagen_q
    # columna 2: imagen_r
    # columna 3: distancia
    # Puede servir la funcion util.escribir_lista_de_columnas_en_archivo() que está definida util.py

    # borrar las siguientes líneas
    print("NO IMPLEMENTADO!")
    sys.exit(1)


# inicio de la tarea
if len(sys.argv) < 4:
    print("Uso: {}  dir_input_imagenes_Q  dir_input_descriptores_R  file_output_resultados".format(sys.argv[0]))
    sys.exit(1)

# lee los parametros de entrada
dir_input_imagenes_Q = sys.argv[1]
dir_input_descriptores_R = sys.argv[2]
file_output_resultados = sys.argv[3]

# ejecuta la tarea
tarea1_parte2(dir_input_imagenes_Q, dir_input_descriptores_R, file_output_resultados)
