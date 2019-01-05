﻿#include "calibracion.h"


Calibracion ::Calibracion()
{

}

Mat Calibracion::grayScale(Mat src)
{
    Mat greyColor;
    cvtColor(src, greyColor, CV_BGR2GRAY);
    return greyColor;
}

Mat Calibracion::thresholdMat(Mat src)
{
    Mat thresh;
    GaussianBlur(src, thresh,Size(9,9), 2, 2);
    adaptiveThreshold(thresh, thresh,255,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,11,6);
    return thresh;
}

Data Calibracion::calculateCenters(Mat original, Mat srcThresh, int rows, int cols)
{

    Data resultData;
    Mat resultCenter = original.clone();
    Mat resultContours = original.clone();
    ProcessCircles pc;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Canny(srcThresh, srcThresh, 50, 150, 3); //0.002818
    findContours(srcThresh, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );// //0.001374

    int idx = 0;
    Vec3f circleTemp;
    RotatedRect rectRot;


    float a; //mjor
    float b; //minor
    //proc0.00132
    for(; idx >= 0 ; idx = hierarchy[idx][0] )
    {
        if( contours[idx].size() > 5 && (hierarchy[idx][2] > -1 || hierarchy[idx][3] > -1 ) )
        {

            rectRot = fitEllipse(contours[idx]);
            if(rectRot.size.width< 50 )
            {
                if(rectRot.size.width > rectRot.size.height)
                {
                    a = rectRot.size.width/2 ;
                    b =rectRot.size.height/2;
                } else
                {
                    b = rectRot.size.width/2 ;
                    a =rectRot.size.height/2;
                }
                if( a/b > 1 && a/b <1.5)
                {
                    //cout << " ratio: " << a /b;
                    ellipse( resultContours,rectRot, Scalar(255,0,255) );
                    circleTemp[0] =  rectRot.center.x; //X center
                    circleTemp[1] = rectRot.center.y; //Y center
                    circleTemp[2] =  rectRot.size.width/2;//radio
                    pc.add(circleTemp);
                }
            }
        }
    }

    //grid 0.000463

    PatternMatrix pm(cols, rows);
    pm.run(pc);
    if (pm.isValid) {
        for(auto& it: pm.circles) {
            Point center(cvRound(it->x), cvRound(it->y));
            circle(resultCenter, center, it->ri, Scalar(0,0,255), 1, 8, 0 );
            circle(resultCenter, center, it->r, Scalar(0,0,255), 1, 8, 0 );
            putText(resultCenter, to_string(it->id), center, FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        }

        for (int i = 0; i < (pm.matrix->size() - 1); i++) {
            if(pm.matrix->at(i) != NULL && pm.matrix->at(i + 1) != NULL) {
                Point p1(cvRound(pm.matrix->at(i)->x), cvRound(pm.matrix->at(i)->y));
                Point p2(cvRound(pm.matrix->at(i + 1)->x), cvRound(pm.matrix->at(i + 1)->y));
                line(resultCenter, p1, p2, Scalar(255, 255, 255), 2);
            }
        }
    }


    resultData.matSrc = resultCenter;
    resultData.matContours = resultContours;
    resultData.numValids = pm.numberValids();
    return resultData;

}
