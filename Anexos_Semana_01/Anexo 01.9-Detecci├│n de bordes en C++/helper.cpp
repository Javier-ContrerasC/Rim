// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 10 de marzo de 2025

#include "helper.hpp"
#include <iostream>
#include <sys/stat.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QApplication>
#include <QFileDialog>

//un namespace anónimo mantiene funciones visibles solo dentro de este archivo
namespace {

bool is_int(const std::string &string) {
	if (string.length() == 0)
		return false;
	for (char c : string) {
		if (!std::isdigit(c))
			return false;
	}
	return true;
}
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

}

std::vector<std::string> get_args_vector(int argc, char **argv) {
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i) {
		args.push_back(std::string(argv[i]));
	}
	return args;
}

std::string ui_select_video() {
	int argc = 0;
	char **argv = 0;
	QApplication app(argc, argv);
	QFileDialog::Options options;
	QString selectedFilter;
	QString filename = QFileDialog::getOpenFileName(nullptr, QString("Elegir Video"), ".",
			QString("Videos (*.mpg *.mp4 *.avi)"), &selectedFilter, options);
	return filename.toStdString();
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

void mostrar_imagen(const std::string &window_name, const cv::Mat &imagen, bool valorAbsoluto, bool escalarMin0Max255) {
	cv::Mat imagen_abs;
	if (valorAbsoluto) {
		imagen_abs = cv::abs(imagen);
	} else {
		imagen_abs = imagen;
	}
	cv::Mat imagen_norm;
	if (escalarMin0Max255) {
		cv::normalize(imagen_abs, imagen_norm, 0, 255, cv::NORM_MINMAX, CV_8U);
	} else {
		imagen_norm = imagen_abs;
	}
	cv::imshow(window_name, imagen_norm);
}

