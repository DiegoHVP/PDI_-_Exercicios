#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>

#define PASSO 5
#define VARIACAO 3
#define RAIO 3
#define NUM_EXECUCOES 5

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Uso: " << argv[0] << " imagem.jpg" << std::endl;
        return -1;
    }

    cv::Mat img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cout << "Erro ao abrir a imagem: " << argv[1] << std::endl;
        return -1;
    }

    int largura = img.cols;
    int altura = img.rows;
    // imagem de saida
    cv::Mat pontos(altura, largura, CV_8U, cv::Scalar(255));
    // Semente para numeros aleatorios
    srand(time(0));

    // Vetores de coordenadas
    std::vector<int> faixaX(altura / PASSO), faixaY(largura / PASSO);
    // Preenchimento dos vetores
    std::iota(faixaX.begin(), faixaX.end(), 0);
    std::iota(faixaY.begin(), faixaY.end(), 0);
    // Conversao para coordenadas reais
    // Exemplo: PASSO = 5, faixaX = {0, 1, 2, 3, 4}
    for (int& x : faixaX) x = x * PASSO + PASSO / 2;
    for (int& y : faixaY) y = y * PASSO + PASSO / 2;

    /* Geracao do pontilhismo basico */ 
    // Embaralhamento das coordenadas
    std::random_shuffle(faixaX.begin(), faixaX.end());
    for (int x : faixaX) {
        // Embaralhamento das coordenadas
        std::random_shuffle(faixaY.begin(), faixaY.end());
        for (int y : faixaY) {
            // Adicao de ruido
            int jitterX = x + rand() % (2 * VARIACAO) - VARIACAO;
            int jitterY = y + rand() % (2 * VARIACAO) - VARIACAO;
            // Verificacao dos limites
            // O metodo std::clamp limita o valor de jitterX e jitterY
            // entre 0 e altura - 1 e largura - 1
            int intensidade = img.at<uchar>(std::clamp(jitterX, 0, altura - 1), std::clamp(jitterY, 0, largura - 1));
            cv::circle(pontos, cv::Point(jitterY, jitterX), RAIO, cv::Scalar(intensidade), -1, cv::LINE_AA);
        }
    }
    cv::imshow("Pontos", pontos);
    cv::imwrite("../pontos.jpg", pontos);

    

    // Adicao do Efeito Canny
    for (int exec = 0; exec < NUM_EXECUCOES; exec++) {
        int raio = 5 - exec;
        int limiarInferior = 15 * exec;
        int limiarSuperior = limiarInferior * 3;

        cv::Mat imgCanny;
        cv::Canny(img, imgCanny, limiarInferior, limiarSuperior);
        cv::imshow("Canny", imgCanny);
        cv::imwrite("../canny.jpg", imgCanny);
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                // Pixel da imagem de Canny e maior que 0
                // Entao, desenha-se um circulo na imagem de saida
                // com a intensidade do pixel da imagem original
                if (imgCanny.at<uchar>(i, j) > 0) {
                    int intensidade = img.at<uchar>(i, j);
                    cv::circle(pontos, cv::Point(j, i), raio, cv::Scalar(intensidade), -1, cv::LINE_AA);
                }
            }
        }
    }
    cv::imshow("Pontilhismo com Canny", pontos);
    cv::imwrite("../pontilhismo_canny.jpg", pontos);
    cv::waitKey(0);
    return 0;
}
