#include <opencv2/opencv.hpp>
#include <iostream>

/*  
    Função para trocar os quadrantes
    após a Transformada Rápida de Fourier (DFT)
*/
void trocarQuadrantes(cv::Mat &imagem) {
    cv::Mat tmp = imagem.clone();
    int novaLargura = imagem.cols % 2 == 0 ? imagem.cols : imagem.cols - 1;
    int novaAltura = imagem.rows % 2 == 0 ? imagem.rows : imagem.rows - 1;

    int linhas = imagem.rows; 
    int colunas = imagem.cols;
    int cx = colunas / 2; 
    int cy = linhas / 2; 

    // Troca dos quadrantes (Topo-Esquerdo com Inferior-Direito)
    // A B   ->   D C
    // C D        B A
    tmp(cv::Rect(0, 0, cx, cy)) = imagem(cv::Rect(cx, cy, cx, cy));
    tmp(cv::Rect(cx, cy, cx, cy)) = imagem(cv::Rect(0, 0, cx, cy));
    tmp(cv::Rect(0, cy, cx, cy)) = imagem(cv::Rect(cx, 0, cx, cy));
    tmp(cv::Rect(cx, 0, cx, cy)) = imagem(cv::Rect(0, cy, cx, cy));


    tmp.copyTo(imagem); // Copia a imagem alterada
}


int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Exemplo: ./dft lena.png" << std::endl; // Exemplo de uso
        return -1;
    }


    cv::Mat imagem = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (imagem.empty()) { 
        std::cout << "Erro ao tentar carregar " << argv[1] << std::endl;
        return -1;
    }

    // Expansao da imagem para o tamanho otimo para a DFT
    int dft_M = cv::getOptimalDFTSize(imagem.rows); // Obtem o tamanho otimo para as linhas
    int dft_N = cv::getOptimalDFTSize(imagem.cols); // Obtem o tamanho otimo para as colunas
    cv::Mat imagemExpandida; // Cria imagem expandida
    
    // Expande a imagem com borda preta
    cv::copyMakeBorder(imagem, imagemExpandida,
        0, dft_M - imagem.rows, 0, dft_N - imagem.cols,
        cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // Preparacao da matriz complexa (parte real e imaginaria)
    // Parte real e parte imaginaria (inicializada com zeros)
    cv::Mat planos[] = {cv::Mat_<float>(imagemExpandida), cv::Mat::zeros(imagemExpandida.size(), CV_32F)};
    cv::Mat imagem_complexa;
    cv::merge(planos, 2, imagem_complexa); // Junta as partes real e imaginária

    // Realizar a DFT
    cv::dft(imagem_complexa, imagem_complexa);
    trocarQuadrantes(imagem_complexa); // Troca os quadrantes

    // Separar a imagem complexa em magnitude e fase
    cv::Mat planos_split[2];
    cv::split(imagem_complexa, planos_split); // Divide parte real e imaginaria

    // Calcular o espectro de magnitude e fase
    cv::Mat magnitude, fase;
    cv::cartToPolar(planos_split[0], planos_split[1], magnitude, fase, false); // Converte para polar (magnitude e fase)

    // Normalizar a fase para visualizacao
    cv::normalize(fase, fase, 0, 1, cv::NORM_MINMAX); // Normaliza a fase

    // Calcular o espectro de magnitude
    cv::magnitude(planos_split[0], planos_split[1], magnitude); // Calcula a magnitude da DFT

    // Adicionar uma constante para evitar log(0)
    magnitude += 1;

    // Calcular o logaritmo da magnitude
    cv::Mat log_magnitude;
    cv::log(magnitude, log_magnitude); // Aplica o log
    cv::normalize(log_magnitude, log_magnitude, 0, 1, cv::NORM_MINMAX); // Normaliza

    cv::imshow("Imagem Original", imagem);
    cv::imshow("Espectro de Magnitude", log_magnitude);
    cv::imshow("Espectro de Fase", fase);

    cv::waitKey(0);

    return 0;
}
