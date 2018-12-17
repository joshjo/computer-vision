#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_4->hide();
    ui->gvDilate->hide();


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

    sceneResultado = new QGraphicsScene(this);
    ui->gvResultado->setScene(sceneResultado);

    objCal = new Calibracion();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    //namedWindow( "video", CV_WINDOW_AUTOSIZE );
   //Mat
    Mat matOriginal, matGray, matThresh, matDilate, matEdge, matSrc, matCopyDilate, matCircles;
    vector<Vec3f> points;
   // CvCapture* cap = cvCaptureFromAVI("/home/josue/Devel/videos/padron1.avi");
    CvCapture* cap = cvCaptureFromAVI("/home/liz/Vídeos/videos-calibracion/PadronAnillos_03.avi");
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
    //time
    unsigned t0, t1;
    //Reconocidos
    int countNoReconocidos = 0;
    int total = 0;
    while( key != 'x' )
    {
        // get the image frame
        frame = cvQueryFrame( cap );
        // exit if unsuccessful
        if( !frame ) break;

        //Proceso
        t0=clock();
/*
        matOriginal = imread("/home/liz/Imágenes/prueba.png", CV_LOAD_IMAGE_COLOR);
        matSrc = matOriginal.clone();
        matCircles = matOriginal.clone();
*/
        matOriginal = cv::cvarrToMat(frame);
        matSrc = cv::cvarrToMat(frame);
        matCircles = cvarrToMat(frame);

        matGray = objCal->grayScale(matOriginal);
        matThresh = objCal->thresholdMat(matGray);
        matCopyDilate = matThresh.clone();
        matEdge = objCal->findEdgeMat(matSrc, matThresh); // omitir este paso
        Data result = objCal->calculateCenters(matCircles, matCopyDilate);

        t1 = clock();

        double time = (double(t1-t0)/CLOCKS_PER_SEC);

        total++;
        if(result.numValids == 0)
            countNoReconocidos ++;
         ui->lblTime->setText(QString::number(time));
         ui->lblNoREconocidos->setText(QString::number(countNoReconocidos));
         ui->lblNumTotal->setText(QString::number(total));
         ui->lblReconodicos->setText(QString::number(total - countNoReconocidos));

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

        //Canny
        QImage imageEdge((unsigned char*) matEdge.data,matEdge.cols, matEdge.rows, QImage::Format_RGB888);
        imageEdge = imageEdge.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
        QPixmap pixmapEdge = QPixmap::fromImage(imageEdge);
        sceneEdge->addPixmap(pixmapEdge);
        ui->gvEdge->setScene(sceneEdge);

        //Centros
        QImage imageCenter((unsigned char*) result.matSrc.data,result.matSrc.cols, result.matSrc.rows, QImage::Format_RGB888);
        imageCenter = imageCenter.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
        QPixmap pixmapCenter = QPixmap::fromImage(imageCenter);
        sceneResultado->addPixmap(pixmapCenter);
        ui->gvResultado->setScene(sceneResultado);
        key = cvWaitKey( 1000 / fps );

    }
    cvReleaseCapture( &cap );

}
