// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 15 de abril de 2025

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <opencv2/core.hpp>

int parse_int(const std::string &string);

double parse_double(const std::string &string);

bool is_int(const std::string &string);

std::vector<std::string> split(const std::string &string, char separador);

std::vector<std::string> get_args_vector(int argc, char **argv);

std::string filename_sin_extension(const std::string &filename);

std::string filename_extension(const std::string &filename);

bool existe_archivo(const std::string &filename);

void agregar_texto(cv::Mat &imagen, int center_x, int center_y, const std::string &texto);

template<typename T>
std::vector<T> leer_bytes_archivo(const std::string &filename) {
	//abrir el archivo
	std::ifstream infile;
	infile.open(filename);
	if (!infile.is_open())
		throw std::runtime_error("no puedo abrir " + filename);
	//ir al final
	infile.seekg(0, std::ios::end);
	//obtener la posicion en bytes
	int numBytes = infile.tellg();
	//volver al inicio
	infile.seekg(0, std::ios::beg);
	//leer todos los bytes del archivo
	if ((numBytes % sizeof(T)) != 0)
		throw std::runtime_error("no cuadran los bytes");
	std::vector<T> vector;
	vector.resize(numBytes / sizeof(T));
	infile.read(reinterpret_cast<char*>(vector.data()), numBytes);
	if (infile.bad())
		throw std::runtime_error("error leyendo " + filename);
	return vector;
}
