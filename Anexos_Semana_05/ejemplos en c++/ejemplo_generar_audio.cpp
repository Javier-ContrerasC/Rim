#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "helper.hpp"

// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 15 de abril de 2025

// EJEMPLO AUDIO

//Para reproducir el audio se usa ffplay o VLC:
//ffplay se instala junto con ffmpeg
//En linux basta instalar el package de ffmpeg
//En windows descargar el binario desde ffmpeg.org: Entrar a a "Windows builds from gyan.dev" y escoger alguna version
//Por ejemplo: https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-full.7z

namespace {

void reproducir_archivo_raw(const std::string &new_filename, int sample_rate) {
	// Reproducir los samples raw
	// Forma 1: usando FFplay
	std::stringstream ss1;
	ss1 << "ffplay";
	ss1 << " -v warning";
	// codificacion del archivo de audio (PCM)
	ss1 << " -acodec pcm_s16le";
	// formato del archivo de entrada (samples en formato raw de 16 bits)
	ss1 << " -f s16le";
	// reproducir con un volumen menor al 100%, para que no duelan los oidos :-)
	ss1 << " -volume 80";
	// cantidad de veces a reproducir
	ss1 << " -loop 0";
	// sample rate del archivo de entrada
	ss1 << " -ar " << sample_rate;
	// nombre del archivo de entrada
	ss1 << " \"" << new_filename << "\"";
	std::string commandFFplay = ss1.str();
	std::cout << "    Comando FFplay : " << commandFFplay << std::endl;
	// FORMA 2: Usando VLC
	std::stringstream ss2;
	ss2 << "vlc";
	// formato de entrada raw (codificacion y tipo de archivo)
	ss2 << " --demux=rawaud";
	ss2 << " --rawaud-fourcc=s16l";
	ss2 << " --rawaud-channels 1";
	// sample rate del archivo de entrada
	ss2 << " --rawaud-samplerate " << sample_rate;
	// nombre del archivo de entrada
	ss2 << " \"" << new_filename << "\"";
	std::string commandVlc = ss2.str();
	std::cout << "    Comando VLC    : " << commandVlc << std::endl;
	// system
	std::cout << "probando FFplay..." << std::endl;
	int ret = std::system(commandFFplay.c_str());
	if (ret != 0) {
		std::cout << "probando VLC..." << std::endl;
		ret = std::system(commandVlc.c_str());
	}
	if (ret != 0)
		throw std::runtime_error("error al ejecutar FFplay y VLC! :-(");
}

const double pi = 3.14159265358979323846;

void sumar_onda(const std::string &params_onda, int sample_rate, std::vector<double> &audio_samples) {
	int hertz = 0;
	double amplitud = 0;
	std::vector<std::string> valores = split(params_onda, '*');
	if (valores.size() <= 1) {
		hertz = parse_int(params_onda);
		amplitud = 1;
	} else {
		hertz = parse_int(valores[0]);
		amplitud = parse_double(valores[1]);
	}
	std::cout << " generando " << hertz << " Hz * " << amplitud << std::endl;
	for (int i = 0; i < (int) audio_samples.size(); ++i) {
		double sample = amplitud * std::cos(i * (hertz * 2 * pi) / ((double) sample_rate));
		//sumar a los samples
		audio_samples[i] += sample;
	}
}

std::vector<double> generar_escala(int sample_rate) {
	// rango de frecuencias a generar
	const int frecuencia_inicio_hz = 100;
	const int frecuencia_fin_hz = 22050;
	const int frecuencia_paso = 50;
	// tiempo para cada frecuencia (0.1 segundos)
	const double segundos_por_onda = 0.1;
	const int samples_por_onda = std::round(segundos_por_onda * sample_rate);
	//generar el audio
	std::vector<double> audio_samples;
	for (int hertz = frecuencia_inicio_hz; hertz <= frecuencia_fin_hz; hertz += frecuencia_paso) {
		for (int i = 0; i < samples_por_onda; ++i) {
			//onda de frecuencia hertz
			double sample = std::cos(i * (hertz * 2 * pi) / ((double) sample_rate));
			//append de los samples
			audio_samples.push_back(sample);
		}
	}
	return audio_samples;
}

std::vector<double> generar_suma_ondas(int sample_rate, const std::string &params) {
	static const double segundos = 10;
	int num_samples = std::round(segundos * sample_rate);
	std::vector<std::string> params_ondas = split(params, '+');
	//generar los
	std::vector<double> audio_samples(num_samples, 0);
	for (const std::string &params_onda : params_ondas) {
		sumar_onda(params_onda, sample_rate, audio_samples);
	}
	return audio_samples;
}

std::vector<double> generar_audio_samples(const std::string &generator_parameter, int sample_rate) {
	if (generator_parameter == "ESCALA") {
		return generar_escala(sample_rate);
	} else {
		return generar_suma_ondas(sample_rate, generator_parameter);
	}
}

//el valor maximo de la onda (un signed short soporta hasta +32767)
static const double AMPLITUD_MAXIMA = 32767 * 0.8;

std::vector<short> convertir_samples_s16le(const std::vector<double> &samples) {
	//buscar la amplitud maxima (mayor valor absoluto)
	double max_value = 0;
	for (double value : samples) {
		double d = std::fabs(value);
		if (d > max_value)
			max_value = d;
	}
	//se guardan los samples en formato short (2 bytes por sample)
	std::vector<short> audio(samples.size(), 0);
	for (int i = 0; i < (int) samples.size(); ++i) {
		//se normaliza la amplitud
		double sample = (samples[i] / max_value) * AMPLITUD_MAXIMA;
		//convertirlo a short
		audio[i] = (short) std::round(sample);
	}
	return audio;
}
void guardar_samples(const std::vector<short> &audio_samples, const std::string &new_filename) {
	std::ofstream outfile;
	outfile.open(new_filename, std::ios::trunc | std::ios::binary);
	if (!outfile.is_open()) {
		std::cout << "error escribiendo " << new_filename << std::endl;
		return;
	}
	int num_bytes = sizeof(short) * audio_samples.size();
	outfile.write((char*) audio_samples.data(), num_bytes);
	outfile.close();
	std::cout << "guardado " << new_filename << " (" << num_bytes << " bytes)" << std::endl;
}

}

int main(int argc, char **argv) {
	try {
		std::vector<std::string> args = get_args_vector(argc, argv);
		std::cout << "CC5213 - Recuperación de Información Multimedia" << std::endl;
		std::cout << "Ejemplo GENERAR AUDIO" << std::endl;
		if (args.size() < 3) {
			std::cout << "Uso: " << args[0] << " [archivoSalida.raw] [hertz1+hertz2] [sample_rate] " << std::endl;
			std::cout << "Uso: " << args[0] << " [archivoSalida.raw] ESCALA [sample_rate] " << std::endl;
			return 1;
		}
		std::string new_filename = args[1];
		std::string generator_parameter = args[2];
		int sample_rate = (args.size() < 4) ? 44100 : parse_int(args[3]);
		if (sample_rate <= 0) {
			std::cout << "invalid sample rate " << sample_rate << std::endl;
			return 1;
		}
		std::vector<double> samples = generar_audio_samples(generator_parameter, sample_rate);
		std::vector<short> audio = convertir_samples_s16le(samples);
		guardar_samples(audio, new_filename);
		reproducir_archivo_raw(new_filename, sample_rate);
	} catch (const std::exception &ex) {
		std::cout << "Ha ocurrido un ERROR: " << ex.what() << std::endl;
	} catch (...) {
		std::cout << "Ha ocurrido ERROR desconocido" << std::endl;
	}
	return 0;
}
