#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Uso: " << argv[0] << " <imagem_entrada.jpg> <prefixo_saida>" << std::endl;
        return -1;
    }

    int num_Clusters = 8;
    int num_rodadas = 1;
    int Repeticoes = 10;

    for (int i = 0; i < Repeticoes; i++) {
        cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
        if (img.empty()) {
            std::cout << "Erro ao abrir a imagem: " << argv[1] << std::endl;
            return -1;
        }

        // Transformar a imagem em um conjunto de amostras para k-means
        cv::Mat amostras(img.rows * img.cols, 3, CV_32F);
        for (int y = 0; y < img.rows; y++) {
            for (int x = 0; x < img.cols; x++) {
                // Para cada pixel, temos um vetor de 3 dimensoes
                for (int canal = 0; canal < 3; canal++) {
                    amostras.at<float>(y + x * img.rows, canal) = img.at<cv::Vec3b>(y, x)[canal];
                }
            }
        }

        // Aplicar k-means
        cv::Mat rotulos, centros;
        cv::kmeans(
            amostras,
            num_Clusters,
            rotulos,
            // Criterios de parada
            cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 10, 0.0001),
            num_rodadas,
            cv::KMEANS_PP_CENTERS,
            centros
        );

        // Criar a imagem rotulada
        cv::Mat imagemRotulada(img.size(), img.type());
        for (int y = 0; y < img.rows; y++) {
            for (int x = 0; x < img.cols; x++) {
                int indiceCentro = rotulos.at<int>(y + x * img.rows, 0);
                for (int canal = 0; canal < 3; canal++) {
                    imagemRotulada.at<cv::Vec3b>(y, x)[canal] = static_cast<uchar>(centros.at<float>(indiceCentro, canal));
                }
            }
        }

        // Salvar a imagem resultante
        std::string prefixoSaida = argv[2];
        std::string nomeImagemSaida = "../saida/" + prefixoSaida + std::to_string(i) + ".jpg";
        if (!cv::imwrite(nomeImagemSaida, imagemRotulada)) {
            std::cout << "Erro ao salvar a imagem: " << nomeImagemSaida << std::endl;
        }
    }
    return 0;
}
