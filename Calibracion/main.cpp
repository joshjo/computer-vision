#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;

const double PI  =3.141592653589793238463;

Mat process (Mat obj);
int main( ) {
  
/*
    Mat image = imread("/home/liz/MEGAsync/Semestre-II/Imagenes-Loayza/Calibracion/sources/prueba2.png", 1 );

    if( !image.data )
    {
    printf( " No image data \n " );
    return -1;
    }

    Mat result = process(image);
    namedWindow( "Original", CV_WINDOW_AUTOSIZE );


    imshow( "Original", image );
    imshow( "Procesado", result );
    
    waitKey(0);
*/
//Video 

  int key = 0;
  //CvCapture* cap = cvCaptureFromCAM( 0 );
  CvCapture* cap = cvCaptureFromAVI("/home/liz/MEGAsync/Semestre-II/Imagenes-Loayza/Proyecto/computer-vision/camera-calibration/video/calibration_kinectv2.avi");
  IplImage* frame = cvQueryFrame( cap );
 
    // Check 
    if ( !cap ) 
    {
        fprintf( stderr, "Cannot open AVI!\n" );
        return 1;
    }
 
    // Get the fps, needed to set the delay
    int fps = ( int )cvGetCaptureProperty( cap, CV_CAP_PROP_FPS );
  
    // Create a window to display the video
    //cvNamedWindow( "video", CV_WINDOW_AUTOSIZE );
    namedWindow( "video", CV_WINDOW_AUTOSIZE );
    
    while( key != 'x' ) 
    {
        // get the image frame 
        frame = cvQueryFrame( cap );
        cout << "Image" << endl;
        // exit if unsuccessful
        if( !frame ) break;
         //Grayscale matrix
        Mat m = cv::cvarrToMat(frame);
        Mat proc = process(m);
        // display current frame 
        //cvShowImage( "video", frame); //frame
        imshow( "video", proc ); //frame
        // exit if user presses 'x'        
        key = cvWaitKey( 1000 / fps );

    }
    
    cvDestroyWindow( "video" );
    cvReleaseCapture( &cap );
 
  return 0;
}

Mat process (Mat image)
{
    Mat result = image;
    cvtColor( image, result, CV_BGR2GRAY); //CV_BGR2GRAY
   

    // Canny
    int erosion_type = MORPH_ELLIPSE;
    int erosion_size = 1;
    Mat	kernel =  getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
    
    GaussianBlur( result, result, Size(15, 15), 2, 2 );
    threshold(result, result, 95, 255, 1); //92 //115  //1 
    dilate(result, result, kernel);
    Canny(result, result, 0, 0 * 3, 3);
    

   vector<vector<Point> > contours;
   vector<Vec4i> hierarchy;
   findContours(result, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
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
            drawContours( image, contours, idx,  Scalar(255,0,255), CV_FILLED, 8, hierarchy );

        }
    }
   return image;
}