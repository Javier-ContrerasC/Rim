// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 23 de marzo de 2025
#pragma once

#include <string>
#include <vector>
#include <opencv2/videoio.hpp>

int parse_int(const std::string &string);

bool is_int(const std::string &string);

std::vector<std::string> get_args_vector(int argc, char **argv);

bool starts_with(const std::string &string, const std::string &prefix);

bool ends_with(const std::string &string, const std::string &suffix);

cv::VideoCapture abrir_video(const std::string &filename);
