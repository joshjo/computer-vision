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

    namedWindow("grayScale", WINDOW_AUTOSIZE);
    //grayScale(image);
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
    //Gaussian blur
    GaussianBlur(src, thresh,Size(3,3), 2, 2);
    adaptiveThreshold(thresh, thresh,200,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV,3,2);
    return thresh;
}

Mat Calibracion:: dilateMat(Mat src)
{
    int erosion_type = MORPH_ELLIPSE;
    int erosion_size = 1;
    Mat kernel =  getStructuringElement( erosion_type,
                                           Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                           Point( erosion_size, erosion_size ) );
    dilate(src, src, kernel);
    return src;

}

Mat Calibracion::findEdgeMat(Mat original, Mat src)
{
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
            if(indexCircularity > 0.78 && minorAxis/majorAxis > 0.50){
                drawContours( original, contours, idx,  Scalar(255,0,255), CV_FILLED, 8, hierarchy );

            }
        }
    return original;
}
