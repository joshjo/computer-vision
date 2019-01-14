#include "calibracion.h"
#include "patternpoint.h"
#include "patternring.h"


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
    //adaptiveThreshold(thresh, thresh,255,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,11,6);
    //adaptiveThreshold(thresh, thresh,255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,11,3);
    adaptiveThreshold(thresh, thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 41, 12);
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(2,2));
    erode(thresh, thresh, element);
    dilate(thresh, thresh, element);

}

void Calibracion::calculateCenters(Data &resultData, Mat srcThresh, int rows, int cols)
{

    ProcessCircles pc;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    int idx = 0;
    Vec3f circleTemp;
    RotatedRect rectRot;
    vector<int> ellipses;

   // Canny(srcThresh, srcThresh, 50, 150, 3);
    //findContours(srcThresh, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );// //0.001374
    findContours( srcThresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    cout << contours.size() << "size" << endl;
    double indexCircularity = 0;
    vector<PatternRing> rings;
    vector<Point> points;
    for(; idx >= 0 ; idx = hierarchy[idx][0] )
    {

        if(contours[idx].size() > 4 && !(hierarchy[idx][2] == -1 &&  hierarchy[idx][3] == -1 ))
        {

            indexCircularity = (4 * PI * contourArea(contours[idx]))/pow(arcLength(contours[idx], true),2);

            if( indexCircularity > 0.65 && contours[hierarchy[idx][2]].size() > 4)
            {
                drawContours( resultData.matContours, contours, idx,  Scalar(255,0,255), CV_INTER_LINEAR, 8, hierarchy );
                drawContours( resultData.matContours, contours, hierarchy[idx][2],  Scalar(0,0,255), CV_INTER_LINEAR, 8, hierarchy );

                RotatedRect rect = fitEllipse(contours[idx]);
                RotatedRect rectChild = fitEllipse(contours[hierarchy[idx][2]]);

                //mean center
                double xCenter = (rect.center.x + rectChild.center.x)/2;
                double yCenter = (rect.center.y + rectChild.center.y)/2;
                rings.push_back(PatternRing(idx, xCenter, yCenter));
                points.push_back(Point( xCenter, yCenter));
                //circle( resultData.matContours, Point(xCenter, yCenter), 1, Scalar(0,0,255), 1, 8, 0 );


               // ellipses.push_back(idx);
            }
        }

    }
    RotatedRect box = cv::minAreaRect(cv::Mat(points));

    Point2f vertices[4];
    box.points(vertices); //bottomLeft, topLeft, topRight, bottomRight
    for (int i = 0; i < 4; i++)
        line(resultData.matContours, vertices[i], vertices[(i+1)%4], Scalar(0,255,0), 2);

    //Buscar los puntos mas cercanos a la linea topLeft, topRight count == 4


    //Buscar Xmax, Ymax, Xmin, Ymin of centers
    if(rings.size() == cols*rows)
    {
        double xCenterMax = 0;
        double yCenterMax = 0;
        double xCenterMin = INFINITY;
        double yCenterMin = INFINITY;
        for(int i = 0 ; i < rings.size() ; i++)
        {
            if(xCenterMax < rings.at(i).xcenter)
                xCenterMax = rings.at(i).xcenter;
            else if(xCenterMin > rings.at(i).xcenter)
                xCenterMin = rings.at(i).xcenter;
            if(yCenterMax < rings.at(i).ycenter)
                yCenterMax = rings.at(i).ycenter;
            else if(yCenterMin > rings.at(i).ycenter)
                yCenterMin = rings.at(i).ycenter;
        }
        cout << "xMax: " << xCenterMax << "yMax: " << yCenterMax << "xMim: " << xCenterMin << "yMin: " <<yCenterMin << endl;
      //  rectangle(resultData.matContours, Point(xCenterMin,yCenterMin), Point(xCenterMax,yCenterMax),Scalar(120,120,0),CV_WARP_FILL_OUTLIERS);
    }
    //distancia netre los puntos
   /* PatternMatrix pm(cols, rows);
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
*/
    resultData.numValids = 30;
}
