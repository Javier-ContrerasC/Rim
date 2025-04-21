// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 15 de abril de 2025

#include "helper.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int parse_int(const std::string &string) {
	std::size_t next_position = 0;
	int n;
	try {
		n = std::stoi(string, &next_position, 10);
	} catch (...) {
		throw std::logic_error("invalid number \"" + string + "\"");
	}
	if (next_position != string.length())
		throw std::logic_error("invalid number \"" + string + "\"");
	return n;
}

double parse_double(const std::string &string) {
	std::size_t next_position = 0;
	double n;
	try {
		n = std::stod(string, &next_position);
	} catch (...) {
		throw std::logic_error("invalid number \"" + string + "\"");
	}
	if (next_position != string.length())
		throw std::logic_error("invalid number \"" + string + "\"");
	return n;
}

bool is_int(const std::string &string) {
	if (string.length() == 0)
		return false;
	for (char c : string) {
		if (!std::isdigit(c))
			return false;
	}
	return true;
}

std::vector<std::string> split(const std::string &string, char separador) {
	std::vector<std::string> v;
	std::size_t from = 0, pos;
	while ((pos = string.find(separador, from)) != std::string::npos) {
		v.push_back(string.substr(from, (pos - from)));
		from = pos + 1;
	}
	if (from < string.length())
		v.push_back(string.substr(from));
	return v;
}

std::vector<std::string> get_args_vector(int argc, char **argv) {
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i) {
		args.push_back(std::string(argv[i]));
	}
	return args;
}
std::string filename_sin_extension(const std::string &filename) {
	std::size_t posStart = filename.find_last_of("/\\") + 1;
	std::size_t posDot = filename.find_last_of(".");
	return (posDot <= 0 || posDot <= posStart) ? filename : filename.substr(0, posDot);
}
std::string filename_extension(const std::string &filename) {
	std::size_t posStart = filename.find_last_of("/\\") + 1;
	std::size_t posDot = filename.find_last_of(".");
	return (posDot <= 0 || posDot <= posStart) ? "" : filename.substr(posDot + 1);
}

bool existe_archivo(const std::string &filename) {
	struct stat st;
	if (stat(filename.c_str(), &st) == 0)
		return S_ISREG(st.st_mode) ? true : false;
	return false;
}

void agregar_texto(cv::Mat &imagen, int center_x, int center_y, const std::string &texto) {
	static int fontFace = cv::FONT_HERSHEY_PLAIN;
	static double fontScale = 1;
	static int thickness = 1;
	int baseline = 0;
	cv::Size textSize = cv::getTextSize(texto, fontFace, fontScale, thickness, &baseline);
	cv::Point p(center_x - textSize.width / 2, center_y + textSize.height);
	cv::Rect rect(p.x - 1, p.y - textSize.height - 1, textSize.width + 2, textSize.height + 2);
	cv::rectangle(imagen, rect, cv::Scalar::all(0), -1);
	cv::putText(imagen, texto, p, fontFace, fontScale, cv::Scalar::all(255), thickness);
}
