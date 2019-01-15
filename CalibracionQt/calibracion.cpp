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
    //adaptiveThreshold(thresh, thresh,255,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,11,6);
    //adaptiveThreshold(thresh, thresh,255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,11,3);
    adaptiveThreshold(thresh, thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 41, 12);
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(2,2));
    // erode(thresh, thresh, element);
    // dilate(thresh, thresh, element);

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
    vector<Point2f> points;
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
                //rings.push_back(PatternRing(idx, xCenter, yCenter));
                points.push_back(Point( xCenter, yCenter));

            }
        }

    }
    //vector<Point> pointsSorted;
    vector<Point2f> ringsSorted = points;

    cout << "centers " << points.size() << endl;
    if( points.size() == rows*cols)
    {
        orderPoints(rows, cols, ringsSorted, points);

        resultData.centers = ringsSorted;
        Scalar color(23,190,187);
        for (size_t i = 0; i < ringsSorted.size(); ++i){
            circle(resultData.matSrc, ringsSorted[i], 2, color, -1, 8, 0);
            putText(resultData.matSrc,to_string(i+1), ringsSorted[i], FONT_HERSHEY_DUPLEX, 1 , color,  1); // Line Thickness (Optional)
            if( i > 0)
                line(resultData.matSrc,  Point( ringsSorted[i-1].x, ringsSorted[i-1].y),  ringsSorted[i], color, 2);

        }
    }
    resultData.numValids = points.size();
}

void Calibracion::orderPoints(int rows, int cols, vector<Point2f> &ringsSorted, vector<Point2f> centers)
{


    RotatedRect box = minAreaRect( Mat(centers));
    Point2f corners[4];
    box.points( corners );
   /* cout << endl << "rot ";
    for(int i=0; i < 4 ; i++)
        cout << corners[i] << " - ";
    cout << endl;*/
    //order
     Point2f tempSorted[4];
     tempSorted[0] = corners[1];
     tempSorted[1] = corners[2];
     tempSorted[2] = corners[0];
      tempSorted[3] = corners[3];

    float holgura = 5;
    float y_eq = 0;
    float x_eq = 0;


    //Get middle points
    vector<Point2f> middlePoints;
    //
    middlePoints = pointsMiddle(tempSorted[0], tempSorted[1], centers);
    middlePoints  = orderPointsMiddle(tempSorted[0], middlePoints);

    ringsSorted[0] = tempSorted[0];

    for (uint i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[i+1] = middlePoints[i];
    }
    ringsSorted[4] = tempSorted[1];

//
    middlePoints = pointsMiddle(tempSorted[0], tempSorted[2], centers);
    middlePoints  = orderPointsMiddle(tempSorted[0], middlePoints);

    for (uint i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[(i+1)*5] = middlePoints[i];
    }
    ringsSorted[15] = tempSorted[2];
//
    middlePoints = pointsMiddle(tempSorted[2], tempSorted[3], centers);
    middlePoints  = orderPointsMiddle(tempSorted[2], middlePoints);

    for (uint i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[i+16] = middlePoints[i];
    }
    ringsSorted[19] = tempSorted[3];

//
    middlePoints = pointsMiddle(tempSorted[1], tempSorted[3], centers);
    middlePoints  = orderPointsMiddle(tempSorted[1], middlePoints);

    for (uint i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[9+(i*5)] = middlePoints[i];
    }
//
    middlePoints = pointsMiddle(ringsSorted[5], ringsSorted[9], centers);
    middlePoints  = orderPointsMiddle(ringsSorted[5], middlePoints);

    for (uint i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[i+6] = middlePoints[i];
    }
//
    middlePoints = pointsMiddle(ringsSorted[10], ringsSorted[14], centers);
    middlePoints  = orderPointsMiddle(ringsSorted[10], middlePoints);

    for (uint i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[i+11] = middlePoints[i];
    }

}

vector<Point2f> Calibracion::orderPointsMiddle(Point2f p, vector<Point2f> middlePoints)
{
    vector<Point2f> result;
    if(middlePoints.size() == 1)
    {
        result.push_back(middlePoints[0]);
        return result;
    }

    if(middlePoints.size() == 3 )
    {
        float d1 = sqrt(pow(p.x - middlePoints[0].x,2) + pow(p.y - middlePoints[0].y,2) );
        float d2 = sqrt(pow(p.x - middlePoints[1].x,2) + pow(p.y - middlePoints[1].y,2) );
        float d3 = sqrt(pow(p.x - middlePoints[2].x,2) + pow(p.y - middlePoints[2].y,2) );

        if(d1 < d2 && d1 < d3)
        {
            result.push_back(middlePoints[0]);

            if(d2 < d3)
            {
                result.push_back(middlePoints[1]);
                result.push_back(middlePoints[2]);
            }

            else
            {
                result.push_back(middlePoints[2]);
                result.push_back(middlePoints[1]);
            }
        }

        if(d2 < d1 && d2 < d3)
        {
            result.push_back(middlePoints[1]);

            if(d1 < d3)
            {
                result.push_back(middlePoints[0]);
                result.push_back(middlePoints[2]);
            }

            else
            {
                result.push_back(middlePoints[2]);
                result.push_back(middlePoints[0]);
            }
        }

        if(d3 < d1 && d3< d2)
        {
            result.push_back(middlePoints[2]);

            if(d1 < d2)
            {
                result.push_back(middlePoints[0]);
                result.push_back(middlePoints[1]);
            }

            else
            {
                result.push_back(middlePoints[1]);
                result.push_back(middlePoints[0]);
            }
        }

    }

    if(middlePoints.size() == 2)
    {
        float d1 = sqrt(pow(p.x - middlePoints[0].x,2) + pow(p.y - middlePoints[0].y,2) );
        float d2 = sqrt(pow(p.x - middlePoints[1].x,2) + pow(p.y - middlePoints[1].y,2) );

        if(d1 < d2)
        {
            result.push_back(middlePoints[0]);
            result.push_back(middlePoints[1]);
        }

        else
        {
            result.push_back(middlePoints[1]);
            result.push_back(middlePoints[0]);
        }

    }

    return result;

}

vector<Point2f> Calibracion::pointsMiddle(Point2f p1, Point2f p2, vector<Point2f> centers)
{
    vector<Point2f> pointsMiddle;
    float m = (p2.y - p1.y)/(p2.x - p1.x + 0.0001);
    float y_eq, x_eq;
    int holgura = 5;

    for(uint i=0; i<centers.size(); i++)
    {
        if( !(
                    (abs(centers[i].x-p1.x)<1 && abs(centers[i].y - p1.y)<1)
                    || (abs(centers[i].x - p2.x)<1 && abs(centers[i].y - p2.y)<1)
                    ))
        {
            if(abs(centers[i].x - p1.x) > abs(centers[i].y - p1.y))
            {
                y_eq = m*(centers[i].x - p1.x) + p1.y;

                if(abs(y_eq - centers[i].y) <= holgura)
                {
                    pointsMiddle.push_back(centers[i]);
                }

            }
            else
            {
                x_eq = (centers[i].y - p1.y)/m + p1.x;

                if(abs(x_eq - centers[i].x) <= holgura)
                {
                    pointsMiddle.push_back(centers[i]);
                }

            }
        }
    }
    return pointsMiddle;
}
