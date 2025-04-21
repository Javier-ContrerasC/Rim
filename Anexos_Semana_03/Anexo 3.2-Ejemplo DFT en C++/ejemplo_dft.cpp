// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 23 de marzo de 2025

// EJEMPLO DE CALCULO DE TRANSFORMADA DE FOURIER
// Teclas:
//   [a,w,s,d] -> mover rectangulo esquina top-left
//   [g,y,h,j] -> mover rectangulo esquina bottom-right
//   [i,k] -> aumentar peso parte real
//   [o,l] -> aumentar peso parte imaginaria
//   [p]   -> intercambiar pesos real<->imaginaria
// Ver ejemplo DFT en: https://docs.opencv.org/4.10.0/d8/d01/tutorial_discrete_fourier_transform.html

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "helper.hpp"

namespace {

int rect_x = 10, rect_y = 10;
int rect_w = 1, rect_h = 1;
int sum_real = 0, sum_imag = 0;

int delta_position = 1;
int delta_sum = 10000000;

bool modificar_param(char key, char key_bajar, char key_subir, int &parametro, int delta) {
	if (key == key_bajar) {
		if (parametro - delta >= 0)
			parametro -= delta;
		return true;
	} else if (key == key_subir) {
		parametro += delta;
		return true;
	}
	return false;
}

void usar_key(char key) {
	bool changed = false;
	changed |= modificar_param(key, 'a', 'd', rect_x, delta_position);
	changed |= modificar_param(key, 'w', 's', rect_y, delta_position);
	changed |= modificar_param(key, 'g', 'j', rect_w, delta_position);
	changed |= modificar_param(key, 'y', 'h', rect_h, delta_position);
	changed |= modificar_param(key, 'k', 'i', sum_real, delta_sum);
	changed |= modificar_param(key, 'l', 'o', sum_imag, delta_sum);
	if (key == 'p') {
		double swap = sum_real;
		sum_real = sum_imag;
		sum_imag = swap;
		changed = true;
	}
	if (changed) {
		std::cout << "pos=[" << rect_x << ", " << rect_y << "] size=[" << rect_w << ", " << rect_h << "] sum=("
				<< (sum_real > 0 ? "+" : "") << sum_real << "," << (sum_imag > 0 ? "+" : "") << sum_imag << ")"
				<< std::endl;
	}
}

cv::Mat imagen_gaussiana(int w, int h, int sigma) {
	std::cout << "gaussiana sigma=" << sigma << std::endl;
	cv::Mat imag;
	imag.create(h, w, CV_32FC1);
	imag = 0;
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			double d = std::sqrt((h / 2 - i) * (h / 2 - i) + (w / 2 - j) * (w / 2 - j));
			imag.at<float>(i, j) = (float) std::exp(-d / sigma);
		}
	}
	double max;
	cv::minMaxIdx(imag, NULL, &max);
	cv::Mat scaled;
	cv::convertScaleAbs(imag, scaled, 255.0 / max, 0);
	return scaled;
}

#define PI     3.14159265358979323846

cv::Mat imagen_coseno(int w, int h, int ciclosW, int ciclosH) {
	std::cout << "coseno ciclos=" << ciclosW << "x" << ciclosH << std::endl;
	cv::Mat imag;
	imag.create(h, w, CV_32FC1);
	imag = 0;
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			double a = (j / (double) (w-1)) * ciclosW;
			double b = (i / (double) (h-1)) * ciclosH;
			// coseno entrega valores en [-1, 1], se escala a [0, 1]
			imag.at<float>(i, j) = (float) (std::cos(2 * PI * (a + b)) + 1) / 2.0;
		}
	}
	cv::Mat scaled;
	cv::convertScaleAbs(imag, scaled, 255.0, 0);
	return scaled;
}

void modificar_frecuencias(cv::Mat &frec_complex) {
	if (sum_real == 0 && sum_imag == 0)
		return;
	//ciclo sobre un rango de frecuencias
	for (int i = rect_y; i < rect_y + rect_h; ++i) {
		for (int j = rect_x; j < rect_x + rect_w; ++j) {
			//obtener el peso de esa frecuencia
			cv::Vec2f complex_value = frec_complex.at<cv::Vec2f>(i, j);
			//modificar el peso
			complex_value[0] += sum_real;
			complex_value[1] += sum_imag;
			//actualizar el peso de esa frecuencia
			frec_complex.at<cv::Vec2f>(i, j) = complex_value;
		}
	}
}

