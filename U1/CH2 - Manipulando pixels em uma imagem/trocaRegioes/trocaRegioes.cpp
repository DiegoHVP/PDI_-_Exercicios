#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Carregar a imagem
    cv::Mat img = cv::imread("../imagem.jpg", cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cout << "Erro ao carregar a imagem!" << std::endl;
        return -1;
    }

    // Verifica as dimensoes
    if (img.cols % 2 != 0 || img.rows % 2 != 0) {
        std::cout << "A imagem deve ter dimensões múltiplas de 2!" << std::endl;
        return -1;
    }

    // Encontrar o meio
    int meio_x = img.cols / 2;
    int meio_y = img.rows / 2;

    // Criar as regioes corretas
    cv::Mat quad1 = img(cv::Rect(0, 0, meio_x, meio_y));
    cv::Mat quad3 = img(cv::Rect(meio_x, 0, meio_x, meio_y));
    cv::Mat quad2 = img(cv::Rect(0, meio_y, meio_x, meio_y));
    cv::Mat quad4 = img(cv::Rect(meio_x, meio_y, meio_x, meio_y));

    // Criar uma copia da imagem
    cv::Mat img_result = img.clone();

    // Trocar os quadrantes
    quad1.copyTo(img_result(cv::Rect(meio_x, 0, meio_x, meio_y)));
    quad2.copyTo(img_result(cv::Rect(0, 0, meio_x, meio_y)));
    quad3.copyTo(img_result(cv::Rect(meio_x, meio_y, meio_x, meio_y)));
    quad4.copyTo(img_result(cv::Rect(0, meio_y, meio_x, meio_y))); 

    // Exibir a imagem
    cv::imshow("Imagem com Quadrantes Trocados", img_result);
    cv::waitKey(0);

    return 0;
}
