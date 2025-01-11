#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    // Declarando as variáveis de imagem e dimensões
    cv::Mat image, mask;
    int altura, largura;
    int num_objetos;

    cv::Point p; // Ponto para marcação durante o floodFill

    // Carregando a imagem em escala de cinza
    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    // Verificando se a imagem foi carregada corretamente
    if (!image.data) {
        std::cout << "Imagem não carregou corretamente\n";
        return -1;
    }

    // Obtendo a altura e largura da imagem
    altura = image.size().height;
    largura = image.size().width;

    // Inicializando o ponto inicial
    p.x = 0;
    p.y = 0;

    // Eliminando objetos nas bordas
    for (int aux = 0; aux < largura; aux++) {
        cv::floodFill(image, cv::Point(aux, 0), 0);              // Linha superior
        cv::floodFill(image, cv::Point(aux, altura - 1), 0);    // Linha inferior
    }

    for (int aux = 0; aux < altura; aux++) {
        cv::floodFill(image, cv::Point(0, aux), 0);             // Coluna esquerda
        cv::floodFill(image, cv::Point(largura - 1, aux), 0);   // Coluna direita
    }

    // Contando os objetos na imagem
    num_objetos = 0;
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (image.at<uchar>(i, j) == 255) {
                // Objeto encontrado
                num_objetos++;
                p.x = j;
                p.y = i;
                cv::floodFill(image, p, 190); // Preenchendo o objeto com tom 190
            }
        }
    }

    // Preenchendo o fundo da imagem com tom de cinza 80
    cv::floodFill(image, cv::Point(0, 0), 80);

    // Contando os buracos na imagem
    int num_buracos = 0;
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (image.at<uchar>(i, j) == 0) {
                // Buraco encontrado
                p.x = j;
                p.y = i;
                cv::floodFill(image, p, 80); // Preenchendo o buraco para evitar contagem duplicada
                num_buracos++;
            }
        }
    }

    // Exibindo o número de objetos e buracos encontrados
    std::cout << "Número de objetos: " << num_objetos << std::endl;
    std::cout << "Número de buracos: " << num_buracos << std::endl;

    // Exibindo e salvando a imagem processada
    cv::imshow("Imagem Processada", image);
    cv::imwrite("../labeling.png", image);
    cv::waitKey();

    return 0;
}
