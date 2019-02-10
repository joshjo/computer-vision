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

    GaussianBlur(src, thresh,Size(5,5), 2, 2);
    //   Mat binary = thresh.clone();
    adaptiveThreshold(thresh, thresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 21, 7); //41-12/ 15-5

    /* Revisar , no reconoce contours.
     int blockSize=21;
     int threshold=10;
  //   Mat binary = thresh.clone();
     int nl = thresh.rows; // number of lines
     int nc = thresh.cols; // total number of elements per line
     // compute integral image
     cv::Mat iimage;
     cv::integral(src, iimage, CV_32S);
     //for each row
     int halfSize = blockSize/2;
     for(int j=halfSize; j<nl - halfSize -1;j++ ){
         // get the address of row j
         uchar* data = thresh.ptr<uchar>(j);
         int* idata1 = iimage.ptr<int>(j-halfSize); // 滑动窗口上边
         int* idata2 = iimage.ptr<int>(j+halfSize+1); // 滑动窗口下边
         //for pixel of a line
         for(int i=halfSize; i<nc-halfSize-1;i++){
             //compute pix_mean
             int pix_mean = (idata2[i+halfSize+1]-idata2[i-halfSize]-idata1[i+halfSize+1]
                     +idata1[i-halfSize])/(blockSize*blockSize);
             //apply adaptive threshold
             if(data[i]>(pix_mean-threshold))
                 data[i] = 255;
             else
                 data[i]  =0;
         }
     }
     // add white border
     for(int j=0;j<halfSize;j++){
         uchar *data = thresh.ptr<uchar>(j);
         for(int i=0; i<thresh.cols;i++)
             data[i] = 0;
     }
     for(int j=thresh.rows-halfSize-1;j<thresh.rows;j++){
         uchar * data = thresh.ptr<uchar>(j);
         for(int i=0; i<thresh.cols;i++){

             if(data[i]>170)
                 data[i] = 0;
             else
                 data[i]  =255;
         }
     }
     for(int j=halfSize;j<nl-halfSize-1;j++){
         uchar* data = thresh.ptr<uchar>(j);
         for(int i=0; i<halfSize;i++)
             data[i] = 0;
         for(int i=thresh.cols-halfSize-1;i<thresh.cols;i++)
             data[i] = 0;
     }



*/
}

