#include "calibracion.h"

Calibracion::Calibracion(const char *filename)
{

}

Calibracion ::Calibracion()
{

}
void Calibracion::initProcess()
{
    //Mat image = imread("/home/liz/MEGAsync/Semestre-II/Imagenes-Loayza/Calibracion/sources/prueba2.png", 1 );

    //namedWindow("grayScale", WINDOW_AUTOSIZE);
    //grayScale(image);
}

Mat Calibracion::grayScale(Mat src)
{
    Mat greyColor;
    cvtColor(src, greyColor, CV_BGR2GRAY);
    //namedWindow("grayScale", WINDOW_AUTOSIZE);
    //imshow("grayScale", greyColor);
    return greyColor;
}

Mat Calibracion::thresholdMat(Mat src)
{
    Mat thresh;

    GaussianBlur(src, thresh,Size(9,9), 0, 0); //suavizar sectores que producen efecto sal pimeinta despues de umbral.

   // namedWindow("gauss", WINDOW_AUTOSIZE);
    //imshow("gauss", thresh);

    //bilateralFilter( src, thresh, 15, 80, 80); costoso

    adaptiveThreshold(thresh, thresh,255,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,11,6);

    //namedWindow("adapt", WINDOW_AUTOSIZE);
    //imshow("adapt", thresh);
    //return thresh;
    return thresh;
}

Mat Calibracion:: erodeMat(Mat src)
{
    //namedWindow("antes", WINDOW_AUTOSIZE);
    //imshow("antes", src);
    int erosion_type = MORPH_ELLIPSE;
    int erosion_size = 1;
    Mat kernel =  getStructuringElement( erosion_type,
                                         Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                         Point( erosion_size, erosion_size ) );
    erode(src, src, kernel);
    //namedWindow("erode", WINDOW_AUTOSIZE);
    //imshow("erode", src);
    return src;

}

Mat Calibracion::findEdgeMat(Mat original, Mat src)
{
    Mat copy = original.clone();
    Canny(src, src, 0, 0 * 3, 3);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    int idx = 0;
    //circularidad
    double indexCircularity = 0;
    double perimeter = 0;
    double area = 0;
    //radio
    double minorAxis = 0;
    double majorAxis = 0;

    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        // add filter num contour
        //circularidad
        area = contourArea(contours[idx]);
        perimeter = arcLength(contours[idx], true);
        indexCircularity = (4 * PI * area)/pow(perimeter,2);

        //rectangulos width y heigth debe ser casi iguales
        //Corregir
        Rect rect = boundingRect(contours[idx]);
        if(rect.width > rect.height){
            majorAxis = rect.width;
            minorAxis = rect.height;
        }
        else if (rect. height > rect.width){
            majorAxis = rect.height;
            minorAxis = rect.width;
        } else{
            majorAxis = rect.height;
            minorAxis = rect.width;
        }

        //cout << "circularidad: " <<  indexCircularity << " eje mayor: " << minorAxis/ majorAxis;
        if(indexCircularity > 0.78 && minorAxis/majorAxis > 0.70){
            drawContours( copy, contours, idx,  Scalar(255,0,255), CV_FILLED, 8, hierarchy );

        }
    }

    return copy;
}


vector<Vec3f> Calibracion::getCircles(Mat original, Mat src)
{
   // namedWindow("circles", WINDOW_AUTOSIZE);
    vector<Vec3f> circles;
    Mat copy = original.clone();
    Canny(src, src, 0, 0 * 3, 3);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    int idx = 0;
    //circularidad
    double indexCircularity = 0;
    double perimeter = 0;
    double area = 0;
    //radio
    double minorAxis = 0;
    double majorAxis = 0;
    Vec3f circle;

    for(; idx >= 0; idx = hierarchy[idx][0] )
    {
        // add filter num contour
        //circularidad
        area = contourArea(contours[idx]);
        perimeter = arcLength(contours[idx], true);
        indexCircularity = (4 * PI * area)/pow(perimeter,2);

        //rectangulos width y heigth debe ser casi iguales
        //Corregir
        Rect rect = boundingRect(contours[idx]);
        if(rect.width > rect.height){
            majorAxis = rect.width;
            minorAxis = rect.height;
        }
        else if (rect. height > rect.width){
            majorAxis = rect.height;
            minorAxis = rect.width;
        } else{
            majorAxis = rect.height;
            minorAxis = rect.width;
        }

        //cout << "circularidad: " <<  indexCircularity << " eje mayor: " << minorAxis/ majorAxis;
        if(indexCircularity > 0.78 && minorAxis/majorAxis > 0.70){
           // drawContours( copy, contours, idx,  Scalar(255,0,255), CV_FILLED, 8, hierarchy );
            Moments m = moments(contours[idx]);
                       circle[0] =  m.m10/m.m00; //X center
                       circle[1] =  m.m01/m.m00; //Y center
                       circle[2] =  rect.width / 2;//radio
           circles.push_back(circle);

        }

       // imshow("cicles", copy);
    }

    return circles;
}


 Mat Calibracion::calculateCenter(Mat original, Mat src)
 {
     Mat result = original.clone();
     ProcessCircles pc;
     vector<Vec3f> points = getCircles(original, src);

     for (auto & it : points) {
         pc.add(it);
     }

     vector<Vec4f> filter;
     for (auto & it : pc.circleGroups) {
         if (it.circles.size() == 2) {
             filter.push_back(it.getPoint());
         }
     }

     PatternMatrix pm(4, 3);
     pm.run(pc);

     //Vec4f vCircle;
//#pragma omp parallel num_threads(4) private (vCircle)
     for( size_t i = 0; i < filter.size(); i++ )
     {
         Vec4f vCircle = filter.at(i);
         //vCircle = filter.at(i);
         Point center(cvRound(vCircle[0]), cvRound(vCircle[1]));
         int r1 = cvRound(vCircle[3]);
         int r2 = cvRound(vCircle[2]);
         circle( result, center, r1, Scalar(0,0,255), 1, 8, 0 );
         circle( result, center, r2, Scalar(0,0,255), 1, 8, 0 );
     }

     return result;
 }
