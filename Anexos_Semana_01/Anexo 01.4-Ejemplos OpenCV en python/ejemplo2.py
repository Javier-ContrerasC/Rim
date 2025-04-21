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


def ejemplo2(filename):
    capture = open_video(filename)
    # nombres de las ventanas que van a mostrar los frames
    window_name1 = filename + " (video)"
    window_name2 = filename + " (bin)"
    cv2.namedWindow(window_name1, cv2.WINDOW_AUTOSIZE)
    cv2.namedWindow(window_name2, cv2.WINDOW_AUTOSIZE)
    # ciclo que obiene un frame y lo muestra
    while capture.grab():
        retval, frame_color = capture.retrieve()
        if not retval:
            continue
        frame_gris = cv2.cvtColor(frame_color, cv2.COLOR_BGR2GRAY)
        thresh, frame_bin = cv2.threshold(frame_gris, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
        print("frame_size={} binary_threshold={}".format(frame_bin.shape, thresh))
        cv2.imshow(window_name1, frame_gris)
        cv2.imshow(window_name2, frame_bin)
        key = cv2.waitKey(33)
        if key == ord('q') or key == 27:
            break
    capture.release()
    cv2.destroyAllWindows()


if len(sys.argv) <= 1:
    print("uso: {} [video.mp4 | id webcam 0,1,2]".format(sys.argv[0]))
    sys.exit()

input_filename = sys.argv[1]

ejemplo2(input_filename)