cv::Mat imagen_to_complex(const cv::Mat &gris) {
	//convertir a float
	cv::Mat real;
	gris.convertTo(real, CV_32F);
	//la parte imaginaria son ceros
	cv::Mat imag;
	imag.create(gris.size(), CV_32FC1);
	imag = 0;
	//unir parte real e imaginaria
	cv::Mat planos[] = { real, imag };
	cv::Mat complex;
	cv::merge(planos, 2, complex);
	return complex;
}

double global_max = 20;

void visualizar_magnitud(const cv::Mat &frec_complex, cv::Mat &imageLogMagnitud) {
	//separar complejos en parte real e imaginaria
	cv::Mat frec_real;
	cv::Mat frec_imag;
	frec_real.create(frec_complex.size(), CV_32FC1);
	frec_imag.create(frec_complex.size(), CV_32FC1);
	cv::Mat frec_planos[] = { frec_real, frec_imag };
	cv::split(frec_complex, frec_planos);
	//calcular la magnitud
	cv::Mat magnitud;
	cv::magnitude(frec_real, frec_imag, magnitud);
	//calcular log magnitud para visualizar
	cv::log(magnitud + 1, magnitud);
	double max;
	cv::minMaxIdx(magnitud, NULL, &max);
	if (max > global_max) {
		global_max = std::max(max, global_max);
		std::cout << " max=" << global_max << std::endl;
	}
	cv::convertScaleAbs(magnitud, imageLogMagnitud, 255.0 / max, 0);
}

void visualizar_centrado_color(const cv::Mat &imageLogMagnitud, cv::Mat &imageCenterColor) {
	//centrar
	int w = imageLogMagnitud.cols;
	int h = imageLogMagnitud.rows;
	int w2 = w / 2;
	int h2 = h / 2;
	cv::Rect r00(0, 0, w2, h2);
	cv::Rect r01(w - w2, 0, w2, h2);
	cv::Rect r10(0, h - h2, w2, h2);
	cv::Rect r11(w - w2, h - h2, w2, h2);
	cv::Mat imageCenter(h, w, imageLogMagnitud.type());
	imageLogMagnitud(r00).copyTo(imageCenter(r11));
	imageLogMagnitud(r01).copyTo(imageCenter(r10));
	imageLogMagnitud(r10).copyTo(imageCenter(r01));
	imageLogMagnitud(r11).copyTo(imageCenter(r00));
	//ver colormaps: https://docs.opencv.org/4.4.0/d3/d50/group__imgproc__colormap.html
	cv::applyColorMap(imageCenter, imageCenterColor, cv::ColormapTypes::COLORMAP_JET);
}

