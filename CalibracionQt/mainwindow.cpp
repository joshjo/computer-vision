#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->gvOriginal->setScene(scene);

    //Init calibracion
   // Calibracion *objeto = new Calibracion("/home/liz/Vídeos/videos-calibracion/PadronAnillos_01.avi");
    //objeto->initProcess();

    //init video
    //VideoCapture *video = new VideoCapture("/home/liz/Vídeos/videos-calibracion/PadronAnillos_01.avi");

    //CvCapture* cap = cvCaptureFromAVI("/home/liz/Vídeos/videos-calibracion/PadronAnillos_01.avi");
   // IplImage* frame = cvQueryFrame( cap );

    VideoCapture video = VideoCapture("/home/liz/Vídeos/videos-calibracion/PadronAnillos_01.avi");
    Mat frame;
    while(video.isOpened())
    {
        video >> frame;
        if(!frame.empty())
            {
                // process and display frames
                pixmap = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888)));
                scene->addItem(pixmap);
            }
        qApp->processEvents();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
