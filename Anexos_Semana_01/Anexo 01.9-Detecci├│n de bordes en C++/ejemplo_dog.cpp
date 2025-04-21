// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 10 de marzo de 2025

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "helper.hpp"

int sigma1 = 5;
int sigma2 = 13;
double threshold = 10;
double delta = 0.1;

void ejemplo(const std::string &filename) {
	cv::VideoCapture capture = abrir_video(filename);
	cv::Mat frame, frame_gris, blur1, blur2, frame_diff, frame_noneg, frame_bin;
	while (capture.grab()) {
		if (!capture.retrieve(frame))
			continue;
		//convertir a gris
		cv::cvtColor(frame, frame_gris, cv::COLOR_BGR2GRAY);
		mostrar_imagen("VIDEO", frame_gris, false, false);
		//calcular DoG
		cv::GaussianBlur(frame_gris, blur1, cv::Size(sigma1, sigma1), 0, 0);
		cv::GaussianBlur(frame_gris, blur2, cv::Size(sigma2, sigma2), 0, 0);
		cv::subtract(blur1, blur2, frame_diff, cv::noArray(), CV_32F);
		mostrar_imagen("Diff", frame_diff, false, true);
		cv::threshold(frame_diff, frame_bin, threshold, 255, cv::THRESH_BINARY);
		agregar_texto(frame_bin, frame_bin.cols / 2, 2, "TH=" + std::to_string(threshold));
		mostrar_imagen("BIN", frame_bin, false, true);
		//esperar por una tecla
		char key = cv::waitKey(1) & 0xFF;
		if (key == ' ')
			key = cv::waitKey(0) & 0xFF;
		if (key == 'q' or key == 27) {
			break;
		} else if (key == 'a') {
			threshold += delta;
		} else if (key == 'z') {
			if (threshold - delta > 0)
				threshold -= delta;
		}
	}
	capture.release();
	cv::destroyAllWindows();
}

int main(int argc, char **argv) {
	try {
		std::vector<std::string> args = get_args_vector(argc, argv);
		std::cout << "CC5213 - Recuperación de Información Multimedia" << std::endl;
		std::cout << "Ejemplo Difference of Gaussians" << std::endl;
		std::cout << "Uso: " << args[0] << " [video_filename | webcam_id]" << std::endl;
		std::string filename = "";
		if (args.size() < 2) {
			filename = ui_select_video();
		} else {
			filename = args[1];
		}
		ejemplo(filename);
	} catch (const std::exception &ex) {
		std::cout << "Ha ocurrido un ERROR: " << ex.what() << std::endl;
	} catch (...) {
		std::cout << "Ha ocurrido ERROR desconocido" << std::endl;
	}
	return 0;
}
