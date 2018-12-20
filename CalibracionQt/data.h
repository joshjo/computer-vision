#ifndef DATA_H
#define DATA_H
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class Data
{
public:
    Data();
public:
    Mat matSrc;
    Mat matContours;
    int numValids;
};

#endif // DATA_H