void ejemplo_imagenes(const std::vector<std::string> &filenames) {
	int i = 1;
	for (const std::string &filename : filenames) {
		std::string num = std::to_string(i++);
		cv::Mat image_gris;
		if (starts_with(filename, "GAUSS_")) {
			int sigma = parse_int(filename.substr(6));
			image_gris = imagen_gaussiana(500, 500, sigma);
		} else if (starts_with(filename, "COS_")) {
			std::string ciclos = filename.substr(4);
			int posicionX = ciclos.find('x');
			int ciclosW = parse_int(ciclos.substr(0, posicionX));
			int ciclosH = parse_int(ciclos.substr(posicionX + 1));
			image_gris = imagen_coseno(500, 500, ciclosW, ciclosH);
		} else {
			image_gris = cv::imread(filename, cv::IMREAD_GRAYSCALE);
		}
		if (image_gris.empty())
			continue;
		cv::imshow("Imagen-" + num, image_gris);
		//convertir la imagen en tipo complejo (2 canales, parte imaginaria=0)
		cv::Mat frame_complex = imagen_to_complex(image_gris);
		//calcular la DFT de la imagen
		cv::Mat frecuencias_complex;
		cv::dft(frame_complex, frecuencias_complex, cv::DFT_COMPLEX_OUTPUT);
		cv::Mat imageLogMagnitud;
		visualizar_magnitud(frecuencias_complex, imageLogMagnitud);
		cv::imshow("LogMagnitud-" + num, imageLogMagnitud);
		cv::Mat imageCenterColor;
		visualizar_centrado_color(imageLogMagnitud, imageCenterColor);
		cv::imshow("CenterColor-" + num, imageCenterColor);
	}
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void ejemplo_video(const std::string &filename) {
	std::cout << "  Teclas:" << std::endl;
	std::cout << "    [a,w,s,d] -> mover rectangulo tl" << std::endl;
	std::cout << "    [g,y,h,j] -> mover rectangulo br" << std::endl;
	std::cout << "    [i,k] -> aumentar peso parte real" << std::endl;
	std::cout << "    [o,l] -> aumentar peso parte imaginaria" << std::endl;
	std::cout << "    [p]   -> intercambiar pesos real<->imaginaria" << std::endl;
	cv::VideoCapture capture = abrir_video(filename);
	cv::Mat frame, frame_gris, output_frame, output_frame_gris;
	cv::Mat imageLogMagnitud, imageCenterColor;
	char key = ' ';
	while (capture.grab()) {
		if (!capture.retrieve(frame))
			continue;
		//convertir a gris
		cv::cvtColor(frame, frame_gris, cv::COLOR_BGR2GRAY);
		cv::imshow("VIDEO", frame_gris);
		//convertir la imagen en tipo complejo (2 canales, parte imaginaria=0)
		cv::Mat frame_complex = imagen_to_complex(frame_gris);
		//calcular la DFT de la imagen
		cv::Mat frecuencias_complex;
		cv::dft(frame_complex, frecuencias_complex, cv::DFT_COMPLEX_OUTPUT);
		//modificar las frecuencias de la DFT
		modificar_frecuencias(frecuencias_complex);
		//visualizar las magnitudes
		visualizar_magnitud(frecuencias_complex, imageLogMagnitud);
		cv::imshow("LogMagnitud", imageLogMagnitud);
		visualizar_centrado_color(imageLogMagnitud, imageCenterColor);
		cv::imshow("CenterColor", imageCenterColor);
		//invertir la DFT
		cv::idft(frecuencias_complex, output_frame, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
		//mostrar la imagen resultante
		output_frame.convertTo(output_frame_gris, CV_8U);
		cv::imshow("OUTPUT", output_frame_gris);
		//esperar por una tecla
		key = cv::waitKey(1) & 0xFF;
		if (key == ' ')
			key = cv::waitKey(0) & 0xFF;
		if (key == 'q' || key == 27)
			break;
		usar_key(key);
	}
	//pausar al final del video si no se presiona salir
	if (key != 'q' && key != 27)
		cv::waitKey(0);
	capture.release();
	cv::destroyAllWindows();
}

}

int main(int argc, char **argv) {
	try {
		std::vector<std::string> args = get_args_vector(argc, argv);
		std::cout << "CC5213 - Recuperación de Información Multimedia" << std::endl;
		std::cout << "Ejemplo DFT" << std::endl;
		std::cout << " Usos: " << args[0] << " id_webcam                Ej: " << args[0] << " 0" << std::endl;
		std::cout << "       " << args[0] << " video_filename           Ej: " << args[0] << " video.mp4" << std::endl;
		std::cout << "       " << args[0] << " images_filenames...      Ej: " << args[0] << " imagen1.jpg imagen2.jpg"
				<< std::endl;
		std::cout << "       " << args[0] << " GAUSS_[sigma]            Ej: " << args[0] << " GAUSS_10" << std::endl;
		std::cout << "       " << args[0] << " COS_[ciclosW]x[ciclosH]  Ej: " << args[0] << " COS_10x20" << std::endl;
		std::vector<std::string> filenames;
		filenames.insert(filenames.end(), args.begin() + 1, args.end());
		if (filenames.empty())
			return 0;
		if (filenames.size() == 1 && (ends_with(filenames[0], ".mp4") || is_int(filenames[0])))
			ejemplo_video(filenames[0]);
		else
			ejemplo_imagenes(filenames);
	} catch (const std::exception &ex) {
		std::cout << "Ha ocurrido un ERROR: " << ex.what() << std::endl;
	} catch (...) {
		std::cout << "Ha ocurrido ERROR desconocido" << std::endl;
	}
	return 0;
}
