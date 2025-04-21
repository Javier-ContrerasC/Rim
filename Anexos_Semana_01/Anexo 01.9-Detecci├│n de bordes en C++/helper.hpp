// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 10 de marzo de 2025

#pragma once

#include <string>
#include <vector>
#include <opencv2/videoio.hpp>

std::vector<std::string> get_args_vector(int argc, char **argv);

std::string ui_select_video();

cv::VideoCapture abrir_video(const std::string &filename);

void agregar_texto(cv::Mat &imagen, int center_x, int center_y, const std::string &texto);

void mostrar_imagen(const std::string &window_name, const cv::Mat &imagen, bool valorAbsoluto, bool escalarMin0Max255);
