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
    // cout << contours.size() << "size" << endl;
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

    // cout << "centers " << points.size() << endl;
    if( points.size() == rows*cols)
    {
        orderPoints(resultData.matSrc, rows, cols, ringsSorted, points);

        resultData.centers = ringsSorted;
        Scalar color(23,190,187);
        for (size_t i = 0; i < ringsSorted.size() ; ++i){
            circle(resultData.matSrc, ringsSorted[i], 2, color, -1, 8, 0);
            putText(resultData.matSrc,to_string(i+1), ringsSorted[i], FONT_HERSHEY_SCRIPT_COMPLEX, 1 , color,  1); // Line Thickness (Optional)
            if( i > 0)
                line(resultData.matSrc,  Point( ringsSorted[i-1].x, ringsSorted[i-1].y),  ringsSorted[i], color, 2);

        }
    }
    resultData.numValids = points.size();
}

void Calibracion::orderPoints(Mat &mat, int rows, int cols, vector<Point2f> &ringsSorted, vector<Point2f> centers)
{
    vector<Point2f> points;
    vector<Point2f> tempSortedCorner;


    convexHull(Mat(centers), points, true);
    getCorners(points, centers , tempSortedCorner);

    //Add corners in patterns
    vector<Point2f> middlePoints;
    ringsSorted[0] = tempSortedCorner[0];
    ringsSorted[4] = tempSortedCorner[1];
    ringsSorted[15] = tempSortedCorner[2];
    ringsSorted[19] = tempSortedCorner[3];

    //Get middle points
    middlePoints = pointsMiddle(tempSortedCorner[0], tempSortedCorner[1], centers);
    middlePoints  = orderPointsMiddle(tempSortedCorner[0], middlePoints);
    for (int i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[i+1] = middlePoints[i];
    }

    middlePoints = pointsMiddle(tempSortedCorner[0], tempSortedCorner[2], centers);
    middlePoints  = orderPointsMiddle(tempSortedCorner[0], middlePoints);
    for (int i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[(i+1)*5] = middlePoints[i];
    }


    middlePoints = pointsMiddle(tempSortedCorner[2], tempSortedCorner[3], centers);
    middlePoints  = orderPointsMiddle(tempSortedCorner[2], middlePoints);

    for (int i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[i+16] = middlePoints[i];
    }

    middlePoints = pointsMiddle(tempSortedCorner[1], tempSortedCorner[3], centers);
    middlePoints  = orderPointsMiddle(tempSortedCorner[1], middlePoints);

    for (int i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[9+(i*5)] = middlePoints[i];
    }

    middlePoints = pointsMiddle(ringsSorted[5], ringsSorted[9], centers);
    middlePoints  = orderPointsMiddle(ringsSorted[5], middlePoints);

    for (int i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[i+6] = middlePoints[i];
    }

    middlePoints = pointsMiddle(ringsSorted[10], ringsSorted[14], centers);
    middlePoints  = orderPointsMiddle(ringsSorted[10], middlePoints);

    for (int i=0; i<middlePoints.size(); i++)
    {
        ringsSorted[i+11] = middlePoints[i];
    }

}

/*
void Calibracion::orderPoints(Mat &mat, int rows, int cols, vector<Point2f> &ringsSorted, vector<Point2f> centers)
{
    RotatedRect box = minAreaRect( Mat(centers));
    Point2f tempSorted[4];
    box.points( tempSorted );
    Scalar color(255,0,0);

    for(int i = 0; i < 4 ; i++)
    {
      //  cout << tempSorted[i] << "-" ;
    }

    cout << endl;

    //verificar el angulo para determinar
    Point2f buttonLeft = tempSorted[0];
    Point2f buttonRight = tempSorted[3];
    Point2f topLeft = tempSorted[1];
    Point2f topRight = tempSorted[2];


    if( box.angle > -10 && box.angle < -9.5)
    {
         cout << box.angle << endl;
    //calcular row 1 topleft - toprigth
    vector<Point2f> points = pointsMiddle(cols, topLeft, topRight, centers);
    for (size_t i = 0; i < points.size(); ++i){
        circle(mat, points[i], 5, color, -1, 8, 0);
        //putText(resultData.matSrc,to_string(i+1), ringsSorted[i], FONT_HERSHEY_SCRIPT_COMPLEX, 1 , color,  1); // Line Thickness (Optional)
        if( i > 0)
            line(mat,  Point( points[i-1].x, points[i-1].y),  points[i], color, 2);

    }
    }
    else
        cout << "-" <<box.angle << endl;
}
*/

