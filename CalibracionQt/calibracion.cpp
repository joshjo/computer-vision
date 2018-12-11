#include "calibracion.h"

Calibracion::Calibracion(const char *filename)
{

}

Calibracion ::Calibracion()
{

}
void Calibracion::initProcess()
{
    //Mat image = imread("/home/liz/MEGAsync/Semestre-II/Imagenes-Loayza/Calibracion/sources/prueba2.png", 1 );

    namedWindow("grayScale", WINDOW_AUTOSIZE);
    //grayScale(image);
}

void Calibracion::grayScale(Mat &src)
{
    //cvtColor(src, src, CV_BGR2GRAY);
    imshow("grayScale", src);
}

void Calibracion::threshold(Mat &src)
{

}

void Calibracion:: dilate(Mat &src)
{

}

void Calibracion::findEdge(Mat &src)
{

}
