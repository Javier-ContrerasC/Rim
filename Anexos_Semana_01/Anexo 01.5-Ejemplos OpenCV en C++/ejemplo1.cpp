// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 10 de marzo de 2025

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv) {
	if (argc < 2) {
        std::cout << "uso: " << argv[0] << " [nombre_de_imagen]" << std::endl; 
		return 1;
	}
    std::string filename = argv[1];
    cv::Mat img_color = cv::imread(filename, cv::IMREAD_COLOR); 
    if (!img_color.data)
        return 1; 
    cv::Mat img_gris, img_bin; 
    cv::cvtColor(img_color, img_gris, cv::COLOR_BGR2GRAY); 
    double th = cv::threshold(img_gris, img_bin, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU); 
    std::cout << filename << " size=" << img_color.size() << " binary_threshold=" << th << std::endl; 
    cv::imshow(filename + " (color)", img_color); 
    cv::imshow(filename + " (gris)", img_gris); 
    cv::imshow(filename + " (bin)", img_bin); 
    cv::waitKey(0); 
    return 0;
}
