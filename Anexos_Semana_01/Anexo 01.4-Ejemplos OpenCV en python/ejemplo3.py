# CC5213 - Recuperación de Información Multimedia
# Profesor: Juan Manuel Barrios
# Fecha: 10 de marzo de 2025

import sys
import os
import cv2


def open_video(filename):
    capture = None
    if filename.isdigit():
        capture = cv2.VideoCapture(int(filename), cv2.CAP_DSHOW)
    elif os.path.isfile(filename):
        capture = cv2.VideoCapture(filename)
    if capture is None or not capture.isOpened():
        print("error abriendo {}".format(filename))
        sys.exit()
    return capture


def open_detector(xml):
    file_xml = cv2.data.haarcascades + xml
    print("leyendo {}".format(file_xml))
    detector = cv2.CascadeClassifier()
    if not detector.load(file_xml):
        print("no puedo cargar {}".format(xml))
        sys.exit()
    return detector


def ejemplo3(filename, xml, scale_factor, min_neighbors):
    # cargar el detector
    detector = open_detector(xml)
    # abrir video
    capture = open_video(filename)
    # procesar frames
    window_name = "video " + filename
    cv2.namedWindow(window_name, cv2.WINDOW_AUTOSIZE)
    while capture.grab():
        retval, frame_color = capture.retrieve()
        if not retval:
            continue
        frame_gris = cv2.cvtColor(frame_color, cv2.COLOR_BGR2GRAY)
        detecciones = detector.detectMultiScale(frame_gris, scale_factor, min_neighbors)
        print("detecciones: {}".format(len(detecciones)))
        for (x, y, w, h) in detecciones:
            center = (int(x + w / 2), int(y + h / 2))
            size = (int(w / 2), int(h / 2))
            color = (255, 255, 0)
            thickness = 3
            cv2.ellipse(frame_color, center, size, 0, 0, 360, color, thickness)
        cv2.imshow(window_name, frame_color)
        key = cv2.waitKey(1)
        if key == ord('q') or key == 27:
            break
    capture.release()
    cv2.destroyAllWindows()


if len(sys.argv) < 5:
    print("Uso:")
    print(" {}".format(sys.argv[0]))
    print("   video          Video o Id de la webcam. Ej: 0, 1, 2 ")
    print("   XML_detector   Archivo detector. Ej: haarcascade_frontalface_default.xml")
    print("   scale_factor   Reducción de la imagen. 1.01 (1%=lento), 1.1 (10%=normal), 1.5 (50%=rápido)")
    print("   min_neighbors  Cuantas veces se debe detectar. 1 (ruidoso), 5 (confiable), 20 (seguro)")
    sys.exit()

# parametros de entrada
input_filename = sys.argv[1]
input_xml = sys.argv[2]
input_scale_factor = float(sys.argv[3])
input_min_neighbors = int(sys.argv[4])

ejemplo3(input_filename, input_xml, input_scale_factor, input_min_neighbors)
