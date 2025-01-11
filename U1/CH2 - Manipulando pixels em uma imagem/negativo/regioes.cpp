#include <opencv2/opencv.hpp>
#include <iostream>

int main() {

    // Carregar a imagem
    cv::Mat img = cv::imread("../imagem.jpg", cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cout << "Erro ao carregar a imagem!" << std::endl;
        return -1;
    }

    // Coordenadas dos pontos P1 e P2
    int x1, y1, x2, y2;
    std::cout << "Informe os pontos de P1 (x1, y1): ";
    std::cin >> x1 >> y1;
    std::cout << "Informe os pontos de P2 (x2, y2): ";
    std::cin >> x2 >> y2;

    // trata os limites da imagem
    x1 = std::max(0, std::min(x1, img.cols - 1));
    y1 = std::max(0, std::min(y1, img.rows - 1));
    x2 = std::max(0, std::min(x2, img.cols - 1));
    y2 = std::max(0, std::min(y2, img.rows - 1));
    
    // trata limites invertidos
    if (x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 > y2) {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    // Copia da imagem
    cv::Mat img_negativo = img.clone();

    // Aplicar o negativo
    for (int i = x1; i < x2; i++) {
        for (int j = y1; j < y2; j++) {
           img_negativo.at<uchar>(i, j) = 255 - img_negativo.at<uchar>(i, j);
        }
    }
    
    
    // Exibir a imagem
    cv::imshow("Imagem com Negativo na Região", img_negativo);
    cv::waitKey(0);

    return 0;
}
