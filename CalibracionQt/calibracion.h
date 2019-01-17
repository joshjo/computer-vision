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
#include "patternring.h"


class Calibracion
{
public:
    Calibracion();

    const double PI =3.141592653589793238463;
    void grayScale(Mat &gray, Mat src);
    void thresholdMat(Mat &thresh, Mat src);
    void calculateCenters(Data &result, Mat src, int rows, int cols);
    void orderPoints(Mat &mat, int rows, int cols, vector<Point2f> &ringsSorted, vector<Point2f> centers);
    vector<Point2f> pointsMiddle(Point2f p1, Point2f p2, vector<Point2f> &centers);
    vector<Point2f> orderPointsMiddle(Point2f p, vector<Point2f> middlePoints);

};

#endif // CALIBRACION_H
