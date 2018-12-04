#include "processCircles.hpp"

#include <iostream>

using namespace std;
using namespace cv;

const double PI  =3.141592653589793238463;
int erosion_type = MORPH_ELLIPSE;
int erosion_size = 1;

Mat process (Mat obj);
vector<Vec3f> getCircles(Mat image);
int main(int argc, char* argv[]) {
 /*

    Mat image = imread("/home/liz/MEGAsync/Semestre-II/Imagenes-Loayza/Calibracion/sources/prueba2.png", 1 );

    if( !image.data )
    {
    printf( " No image data \n " );
    return -1;
    }

    //Mat result = process(image);
    namedWindow( "Original", CV_WINDOW_AUTOSIZE );
    namedWindow( "Procesado", CV_WINDOW_AUTOSIZE );
    imshow( "Original", image );
    vector<Vec3f> points = getCircles(image);
        for( size_t i = 0; i < points.size(); i++ )
        {
            Vec3f vCircle = points.at(i);

            Point center(cvRound(vCircle[0]), cvRound(vCircle[1]));
            int radius = cvRound(vCircle[2]);
            // draw the circle center
            //circle( image, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // draw the circle outline
            circle( image, center, radius, Scalar(0,0,255), 1, 8, 0 );
        }

    imshow( "Procesado", image );

    waitKey(0);
*/
//Video

    if (argc < 2) {
        // Tell the user how to run the program
        cerr << "Usage: " << argv[0] << " File/path" << endl;
        /* "Usage messages" are a conventional way of telling the user
         * how to run a program if they enter the command incorrectly.
         */
        return 1;
    }

    int key = 0;
    //CvCapture* cap = cvCaptureFromCAM( 0 );
    CvCapture* cap = cvCaptureFromAVI(argv[1]);
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
        //Mat proc = process(m);

        ProcessCircles pc;

        // pc.add()
        vector<Vec3f> points = getCircles(m);

        for (auto & it : points) {
            pc.add(it);
        }

        vector<Vec4f> filter;
        for (auto & it : pc.circleGroups) {
            if (it.circles.size() == 2) {
                filter.push_back(it.getPoint());
            }
        }


        for( size_t i = 0; i < filter.size(); i++ )
        {
            Vec4f vCircle = filter.at(i);

            Point center(cvRound(vCircle[0]), cvRound(vCircle[1]));
            int r1 = cvRound(vCircle[3]);
            int r2 = cvRound(vCircle[2]);
            circle( m, center, r1, Scalar(0,0,255), 1, 8, 0 );
            circle( m, center, r2, Scalar(0,0,255), 1, 8, 0 );
        }
        // for( size_t i = 0; i < points.size(); i++ )
        // {
        //     Vec3f vCircle = points.at(i);

        //     Point center(cvRound(vCircle[0]), cvRound(vCircle[1]));
        //     int radius = cvRound(vCircle[3]);
        //     circle( m, center, radius, Scalar(0,0,255), 1, 8, 0 );
        // }
        // display current frame
        //cvShowImage( "video", frame); //frame
        imshow( "video", m ); //frame
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
    Mat kernel =  getStructuringElement( erosion_type,
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

vector<Vec3f> getCircles(Mat image)
{
    Mat result;
    vector<Vec3f> circles;
    cvtColor( image, result, CV_BGR2GRAY); //CV_BGR2GRAY

    Mat kernel =  getStructuringElement( erosion_type,
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
        Vec3f circle;
        Rect rect = boundingRect(contours[idx]);

        //circularidad
        area = contourArea(contours[idx]);
        perimeter = arcLength(contours[idx], true);
        indexCircularity = (4 * PI * area)/pow(perimeter,2);

        //rectangulos width y heigth debe ser casi iguales
        //Corregir

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

        //momentos // momentos hu
        if(indexCircularity > 0.78 && minorAxis/majorAxis > 0.50){
            //drawContours( image, contours, idx,  Scalar(255,0,255), CV_FILLED, 8, hierarchy );
            Moments m = moments(contours[idx]);
            circle[0] =  m.m10/m.m00; //X center
            circle[1] =  m.m01/m.m00; //Y center
            circle[2] =  rect.width / 2;//radio
            circles.push_back(circle);
        }
    }
   return circles;
}
