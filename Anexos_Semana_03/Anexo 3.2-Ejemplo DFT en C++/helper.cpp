// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 23 de marzo de 2025
#include "helper.hpp"
#include <iostream>
#include <sys/stat.h>
#include <opencv2/core.hpp>

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

bool is_int(const std::string &string) {
	if (string.length() == 0)
		return false;
	for (char c : string) {
		if (!std::isdigit(c))
			return false;
	}
	return true;
}

std::vector<std::string> get_args_vector(int argc, char **argv) {
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i) {
		args.push_back(std::string(argv[i]));
	}
	return args;
}

bool starts_with(const std::string &string, const std::string &prefix) {
	if (string.length() < prefix.length())
		return false;
	return (0 == string.compare(0, prefix.length(), prefix));
}

bool ends_with(const std::string &string, const std::string &suffix) {
	if (string.length() < suffix.length())
		return false;
	return (0 == string.compare(string.length() - suffix.length(), suffix.length(), suffix));
}

cv::VideoCapture abrir_video(const std::string &filename) {
	cv::VideoCapture capture;
	if (is_int(filename)) {
		int id_webcam = parse_int(filename);
		std::cout << "abriendo camara " << id_webcam << std::endl;
		if (capture.open(id_webcam)) {
			//leer el primer frame
			cv::Mat frame;
			capture.read(frame);
			std::cout << "video " << frame.size << std::endl;
		}
	} else {
		std::cout << "abriendo archivo " << filename << std::endl;
		capture.open(filename);
	}
	if (!capture.isOpened()) {
		std::cout << "no puedo abrir video " << filename << std::endl;
		throw std::runtime_error("no puedo abrir video");
	}
	std::cout << filename << ": " << capture.get(cv::CAP_PROP_FRAME_WIDTH) << "x"
			<< capture.get(cv::CAP_PROP_FRAME_HEIGHT) << " fps=" << capture.get(cv::CAP_PROP_FPS) << std::endl;
	return capture;
}
