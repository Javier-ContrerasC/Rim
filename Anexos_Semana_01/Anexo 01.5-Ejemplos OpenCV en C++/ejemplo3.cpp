// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 10 de marzo de 2025

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

int main(int argc, char **argv) {
	if (argc < 5) {
		std::cout << "Uso:" << std::endl;
		std::cout << " " << argv[0] << std::endl;
		std::cout << "   id_webcam      Webcam. Ej: 0, 1, 2 " << std::endl;
		std::cout << "   XML_detector   Archivo detector. Ej: haarcascade_frontalface_default.xml" << std::endl;
		std::cout << "   scale_factor   Reducción de la imagen. 1.01 (1%=lento), 1.1 (10%=normal), 1.5 (50%=rápido)"
				<< std::endl;
		std::cout << "   min_neighbors  Cuantas veces se debe detectar. 1 (ruidoso), 5 (confiable), 20 (seguro)"
				<< std::endl;
		return 1;
	}

	//parametros de entrada
	int id_webcam = std::atoi(argv[1]);
	std::string xml = argv[2];
	double scale_factor = std::atof(argv[3]);
	int min_neighbors = std::atoi(argv[4]);

	//cargar el detector
	std::string file_xml = cv::samples::findFile(xml);
	std::cout << "cargando " << file_xml << std::endl;
	cv::CascadeClassifier detector;
	if (!detector.load(xml)) {
		std::cout << "error leyendo " << xml << std::endl;
		return 1;
	}
	std::cout << "OK " << xml << std::endl;

	//abrir webcam
	std::cout << "abriendo webcam " << id_webcam << std::endl;
	cv::VideoCapture capture;
	capture.open(id_webcam);
	if (!capture.isOpened()) {
		std::cout << "error abriendo " << id_webcam << std::endl;
		return 1;
	}
	std::cout << "OK webcam " << id_webcam << std::endl;

	//procesar frames
	std::string window_name = "webcam " + std::to_string(id_webcam);
	cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
	cv::Mat frame_color, frame_gris;
	while (capture.grab()) {
		if (!capture.retrieve(frame_color))
			break;
		cv::cvtColor(frame_color, frame_gris, cv::COLOR_BGR2GRAY);
		std::vector<cv::Rect> detecciones;
		detector.detectMultiScale(frame_gris, detecciones, scale_factor, min_neighbors);
		std::cout << detecciones.size() << " detecciones" << std::endl;
		for (const cv::Rect &deteccion : detecciones) {
			cv::Point center(deteccion.x + deteccion.width / 2, deteccion.y + deteccion.height / 2);
			cv::Size size(deteccion.width / 2, deteccion.height / 2);
			cv::Scalar color(255, 255, 0);
			int thickness = 3;
			cv::ellipse(frame_color, center, size, 0, 0, 360, color, thickness);
		}
		cv::imshow(window_name, frame_color);
		char c = cv::waitKey(1);
		if (c == 27 || c == 'q') //tecla ESC o q
			break;
	}
	return 0;
}
