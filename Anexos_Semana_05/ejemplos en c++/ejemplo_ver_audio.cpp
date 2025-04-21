#include <fftw3.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Mfcc.hpp"
#include "helper.hpp"

// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 15 de abril de 2025

// EJEMPLO AUDIO

namespace {

std::string filename_salida(const std::string &file_input, int sample_rate) {
	std::stringstream ss;
	ss << filename_sin_extension(file_input) << "." << sample_rate << ".raw";
	return ss.str();
}
void comando_ffmpeg(const std::string &file_input, int sample_rate, const std::string &file_output) {
	//s16le: Signed 16 bits little-endian
	//ver https://trac.ffmpeg.org/wiki/audio%20types
	std::stringstream ss;
	ss << "ffmpeg";
	ss << " -loglevel warning";
	// sobre escribir
	ss << " -y";
	// video de entrada
	ss << " -i  \"" << file_input << "\"";
	// salir audio 1 canal
	ss << " -ac 1";
	// ignorar cualquier pista de video
	ss << " -vn";
	// ignorar cualquier pista de subtitulos
	ss << " -sn";
	// audio sample rate
	ss << " -ar " << sample_rate;
	// formato del archivo de salida
	ss << " -f s16le";
	// codec de audio de salida
	ss << " -acodec pcm_s16le";
	// archivo a generar
	ss << " \"" << file_output << "\"";
	std::string command = ss.str();
	// system
	std::cout << "Ejecutando:" << std::endl;
	std::cout << "   " << command << std::endl;
	int ret = std::system(command.c_str());
	if (ret != 0)
		throw std::runtime_error("error al ejecutar: " + command);
}

std::vector<short> load_audio_samples(const std::string &file_input, int sample_rate) {
	if (filename_extension(file_input) == "raw") {
		return leer_bytes_archivo<short>(file_input);
	}
	//nombre del archivo de salida con el audio raw
	std::string file_output = filename_salida(file_input, sample_rate);
	//si ya existe ese archivo, retornarlo
	if (!existe_archivo(file_output)) {
		//ejecutar FFMPEG para decodificar el audio y guardar en formato raw
		comando_ffmpeg(file_input, sample_rate, file_output);
	}
	//como se usa s16le se debe cargar en un vector de short
	std::vector<short> audio_samples = leer_bytes_archivo<short>(file_output);
	return audio_samples;
}

void dibujarVector(cv::Mat &image, int height, int width_max, const std::vector<double> &vector_values,
		const cv::Scalar &color_background, const cv::Scalar &color_foreground, double max_value) {
	//size de la imagen
	int vector_size = vector_values.size();
	int stepCol = 1, stepDim = 1, width = vector_size;
	if (vector_size < width_max) {
		stepCol = std::floor(width_max / (double) vector_size);
		width = vector_size * stepCol;
	} else if (vector_size > width_max) {
		stepDim = std::ceil(vector_size / (double) width_max);
		width = std::ceil(vector_size / (double) stepDim);
	}
	//limpiar la imagen
	image.create(height, width, CV_8UC3);
	cv::rectangle(image, cv::Point(0, 0), cv::Point(image.cols, image.rows), color_background, cv::FILLED);
	//linea central
	int linea_base = image.rows / 2 - 1;
	cv::line(image, cv::Point(0, linea_base), cv::Point(image.cols, linea_base), cv::Scalar(0, 0, 0), 1);
	int alto_max = image.rows / 2 - 1;
	//ciclo por columna de la imagen
	cv::Point prev(-1, -1), current;
	for (int col = 0, dim = 0; col < width && dim < vector_size; col += stepCol, dim += stepDim) {
		//obtener el valor en proporcion del maximo
		double val = vector_values.at(dim) / (max_value == 0 ? 1 : max_value);
		//escalar el valor al alto de la imagen
		int alto = std::round(val * alto_max);
		//coordenadas del punto
		current.x = col;
		current.y = std::max(std::min(linea_base - alto, image.rows - 1), 0);
		//dibujar la linea
		if (prev.x >= 0 && current.x < image.cols)
			cv::line(image, prev, current, color_foreground, 1);
		prev = current;

	}
	std::string txt = "mostrando " + std::to_string(vector_size) + " valores (" + std::to_string(stepCol) + " pix son "
			+ std::to_string(stepDim) + " vals)";
	agregar_texto(image, width / 2, height - 40, txt);
}

void drawSpectogram(cv::Mat &image_gray, cv::Mat &image_color, int height, int width_max,
		const std::vector<double> &vector_values, double max_value, int position, bool clean_image) {
	//size de la imagen
	int vector_size = vector_values.size();
	int stepDim = 1, width = vector_size;
	if (vector_size > width_max) {
		stepDim = std::ceil(vector_size / (double) width_max);
		width = std::ceil(vector_size / (double) stepDim);
	}
	//limpiar la imagen
	if (clean_image) {
		image_gray.create(height, width, CV_8UC1);
		cv::rectangle(image_gray, cv::Point(0, 0), cv::Point(image_gray.cols, image_gray.rows), cv::Scalar::all(0),
				cv::FILLED);
	}
	for (int col = 0, dim = 0; col < width && dim < vector_size; col += 1, dim += stepDim) {
		//obtener el valor en proporcion del maximo
		double val = vector_values.at(dim) / (max_value == 0 ? 1 : max_value);
		//obtener el valor gris en proporcion del maximo
		int gray = std::round(val * 255);
		//asegurarse que esta en el rango de 8 bits
		gray = std::max(0, std::min(255, gray));
		//coordenadas del punto
		image_gray.at<uchar>(position, col) = gray;
	}
	//convertir a colores
	cv::applyColorMap(image_gray, image_color, cv::COLORMAP_JET);
}

template<typename T>
void setMaxValue(double &max_value, const std::vector<T> &values) {
	for (T value : values) {
		double d = std::fabs(value);
		if (d > max_value)
			max_value = d;
	}
}

static double global_max_sample = 0;
static double global_max_energy = 0;
static bool global_show_samples = true, global_show_espectro = true, global_show_espectrograma = true,
		global_show_mfcc = true;

void mostrar_samples(const std::vector<double> &ventana_samples) {
	if (!global_show_samples) {
		cv::destroyWindow("samples");
		return;
	}
	static cv::Scalar color_background = cv::Scalar(0, 127, 127);
	static cv::Scalar color_foreground = cv::Scalar(255, 255, 127);
	static cv::Mat image;
	dibujarVector(image, 400, 1025, ventana_samples, color_background, color_foreground, global_max_sample);
	cv::imshow("samples", image);
}

void mostrar_spectrum(const std::vector<double> &energias_espectro) {
	if (!global_show_espectro) {
		cv::destroyWindow("spectrum");
		return;
	}
	static cv::Scalar color_background = cv::Scalar(0, 127, 0);
	static cv::Scalar color_foreground = cv::Scalar(0, 255, 255);
	static cv::Mat image;
	dibujarVector(image, 400, 1025, energias_espectro, color_background, color_foreground, global_max_energy);
	cv::imshow("spectrum", image);
}

static int global_spectogram_position = 0;
static bool global_spectogram_clean = true;

void mostrar_spectrogram(const std::vector<double> &energias_espectro) {
	if (!global_show_espectrograma) {
		cv::destroyWindow("spectogram");
		return;
	}
	static cv::Mat image, image_color;
	drawSpectogram(image, image_color, 300, 1025, energias_espectro, global_max_energy, global_spectogram_position,
			global_spectogram_clean);
	global_spectogram_position = (global_spectogram_position + 1) % image.rows;
	global_spectogram_clean = false;
	cv::imshow("spectogram", image_color);
}

void mostrar_mfcc(const std::vector<double> &energias_espectro, Mfcc &mfcc) {
	if (!global_show_mfcc) {
		cv::destroyWindow("mfcc");
		return;
	}
	//calcular mfcc
	std::vector<double> mfcc_descriptor;
	mfcc.getCoefficients(energias_espectro, mfcc_descriptor);
	//no se conocen a priori estos maximos, se iran escalando de a poco
	static double global_max_mfcc = 0;
	setMaxValue(global_max_mfcc, mfcc_descriptor);
	static cv::Scalar color_background = cv::Scalar(127, 127, 127);
	static cv::Scalar color_foreground = cv::Scalar(255, 255, 255);
	cv::Mat image_mfcc;
	dibujarVector(image_mfcc, 300, 300, mfcc_descriptor, color_background, color_foreground, global_max_mfcc);
	//printVector("mfcc", mfcc_descriptor);
	cv::imshow("mfcc", image_mfcc);
}

void ejemplo(const std::vector<short> &audio_samples, int sample_rate) {
	//procesar el audio por ventanas
	int ventana_size = sample_rate;
	int desplazamiento = sample_rate / 4;
	//imprimir tamaños en milisegundos
	int audio_ms = std::round((1000.0 * audio_samples.size()) / sample_rate);
	int ventana_ms = std::round((1000.0 * ventana_size) / sample_rate);
	int despl_ms = std::round((1000.0 * desplazamiento) / sample_rate);
	///valor del maximo para normalizar la visualizacion
	setMaxValue(global_max_sample, audio_samples);
	//preparar la FT
	//ventana de samples a analizar
	std::vector<double> ventana(ventana_size, 0);
	//el numero de coeficientes calculados depende del tamaño de ventana
	//según la documentación de FFTW requiere la mitad más uno. Ver: https://www.fftw.org/fftw3_doc/Real_002ddata-DFT-Array-Format.html
	int num_coeficientes_ft = ventana_size / 2 + 1;
	//espacio para los complejos de salida, cada fftw_malloc debe tener un fftw_free
	fftw_complex *coeficientes_ft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_coeficientes_ft);
	//preparar la FT. r2c=real to complex. recibe arrays de entrada y salida
	fftw_plan plan_ft = fftw_plan_dft_r2c_1d(ventana_size, ventana.data(), coeficientes_ft, FFTW_MEASURE);
	//vector para guardar el espectro (magnitud de los complejos)
	std::vector<double> espectro(num_coeficientes_ft, 0);
	bool use_log_energy = false;
	//libreria que calcula MFCC a partir del espectro
	static const int mfcc_filters = 48;	// cantidad de filtros para MFCC
	static const int mfcc_coeffs = 48;	// cantidad de coeficientes MFCC a calcular
	Mfcc mfcc;
	mfcc.init(sample_rate, mfcc_filters, num_coeficientes_ft, mfcc_coeffs);
	//escribir
	std::cout << "audio largo=" << audio_samples.size() << " samples, " << audio_ms << " ms. samplerate=" << sample_rate
			<< std::endl;
	std::cout << "ventana largo=" << ventana_size << " samples, " << ventana_ms << " ms.  desplazamiento= "
			<< desplazamiento << " samples, " << despl_ms << " ms." << std::endl;
	std::cout << "coeficientes_ft=" << num_coeficientes_ft << " coeficientes_mfcc=" << mfcc_coeffs << std::endl;
	//ciclo por ventana
	for (int start = 0; start + ventana_size < (int) audio_samples.size(); start += desplazamiento) {
		int ventana_inicio_ms = std::round((1000.0 * start) / sample_rate);
		int ventana_fin_ms = std::round((1000.0 * (start + ventana_size)) / sample_rate);
		std::cout << "ventana " << start << " [" << ventana_inicio_ms << "ms - " << ventana_fin_ms << "ms]"
				<< std::endl;
		//copiar los valores a la ventana
		for (int i = 0; i < ventana_size; ++i) {
			//opcional: multiplicar la ventana por una funcion que suavice los bordes: Hann
			ventana[i] = audio_samples[start + i];
		}
		//calcular la FT
		fftw_execute(plan_ft);
		//recorrer el resultado y calcular el espectro
		for (int i = 0; i < num_coeficientes_ft; i++) {
			//calcular la magnitud de cada frecuencia
			double real = coeficientes_ft[i][0];
			double imag = coeficientes_ft[i][1];
			espectro[i] = std::sqrt(real * real + imag * imag);
			if (use_log_energy)
				espectro[i] = std::log10(1 + espectro[i]);
		}
		//escalar el visor del espectro
		setMaxValue(global_max_energy, espectro);
		//dibujar el audio
		mostrar_samples(ventana);
		//dibujar el espectro
		mostrar_spectrum(espectro);
		//dibujar el espectograma
		mostrar_spectrogram(espectro);
		//dibujar el descriptor MFCC
		mostrar_mfcc(espectro, mfcc);
		//esperar una tecla. ESC=salir, espacio=pausa
		int c = cv::waitKey(100);
		if (c == ' ')
			c = cv::waitKey(0);
		if (c == 27)
			break;
		else if (c == '1')
			global_show_samples = !global_show_samples;
		else if (c == '2')
			global_show_espectro = !global_show_espectro;
		else if (c == '3')
			global_show_espectrograma = !global_show_espectrograma;
		else if (c == '4')
			global_show_mfcc = !global_show_mfcc;
		else if (c == 'p') {
			use_log_energy = !use_log_energy;
			global_max_energy = 0;
			global_spectogram_position = 0;
			global_spectogram_clean = true;
		}
		//evitar cerrar todas las ventanas
		if (!global_show_samples && !global_show_espectro && !global_show_espectrograma && !global_show_mfcc)
			global_show_samples = true;
	}
	fftw_destroy_plan(plan_ft);
	fftw_free(coeficientes_ft);
}

}

int main(int argc, char **argv) {
	try {
		std::vector<std::string> args = get_args_vector(argc, argv);
		std::cout << "CC5213 - Recuperación de Información Multimedia" << std::endl;
		std::cout << "Ejemplo VER AUDIO" << std::endl;
		std::cout << " Usos: " << args[0] << " [filename] [sample_rate]" << std::endl;
		if (args.size() < 3)
			return 0;
		std::string filename = args[1];
		int sample_rate = parse_int(args[2]);
		if (!existe_archivo(filename)) {
			std::cout << "no encuentro " << filename << std::endl;
			return 1;
		}
		//obtener los samples del audio decodificado
		std::vector<short> audio_samples = load_audio_samples(filename, sample_rate);
		if (audio_samples.empty()) {
			std::cout << "no puedo leer audio de " << filename << std::endl;
			return 1;
		}
		ejemplo(audio_samples, sample_rate);
	} catch (const std::exception &ex) {
		std::cout << "Ha ocurrido un ERROR: " << ex.what() << std::endl;
	} catch (...) {
		std::cout << "Ha ocurrido ERROR desconocido" << std::endl;
	}
	return 0;
}
