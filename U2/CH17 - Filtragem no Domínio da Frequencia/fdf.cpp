#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

void trocarQuadrantes(cv::Mat& image) {
  cv::Mat tmp, A, B, C, D;

  // Se a imagem tiver largura ou altura impar, recorta a imagem
  int novaLargura = image.cols % 2 == 0 ? image.cols : image.cols - 1;
  int novaAltura = image.rows % 2 == 0 ? image.rows : image.rows - 1;

  image = image(cv::Rect(0, 0, novaLargura, novaAltura));


  int centerX = novaLargura.cols / 2;
  int centerY = novaAltura.rows / 2;

  // rearranja os quadrantes da transformada de Fourier de forma que 
  // a origem fique no centro da imagem
  // A B   ->  D C
  // C D       B A

  // Define os quadrantes
  A = image(cv::Rect(0, 0, centerX, centerY));
  B = image(cv::Rect(centerX, 0, centerX, centerY));
  C = image(cv::Rect(0, centerY, centerX, centerY));
  D = image(cv::Rect(centerX, centerY, centerX, centerY));

  // Faz a troca dos quadrantes
  // A <-> D
  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);
  // C <-> B
  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}


// Filtra a imagem no domínio da frequência
void passaBaixaFiltro(const cv::Mat &image, cv::Mat &filter){
  cv::Mat_<float> filter2D(image.rows, image.cols);
  int centerX = image.cols / 2;
  int centerY = image.rows / 2;
  int radius = 20;

  // Cria um filtro passa-baixa ideal
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      // Calcula a distancia do centro da imagem
      // Se a distância for menor que o raio, o pixel é 1
      if (pow(i - centerY, 2) + pow(j - centerX, 2) <= pow(radius, 2)) {
        filter2D.at<float>(i, j) = 1;
      } else {
        filter2D.at<float>(i, j) = 0;
      }
    }
  }

  cv::Mat planes[] = {cv::Mat_<float>(filter2D), cv::Mat::zeros(filter2D.size(), CV_32F)};
  cv::merge(planes, 2, filter);
}

int main(int argc, char** argv) {
  cv::Mat image, imagemExpandida, imagemComplexa;
  std::vector<cv::Mat> planos; 

  image = imread(argv[1], cv::IMREAD_GRAYSCALE);
  if (image.empty()) {
    std::cout << "Erro abrindo imagem" << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  // expande a imagem de entrada para o melhor tamanho no qual a DFT pode ser
  // executada, preenchendo com zeros a lateral inferior direita.
  int dft_M = cv::getOptimalDFTSize(image.rows);
  int dft_N = cv::getOptimalDFTSize(image.cols); 
  cv::copyMakeBorder(image, imagemExpandida, 0, dft_M - image.rows, 0, dft_N - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

  // prepara a matriz complexa para ser preenchida
  // primeiro a parte real, contendo a imagem de entrada
  planos.push_back(cv::Mat_<float>(imagemExpandida)); 

  // depois a parte imaginaria com valores nulos
  planos.push_back(cv::Mat::zeros(imagemExpandida.size(), CV_32F));

  // combina os planos em uma unica estrutura de dados complexa
  cv::merge(planos, imagemComplexa);  

  // calcula a DFT
  cv::dft(imagemComplexa, imagemComplexa); 
  trocarQuadrantes(imagemComplexa);

  // cria o filtro ideal e aplica a filtragem de frequencia
  cv::Mat filter;
  passaBaixaFiltro(imagemComplexa, filter);
  cv::mulSpectrums(imagemComplexa, filter, imagemComplexa, 0);

  // calcula a DFT inversa
  trocarQuadrantes(imagemComplexa);
  cv::idft(imagemComplexa, imagemComplexa);

  // planos[0] : Re(DFT(image)
  // planos[1] : Im(DFT(image)
  cv::split(imagemComplexa, planos);

  // recorta a imagem filtrada para o tamanho original
  // selecionando a regiao de interesse (roi)
  cv::Rect roi(0, 0, image.cols, image.rows);
  cv::Mat result = planos[0](roi);

  // normaliza a parte real para exibicao
  cv::normalize(result, result, 0, 1, cv::NORM_MINMAX);

  cv::imshow("Resultado", result);
  cv::imwrite("../dft.png", result * 255);

  cv::waitKey();
  return EXIT_SUCCESS;
}