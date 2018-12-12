#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->gvOriginal->setScene(scene);

    sceneGray = new QGraphicsScene(this);
    ui->gvGrayScale->setScene(sceneGray);

    sceneBinary = new QGraphicsScene(this);
    ui->gvUmbral->setScene(sceneBinary);

    sceneDilate = new QGraphicsScene(this);
    ui->gvDilate->setScene(sceneDilate);

    sceneEdge = new QGraphicsScene(this);
    ui->gvEdge->setScene(sceneEdge);


    objCal = new Calibracion();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    namedWindow( "video", CV_WINDOW_AUTOSIZE );
   //Mat
    Mat matOriginal, matGray, matThresh, matDilate, matEdge, matSrc;
    CvCapture* cap = cvCaptureFromAVI("/home/liz/Vídeos/videos-calibracion/PadronAnillos_01.avi");
    IplImage* frame = cvQueryFrame( cap );
    int key = 0;
    // Check
    if ( !cap )
    {
        fprintf( stderr, "Cannot open AVI!\n" );
        return;
    }
    int i = 0;
    // Get the fps, needed to set the delay
    int fps = ( int )cvGetCaptureProperty( cap, CV_CAP_PROP_FPS );

    while( key != 'x' )
    {
        // get the image frame
        frame = cvQueryFrame( cap );
        // exit if unsuccessful
        if( !frame ) break;

        matOriginal = cv::cvarrToMat(frame);
        matSrc = cv::cvarrToMat(frame);
        matGray = objCal->grayScale(matOriginal);
        matThresh = objCal->thresholdMat(matGray);
        matDilate = objCal->dilateMat(matThresh);
        matEdge = objCal->findEdgeMat(matSrc, matDilate);


        //Original
        //QImage image((unsigned char*) matOriginal.data, matOriginal.cols, matOriginal.rows, QImage::Format_RGB888);
        QImage image((unsigned char*) matOriginal.data,matOriginal.cols, matOriginal.rows, QImage::Format_RGB888);
        image = image.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
        QPixmap pixmap = QPixmap::fromImage(image);
        scene->addPixmap(pixmap);
        ui->gvOriginal->setScene(scene);

        //Gray
        QImage imageGray((unsigned char*) matGray.data,matGray.cols, matGray.rows, QImage::Format_Grayscale8);
        imageGray = imageGray.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
        QPixmap pixmapGray = QPixmap::fromImage(imageGray);
        sceneGray->addPixmap(pixmapGray);
        ui->gvGrayScale->setScene(sceneGray);

        //umbral
        QImage imageBinary((unsigned char*) matThresh.data,matThresh.cols, matThresh.rows, QImage::Format_Grayscale8);
        imageBinary = imageBinary.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
        QPixmap pixmapBinary = QPixmap::fromImage(imageBinary);
        sceneBinary->addPixmap(pixmapBinary);
        ui->gvUmbral->setScene(sceneBinary);

        //Dilate
        QImage imageDilate((unsigned char*) matDilate.data,matDilate.cols, matDilate.rows, QImage::Format_Grayscale8);
        imageDilate = imageDilate.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
        QPixmap pixmapDilate = QPixmap::fromImage(imageDilate);
        sceneDilate->addPixmap(pixmapDilate);
        ui->gvDilate->setScene(sceneDilate);

        //Canny
        QImage imageEdge((unsigned char*) matEdge.data,matEdge.cols, matEdge.rows, QImage::Format_RGB888);
        imageEdge = imageEdge.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
        QPixmap pixmapEdge = QPixmap::fromImage(imageEdge);
        sceneEdge->addPixmap(pixmapEdge);
        ui->gvEdge->setScene(sceneEdge);

        if(i == 0)
        {
            i = 1;
            imshow( "video", matThresh ); //frame
        }
        key = cvWaitKey( 1000 / fps );

    }
    //cvDestroyWindow( "video" );
    cvReleaseCapture( &cap );

}
