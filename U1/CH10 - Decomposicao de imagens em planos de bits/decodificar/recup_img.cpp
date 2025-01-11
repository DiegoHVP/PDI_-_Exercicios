#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
    cv::Mat img = cv::imread(argv[1]);
    if (img.empty()) {
        std::cout << "Erro ao abrir a imagem" << std::endl;
        return -1;
    }

    cv::Mat img_original = img.clone();
    cv::Mat img_decodificada = img.clone();


    int nbits = 3;

    // Processar a imagem portadora
    // removendo a imagem interna
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            cv::Vec3b &pixel = img_original.at<cv::Vec3b>(y, x);
            pixel[0] = (pixel[0] >> nbits) << nbits; // Canal azul
            pixel[1] = (pixel[1] >> nbits) << nbits; // Canal verde
            pixel[2] = (pixel[2] >> nbits) << nbits; // Canal vermelho
        }
    }

    // Processar a imagem codificada
    // recuperando a imagem interna
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            cv::Vec3b &pixel = img_decodificada.at<cv::Vec3b>(y, x);
            pixel[0] = pixel[0] << (8 - nbits); // Canal azul
            pixel[1] = pixel[1] << (8 - nbits); // Canal verde
            pixel[2] = pixel[2] << (8 - nbits); // Canal vermelho
        }
    }

    // Exibir as imagens
    cv::imshow("Imagem Portadora", img_original);
    cv::imshow("Imagem Codificada", img_decodificada);
    cv::imwrite("../decodificada.png", img_decodificada);
    cv::waitKey(0);

    return 0;
}
