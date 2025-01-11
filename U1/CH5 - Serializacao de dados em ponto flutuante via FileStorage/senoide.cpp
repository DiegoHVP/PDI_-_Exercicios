#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <cmath>

int SIDE = 256;
int PERIODOS = 4;

int main(int argc, char** argv) {
    std::stringstream ss_img, ss_yml;
    cv::Mat image;

    // Criar a matriz senoide e salvar como YML
    ss_yml << "senoide-" << SIDE << ".yml";
    image = cv::Mat::zeros(SIDE, SIDE, CV_32FC1);
    cv::FileStorage fs(ss_yml.str(), cv::FileStorage::WRITE);

    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            image.at<float>(i, j) = 127 * sin(2 * M_PI * PERIODOS * j / SIDE) + 128;
        }
    }

    fs << "mat" << image;
    fs.release();



    // Normalizar e salvar como PNG
    cv::Mat image_png;
    cv::normalize(image, image_png, 0, 255, cv::NORM_MINMAX);
    image_png.convertTo(image_png, CV_8U);
    ss_img << "senoide-" << SIDE << ".png";
    cv::imwrite(ss_img.str(), image_png);



    // Reabrir os arquivos e comparar
    cv::Mat yml_mat, png_mat;
    fs.open(ss_yml.str(), cv::FileStorage::READ);
    fs["mat"] >> yml_mat;
    fs.release();
    png_mat = cv::imread(ss_img.str(), cv::IMREAD_GRAYSCALE);
    png_mat.convertTo(png_mat, CV_32FC1);


    // Tracar diferença
    int linha = SIDE / 2; // Linha central
    cv::Mat linha_yml = yml_mat.row(linha);
    cv::Mat linha_png = png_mat.row(linha);

    cv::Mat linha_diff;
    cv::absdiff(linha_yml, linha_png, linha_diff);

    std::ofstream linha_csv("linha_diferenca.csv");
    linha_csv << "Pixel,Valor_PNG,Valor_YML,Diferenca" << std::endl;
    
    // Salvar a diferença em um arquivo CSV
    // Cada linha contém:
    // Indice do pixel, valor do pixel no PNG, valor do pixel no YML e a diferenca entre eles
    for (int i = 0; i < linha_diff.cols; i++) {
        float val_png = linha_png.at<float>(0, i);
        float val_yml = linha_yml.at<float>(0, i);
        float diferenca = linha_diff.at<float>(0, i);
        linha_csv << i << "," << val_png << "," << val_yml << "," << diferenca << std::endl;
    }
    linha_csv.close();

    return 0;
}
