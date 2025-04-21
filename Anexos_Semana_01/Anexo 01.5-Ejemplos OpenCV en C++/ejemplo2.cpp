// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 10 de marzo de 2025

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "uso: " << argv[0] << " [video.mp4 | id webcam 0,1,2]" << std::endl;
		return 1;
	}
	std::string filename = argv[1];
	//abrir video
	cv::VideoCapture capture;
	capture.open(filename);
	if (!capture.isOpened())
		capture.open(std::atoi(argv[1]));
	if (!capture.isOpened())
		return 1;
	//procesar frames
	cv::namedWindow(filename + " (video)", cv::WINDOW_AUTOSIZE);
	cv::Mat frame_color, frame_gris, frame_bin;
	while (capture.grab()) {
		if (!capture.retrieve(frame_color))
			break;
		cv::cvtColor(frame_color, frame_gris, cv::COLOR_BGR2GRAY);
		double th = cv::threshold(frame_gris, frame_bin, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		std::cout << "frame_size=" << frame_bin.size() << " binary_threshold=" << th << std::endl;
		cv::imshow(filename + " (video)", frame_gris);
		cv::imshow(filename + " (bin)", frame_bin);
		char c = cv::waitKey(33);
		if (c == 27 || c == 'q') //tecla ESC o q
			break;
	}
	return 0;
}