void Calibracion::getCorners(vector<Point2f> points, vector<Point2f> centers, vector<Point2f> &corners)
{
    points.push_back(points[0]);
    points.push_back(points[1]);

    float x_s = points[0].x;
    float y_s = points[0].y;
    float x_holgura = 0.000001;
    float m = (points[1].y - y_s)/(points[1].x - x_s + x_holgura);
    float holgura = 10;
    float y_eq = 0;
    float x_eq = 0;

    for(uint i=1; i<points.size(); i++)
    {
        //  cout << corners[i] << " - " ;
        if(abs(points[i].x - x_s) > abs(points[i].y - y_s))
        {
            y_eq = m*(points[i].x - x_s) + y_s;

            if(abs(y_eq - points[i].y) >= holgura)
            {
                corners.push_back(points[i-1]);
                x_s = points[i-1].x;
                y_s = points[i-1].y;
                m = (points[i].y - y_s)/(points[i].x - x_s + x_holgura);
            }

        }
        else
        {
            x_eq = (points[i].y - y_s)/m + x_s;

            if(abs(x_eq - points[i].x) >= holgura)
            {
                corners.push_back(points[i-1]);
                x_s = points[i-1].x;
                y_s = points[i-1].y;
                m = (points[i].y - y_s)/(points[i].x - x_s + x_holgura);
            }

        }

        // cout << corners[i] << endl ;

    }
    //cout<<sorted_ellipses.size()<<endl;
    x_s = corners[0].x;
    y_s = corners[0].y;
    m = (corners[1].y - y_s)/(corners[1].x - x_s + x_holgura);

    int count_dist1 = 0;
    int count_dist2 = 0;

    for(uint i=0; i<centers.size(); i++)
    {
        if(abs(centers[i].x - x_s) > abs(centers[i].y - y_s))
        {
            y_eq = m*(centers[i].x - x_s) + y_s;

            if(abs(y_eq - centers[i].y) >= holgura)
            {
                count_dist1++;
            }

        }
        else
        {
            x_eq = (centers[i].y - y_s)/m + x_s;

            if(abs(x_eq - centers[i].x) >= holgura)
            {
                count_dist1++;
            }

        }
    }

    x_s = corners[1].x;
    y_s = corners[1].y;
    m = (corners[2].y - y_s)/(corners[2].x - x_s + x_holgura);

    for(uint i=0; i<centers.size(); i++)
    {
        if(abs(centers[i].x - x_s) > abs(centers[i].y - y_s))
        {
            y_eq = m*(centers[i].x - x_s) + y_s;

            if(abs(y_eq - centers[i].y) >= holgura)
            {
                count_dist2++;
            }

        }
        else
        {
            x_eq = (centers[i].y - y_s)/m + x_s;

            if(abs(x_eq - centers[i].x) >= holgura)
            {
                count_dist2++;
            }

        }
    }


    if(count_dist1 < count_dist2)
    {
        Point p = corners[2];
        corners[2] = corners[3];
        corners[3] = p;
    }
    else
    {
        vector<Point2f> temp;
        temp.push_back(corners[3]);
        temp.push_back(corners[0]);
        temp.push_back(corners[2]);
        temp.push_back(corners[1]);
        corners = temp;

    }


}

vector<Point2f> Calibracion::pointsMiddleRow(int numPoints, Point2f p1, Point2f p2, vector<Point2f> &centers)
{
    vector<Point2f> pointsMiddle;
    vector<PointDist> distances;

    float m = (p1.y - p1.y)/(p1.x - p2.x + 0.00001);
    float n = (p1.y + p2.y)/2;
    float d = 0;
    float y = 0;
    //calculamos la disancia para cada punto
    for(int i = 0 ; i < centers.size(); i++)
    {
        //d = |yo - y(x0)| /y = mx + n
        y = m*centers[i].x + n;
        d = abs(centers[i].y - y);
        distances.push_back(PointDist(centers[i].x, centers[i].y, d));
    }

    //ordenar distancias de menor
    sort(distances.begin(), distances.end(),[ ]( const PointDist& lhs, const PointDist& rhs )
    {
        return lhs.d < rhs.d;
    });

    centers.clear();
    for(int i = 0 ; i < distances.size(); i++)
    {
        if(i < numPoints)
            pointsMiddle.push_back(Point2f(distances[i].x, distances[i].y));
        else
            centers.push_back(Point2f(distances[i].x, distances[i].y));
    }
    //order x
    sort(pointsMiddle.begin(), pointsMiddle.end(),[ ]( const Point2f& lhs, const Point2f& rhs )
    {
        return lhs.x < rhs.x;
    });

    return pointsMiddle;
}

vector<Point2f> Calibracion::orderPointsMiddle(Point2f p, vector<Point2f> middlePoints)
{
    vector<Point2f> result;
    /*sort(middlePoints.begin(), middlePoints.end(),[p]( const Point2f& lhs, const Point2f& rhs )
    {
        cout << p << endl;
        float d1 = sqrt(pow(p.x - rhs.x,2) + pow(p.y - rhs.y,2) );
         float d2 = sqrt(pow(p.x - lhs.x,2) + pow(p.y - lhs.y,2) );
        return d2 < d1;
    });*/

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

vector<Point2f> Calibracion::pointsMiddle(Point2f p1, Point2f p2, vector<Point2f> &centers)
{

    vector<Point2f> pointsMiddle;
    vector<Point2f> tempDelete;
    float m = (p2.y - p1.y)/(p2.x - p1.x + 0.0001);
    float y, x;
    float d = 0;
    float d1_x = 0;
    float d1_y = 0;
    float d2_x = 0;
    float d2_y = 0;
    int holgura = 5;


    for(int i=0; i<centers.size(); i++)
    {
        d1_x = abs(centers[i].x - p1.x);
        d1_y = abs(centers[i].y - p1.y);
        d2_x = abs(centers[i].x - p2.x);
        d2_y = abs(centers[i].y - p2.y);

        if( !( (d1_x < 1 && d1_y < 1)|| (d2_x < 1 && d2_y < 1)))
        {
            if(d1_x > d1_y)
            {
                y = m*(centers[i].x - p1.x) + p1.y;
                if(abs(y - centers[i].y) <= holgura)
                {
                    pointsMiddle.push_back(centers[i]);
                }
                else
                {
                    tempDelete.push_back(centers[i]);
                }

            }
            else
            {
                x = (centers[i].y - p1.y)/m + p1.x;

                if(abs(x - centers[i].x) <= holgura)
                {
                    pointsMiddle.push_back(centers[i]);
                }
                else
                {
                    tempDelete.push_back(centers[i]);
                }

            }
        }
        else
        {
            tempDelete.push_back(centers[i]);
        }
    }

    centers = tempDelete;

    return pointsMiddle;
}
