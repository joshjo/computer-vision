#ifndef CALIBRACION_H
#define CALIBRACION_H

//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui_c.h>
//#include <opencv2/core/core.hpp>
//#include <iostream>
#include <omp.h>
#include <processCircles.hpp>
#include "data.h"


class Calibracion
{
public:
    Calibracion();

    Mat grayScale(Mat src);
    Mat thresholdMat(Mat src);
    Data calculateCenters(Data resultData, Mat src, int rows, int cols);
    const double PI =3.141592653589793238463;

};

#endif // CALIBRACION_H
