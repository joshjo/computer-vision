#ifndef CALIBRACION_H
#define CALIBRACION_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core.hpp>
#include <iostream>



using namespace cv;

class Calibracion
{
public:
    Calibracion(const char *filename);
    Calibracion();

    void initProcess();
    void grayScale(Mat &src);
    void threshold(Mat &src);
    void dilate(Mat &src);
    void findEdge(Mat &src);

    //windows

};

#endif // CALIBRACION_H