void Calibracion::thresholdMatv2(Mat &thresh, Mat src)
{

    GaussianBlur(src, thresh,Size(5,5), 2, 2);
    adaptiveThreshold(thresh, thresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 41, 7); //41-12/ 15-5


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

    findContours( srcThresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    double indexCircularity = 0;
    RotatedRect rect;
    RotatedRect rectChild;
    vector<Point2f> points;
    // Index to adaptative
    for(; idx >= 0 ; idx = hierarchy[idx][0] )
    {
        if( contours[idx].size() > 4 && !(hierarchy[idx][2] == -1 &&  hierarchy[idx][3] == -1 ))
        {
            indexCircularity = (4 * PI * contourArea(contours[idx]))/pow(arcLength(contours[idx], true),2);
            if( indexCircularity > 0.7 && contours[hierarchy[idx][2]].size() > 4)
            {
                rect = fitEllipse(contours[idx]);
                rectChild = fitEllipse(contours[hierarchy[idx][2]]);
                drawContours( resultData.matContours, contours, (int)idx,  Scalar(255,0,255), CV_INTER_LINEAR, 8, hierarchy );
                drawContours( resultData.matContours, contours, hierarchy[idx][2],  Scalar(0,0,255), CV_INTER_LINEAR, 8, hierarchy );

                //mean center
                // double xCenter = (rect.center.x + rectChild.center.x)/2;
                // double yCenter = (rect.center.y + rectChild.center.y)/2;
                float xCenter = rect.center.x ;
                float yCenter = rect.center.y ;
                points.push_back(Point2f( xCenter, yCenter));

            }
        }

    }

    vector<Point2f> ringsSorted = points;
    int verifyCount = 0;

    if( points.size() == rows*cols)
    {
        verifyCount = orderPoints(rows, cols, ringsSorted, points);

        resultData.centers = ringsSorted;
        if(verifyCount ==  rows*cols)
        {
            Scalar color(23,190,187);
            circle(resultData.matSrc, ringsSorted[0], 2, color, -1, 8, 0);
            putText(resultData.matSrc,to_string(1), ringsSorted[0], FONT_HERSHEY_SCRIPT_COMPLEX, 1 , color,  1);
            for (size_t i = 1; i < ringsSorted.size() ; ++i){
                circle(resultData.matSrc, ringsSorted[i], 2, color, -1, 8, 0);
                putText(resultData.matSrc,to_string(i+1), ringsSorted[i], FONT_HERSHEY_SCRIPT_COMPLEX, 1 , color,  1); // Line Thickness (Optional)
                line(resultData.matSrc,  Point( ringsSorted[i-1].x, ringsSorted[i-1].y),  ringsSorted[i], color, 2);

            }
        }
    }
    resultData.numValids = verifyCount;
}

int Calibracion::orderPoints(int rows, int cols, vector<Point2f> &ringsSorted, vector<Point2f> centers)
{
    int countResult = 0;
    vector<Point2f> points;
    vector<Point2f> centerTemp;
    vector<Point2f> tempSortedCorner;


    convexHull(Mat(centers), points, true);
    getCorners(points, centers , tempSortedCorner);

    //Add corners in patterns
    vector<Point2f> middlePoints;
    ringsSorted[0] = tempSortedCorner[0];
    ringsSorted[4] = tempSortedCorner[1];
    ringsSorted[15] = tempSortedCorner[2];
    ringsSorted[19] = tempSortedCorner[3];
    countResult = 4;
    //Eliminar puntos
    for(int i=0; i<centers.size(); i++)
    {
        if(!((abs(centers[i].x - ringsSorted[0].x) < 1 && abs(centers[i].y - ringsSorted[0].y) < 1)
             || (abs(centers[i].x - ringsSorted[4].x) < 1 && abs(centers[i].y - ringsSorted[4].y) < 1)
             || (abs(centers[i].x - ringsSorted[15].x) < 1 && abs(centers[i].y - ringsSorted[15].y) < 1)
             || (abs(centers[i].x - ringsSorted[19].x) < 1 && abs(centers[i].y - ringsSorted[19].y) < 1)))
        {
            centerTemp.push_back(centers[i]);
        }

    }

    centers = centerTemp;

    //Get middle points
    /*Row: 0-5*/
    middlePoints = pointsMiddle(tempSortedCorner[0], tempSortedCorner[1], centers); //3
    countResult += 3;
    if(middlePoints.size() < 3)
        return countResult;
    middlePoints  = orderPointsMiddle(tempSortedCorner[0], middlePoints);
    for (int i = 0; i < middlePoints.size(); i++)
    {
        ringsSorted[i+1] = middlePoints[i];
    }

    /**/
    middlePoints = pointsMiddle(tempSortedCorner[0], tempSortedCorner[2], centers); //2
    countResult += 2;
    if(middlePoints.size() < 2)
        return countResult;
    middlePoints  = orderPointsMiddle(tempSortedCorner[0], middlePoints);
    for (int i = 0; i < middlePoints.size(); i++)
    {
        ringsSorted[(i+1)*5] = middlePoints[i];
    }

    /*Row: 15-19*/
    middlePoints = pointsMiddle(tempSortedCorner[2], tempSortedCorner[3], centers); //3
    countResult += 3;
    if(middlePoints.size() < 3)
        return countResult;
    middlePoints  = orderPointsMiddle(tempSortedCorner[2], middlePoints);
    for (int i = 0; i < middlePoints.size(); i++)
    {
        ringsSorted[i+16] = middlePoints[i];
    }

    middlePoints = pointsMiddle(tempSortedCorner[1], tempSortedCorner[3], centers);//2 points
    countResult += 2;
    if(middlePoints.size() < 2)
        return countResult;
    middlePoints  = orderPointsMiddle(tempSortedCorner[1], middlePoints);

    for (int i = 0; i < middlePoints.size(); i++)
    {
        ringsSorted[9+(i*5)] = middlePoints[i];
    }

    /*Row: 5-9*/
    middlePoints = pointsMiddle(ringsSorted[5], ringsSorted[9], centers); //3
    countResult += 3;
    if(middlePoints.size() < 3)
        return countResult;
    middlePoints  = orderPointsMiddle(ringsSorted[5], middlePoints);

    for (int i = 0; i < middlePoints.size(); i++)
    {
        ringsSorted[i+6] = middlePoints[i];
    }
    /*Row: 10-14*/
    middlePoints = pointsMiddle(ringsSorted[10], ringsSorted[14], centers); //3
    countResult += 3;
    if(middlePoints.size() < 3)
        return countResult;
    middlePoints  = orderPointsMiddle(ringsSorted[10], middlePoints);

    for (int i = 0; i < middlePoints.size(); i++)
    {
        ringsSorted[i+11] = middlePoints[i];
    }

    return  countResult;
}

void Calibracion::getCorners(vector<Point2f> points, vector<Point2f> centers, vector<Point2f> &corners)
{
    points.push_back(points[0]);
    points.push_back(points[1]);

    float x_eval = points[0].x;
    float y_eval = points[0].y;
    float x_holgura = 0.000001;
    float m = (points[1].y - y_eval)/(points[1].x - x_eval + x_holgura);
    float holgura = 10;
    float holgura_min = 3;
    float y = 0;
    float x = 0;

    for(int i=1; i < points.size(); i++)
    {
        if(abs(points[i].x - x_eval) > abs(points[i].y - y_eval))
        {
            y = m*(points[i].x - x_eval) + y_eval;

            if(abs(y - points[i].y) >= holgura)
            {
                corners.push_back(points[i-1]);
                x_eval = points[i-1].x;
                y_eval = points[i-1].y;
                m = (points[i].y - y_eval)/(points[i].x - x_eval + x_holgura);
            }

        }
        else
        {
            x = (points[i].y - y_eval)/m + x_eval;

            if(abs(x - points[i].x) >= holgura)
            {
                corners.push_back(points[i-1]);
                x_eval = points[i-1].x;
                y_eval = points[i-1].y;
                m = (points[i].y - y_eval)/(points[i].x - x_eval + x_holgura);
            }

        }

    }

    x_eval = corners[0].x;
    y_eval = corners[0].y;
    m = (corners[1].y - y_eval)/(corners[1].x - x_eval + x_holgura);
    int countDist1 = 0;
    int countDist2 = 0;

    for(int i=0; i<centers.size(); i++)
    {
        if(abs(centers[i].x - x_eval) > abs(centers[i].y - y_eval))
        {
            y = m*(centers[i].x - x_eval) + y_eval;

            if(abs(y - centers[i].y) >= holgura)
            {
                countDist1++;
            }

        }
        else
        {
            x = (centers[i].y - y_eval)/m + x_eval;

            if(abs(x - centers[i].x) >= holgura)
            {
                countDist1++;
            }

        }
    }

    x_eval = corners[1].x;
    y_eval = corners[1].y;
    m = (corners[2].y - y_eval)/(corners[2].x - x_eval + x_holgura);

    for(int i=0; i<centers.size(); i++)
    {
        if(abs(centers[i].x - x_eval) > abs(centers[i].y - y_eval))
        {
            y = m*(centers[i].x - x_eval) + y_eval;

            if(abs(y - centers[i].y) >= holgura)
            {
                countDist2++;
            }

        }
        else
        {
            x = (centers[i].y - y_eval)/m + x_eval;

            if(abs(x - centers[i].x) >= holgura)
            {
                countDist2++;
            }

        }
    }

    if(countDist1 < countDist2)
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

vector<Point2f> Calibracion::orderPointsMiddle(Point2f p, vector<Point2f> middlePoints)
{
    vector<Point2f> result;

    if(middlePoints.size() == 3 )
    {
        double d1 = sqrt(pow(p.x - middlePoints[0].x,2) + pow(p.y - middlePoints[0].y,2) );
        double d2 = sqrt(pow(p.x - middlePoints[1].x,2) + pow(p.y - middlePoints[1].y,2) );
        double d3 = sqrt(pow(p.x - middlePoints[2].x,2) + pow(p.y - middlePoints[2].y,2) );

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
        double d1 = sqrt(pow(p.x - middlePoints[0].x,2) + pow(p.y - middlePoints[0].y,2) );
        double d2 = sqrt(pow(p.x - middlePoints[1].x,2) + pow(p.y - middlePoints[1].y,2) );

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
    float m = (p2.y - p1.y)/(p2.x - p1.x + 0.000001);
    float y, x;
    float d = 0;
    float dist1_x = 0;
    float dist1_y = 0;
    float dist2_x = 0;
    float dist2_y = 0;
    int holgura = 4;
    int holguramin = 2;


    for(int i=0; i<centers.size(); i++)
    {
        dist1_x = abs(centers[i].x - p1.x);
        dist1_y = abs(centers[i].y - p1.y);
        dist2_x = abs(centers[i].x - p2.x);
        dist2_y = abs(centers[i].y - p2.y);

        if( !( (dist1_x < 1 && dist1_y < 1)|| (dist2_x < 1 && dist2_y < 1)))
        {
            if(dist1_x > dist1_y)
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

    }

    centers = tempDelete;
    return pointsMiddle;
}
