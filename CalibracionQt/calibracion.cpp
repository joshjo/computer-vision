#include "calibracion.h"


Calibracion ::Calibracion()
{

}

void Calibracion::grayScale(Mat &greyColor, Mat src)
{
    cvtColor(src, greyColor, CV_BGR2GRAY);
}

void Calibracion::thresholdMat(Mat &thresh, Mat src)
{
    GaussianBlur(src, thresh,Size(9,9), 2, 2);
    adaptiveThreshold(thresh, thresh,255,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,11,6);
}

void Calibracion::calculateCenters(Data &resultData, Mat srcThresh, int rows, int cols)
{

    ProcessCircles pc;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    int idx = 0;
    Vec3f circleTemp;
    RotatedRect rectRot;

    Canny(srcThresh, srcThresh, 50, 150, 3);
    findContours(srcThresh, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );// //0.001374

    double indexCircularity = 0;

    for(; idx >= 0 ; idx = hierarchy[idx][0] )
    {
        if( !(hierarchy[idx][2] == -1 &&  hierarchy[idx][3] == -1 ))
        {

            indexCircularity = (4 * PI * contourArea(contours[idx]))/pow(arcLength(contours[idx], true),2);

            Rect rect = boundingRect(contours[idx]);
            //(rect.width / 2) > 1 &&
            if( rect.width > 1 && indexCircularity > 0.65)
            {
                drawContours( resultData.matContours, contours, idx,  Scalar(255,0,255), CV_FILLED, 8, hierarchy );
                Moments m = moments(contours[idx]);
                circleTemp[0] =  m.m10/m.m00; //X center
                circleTemp[1] =  m.m01/m.m00; //Y center
                circleTemp[2] =  rect.width / 2;//radio
                pc.add(circleTemp);
                //cout << rect.width << "[" <<circleTemp[0] <<", " <<circleTemp[1] <<", " <<circleTemp[3]  << "], "<< endl;
           }
        }
    }

    PatternMatrix pm(cols, rows);
    pm.run(pc);
    if (pm.isValid) {
        for(auto& it: pm.circles) {
            Point center(cvRound(it->x), cvRound(it->y));
            corners.push_back(center);
            circle(resultData.matSrc, center, it->ri, Scalar(0,0,255), 1, 8, 0 );
            circle(resultData.matSrc, center, it->r, Scalar(0,0,255), 1, 8, 0 );
            putText(resultData.matSrc, to_string(it->id), center, FONT_HERSHEY_TRIPLEX, 1, Scalar(255,255,255));
        }

        for (int i = 0; i < (pm.matrix->size() - 1); i++) {
            if(pm.matrix->at(i) != NULL && pm.matrix->at(i + 1) != NULL) {
                Point p1(cvRound(pm.matrix->at(i)->x), cvRound(pm.matrix->at(i)->y));
                Point p2(cvRound(pm.matrix->at(i + 1)->x), cvRound(pm.matrix->at(i + 1)->y));
                line(resultData.matSrc, p1, p2, Scalar(255, 255, 255), 2);
            }
        }
    }

    resultData.numValids = pm.numberValids();
}
