#include "calibracion.h"


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
    GaussianBlur(src, thresh,Size(9,9), 0, 0);
    adaptiveThreshold(thresh, thresh,255,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,11,6);
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

    Canny(srcThresh, srcThresh, 50, 150, 3);
    findContours(srcThresh, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);


    double indexCircularity = 0; //circularidad
    Vec3f circleTemp;
    int idx = 0;
    for(; idx >= 0 ; idx = hierarchy[idx][0] )
    {
        if((hierarchy[idx][2] > -1 || hierarchy[idx][3] > -1 ))
        {
            indexCircularity = (4 * PI * contourArea(contours[idx]))/pow(arcLength(contours[idx], true),2);

            Rect rect = boundingRect(contours[idx]);
            if(indexCircularity > 0.65)
            {
                drawContours( resultContours, contours, idx,  Scalar(255,0,255), CV_FILLED, 8, hierarchy );
                Moments m = moments(contours[idx]);
                circleTemp[0] =  m.m10/m.m00; //X center
                circleTemp[1] =  m.m01/m.m00; //Y center
                circleTemp[2] =  rect.width / 2;//radio
                pc.add(circleTemp);

            }
        }
        //else  cout << "not " << hierarchy[idx][1] << " " << hierarchy[idx][2] << hierarchy[idx][3] << endl;

    }

    vector<Vec4f> filter;
    for (auto & it : pc.circleGroups) {
        if (it.circles.size() == 2) {
            filter.push_back(it.getPoint());
        }
    }

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
