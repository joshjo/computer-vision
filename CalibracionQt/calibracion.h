#ifndef CALIBRACION_H
#define CALIBRACION_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core.hpp>
#include <iostream>



using namespace cv;
using namespace std;

class Calibracion
{
public:
    Calibracion(const char *filename);
    Calibracion();

    void initProcess();
    Mat grayScale(Mat src);
    Mat thresholdMat(Mat src);
    Mat dilateMat(Mat src);
    Mat findEdgeMat(Mat original, Mat src);
    const double PI =3.141592653589793238463;
    //windows

};

#endif // CALIBRACION_H
