# CC5213 - TAREA 1 - RECUPERACIÓN DE INFORMACIÓN MULTIMEDIA
# Fecha: 27 de marzo de 2025
# Alumno: [nombre]

import sys
import os
import util as util


def tarea1_parte1(dir_input_imagenes_R, dir_output_descriptores_R):
    if not os.path.isdir(dir_input_imagenes_R):
        print("ERROR: no existe directorio {}".format(dir_input_imagenes_R))
        sys.exit(1)
    elif os.path.exists(dir_output_descriptores_R):
        print("ERROR: ya existe directorio {}".format(dir_output_descriptores_R))
        sys.exit(1)
    # Implementar la fase offline

    # 1-leer imágenes en dir_input_imagenes
    # puede servir la funcion util.listar_archivos_con_extension() que está definida en util.py

    # 2-calcular descriptores de imágenes
    # ver codigo de ejemplo publicado en el curso

    # 3-escribir en dir_output_descriptores_R los descriptores calculados en uno o más archivos
    # puede servir la funcion util.guardar_objeto() que está definida en util.py

    # borrar las siguientes líneas
    print("NO IMPLEMENTADO!")
    sys.exit(1)


# inicio de la tarea
if len(sys.argv) < 3:
    print("Uso: {}  dir_input_imagenes_R  dir_output_descriptores_R".format(sys.argv[0]))
    sys.exit(1)

# lee los parametros de entrada
dir_input_imagenes_R = sys.argv[1]
dir_output_descriptores_R = sys.argv[2]

# ejecuta la tarea
tarea1_parte1(dir_input_imagenes_R, dir_output_descriptores_R)
