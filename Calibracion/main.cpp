#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
 
using namespace std;
using namespace cv;

Mat process (Mat obj);
int main( ) {
  
/*
    Mat image = imread("/home/liz/MEGAsync/Semestre-II/Imagenes-Loayza/Calibracion/sources/prueba.png", 1 );

    if( !image.data )
    {
    printf( " No image data \n " );
    return -1;
    }

    Mat gray_image, binary_image, cany;
    cvtColor( image, gray_image, CV_BGR2GRAY); //CV_BGR2GRAY
   

    // Canny
    int erosion_type = MORPH_ELLIPSE;
    int erosion_size = 1;
    Mat	kernel =  getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
    //Mat erode;
	//erode(gray_image, gray_image, kernel);

    namedWindow( "Original", CV_WINDOW_AUTOSIZE );
    namedWindow( "Gray image", CV_WINDOW_AUTOSIZE );
    namedWindow( "Gaussian", CV_WINDOW_AUTOSIZE );
    namedWindow( "Umbral binary", CV_WINDOW_AUTOSIZE );
    namedWindow( "Canny", CV_WINDOW_AUTOSIZE );
    namedWindow( "Erode Cany", CV_WINDOW_AUTOSIZE );
    namedWindow( "Houg", CV_WINDOW_AUTOSIZE );


    imshow( "Original", image );
    imshow( "Gray image", gray_image );
    GaussianBlur( gray_image, gray_image, Size(15, 15), 2, 2 );
    imshow(  "Gaussian", gray_image );
    threshold(gray_image, binary_image, 70, 255, 1); //92 //115
    imshow(  "Umbral binary", binary_image );
    Canny(binary_image, cany, 0, 0 * 3, 3);
    imshow( "Canny", cany );
    dilate(binary_image, binary_image, kernel);
    Canny(binary_image, binary_image, 0, 0 * 3, 3);
    imshow( "Erode Cany", binary_image );
   
    

    //hough
    vector<Vec3f> circles;
    HoughCircles(binary_image, circles, HOUGH_GRADIENT, 1,
                 binary_image.rows/16,  // change this value to detect circles with different distances to each other
                 100, 30, 1, 30 // change the last two parameters
            // (min_radius & max_radius) to detect larger circles
    );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle( binary_image, center, 1, Scalar(0,100,100), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle( binary_image, center, radius, Scalar(255,0,255), 3, LINE_AA);
    }

    imshow( "Houg", binary_image );
    

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
    int erosion_size = 2;
    Mat	kernel =  getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
    
    GaussianBlur( result, result, Size(15, 15), 2, 2 );
    threshold(result, result, 90, 255, 1); //92 //115
    dilate(result, result, kernel);
    Canny(result, result, 0, 0 * 3, 3);
    

    //Houg
     //hough
    vector<Vec3f> circles;
    HoughCircles(result, circles, HOUGH_GRADIENT, 1,
                 1,  // change this value to detect circles with different distances to each other
                 100, 30, 1, 30 // change the last two parameters
            // (min_radius & max_radius) to detect larger circles
    );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle( image, center, 1, Scalar(0,100,100), 1, LINE_AA);
        // circle outline
        int radius = c[2];
        circle( image, center, radius, Scalar(255,0,255), 1, LINE_AA);
    }
    return image;
    //return result;
}