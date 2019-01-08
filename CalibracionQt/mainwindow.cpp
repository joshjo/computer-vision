#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/stat.h>


bool doesExist (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->txtColumnas->setValidator(new QIntValidator);
    ui->txtFilas->setValidator(new QIntValidator);

    objCal = new Calibracion();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_calibrateBtn_clicked()
{
    if( ! verifyParameters()) {
        return;
    }
//    int num_imgs = 32;
//    float radius = 0.0243;
//    Mat img, matProcess, matGray, matThresh, matResult;

//    vector< vector< Point3f > > object_points;
//    vector< vector< Point2f > > image_points;

//    for (int k = 1; k <= num_imgs; k++) {
//        char img_file[100];
//        sprintf(img_file, "images/image_%02d.png", k);
//        if(!doesExist(img_file)) {
//            cout << "Can't find image" << img_file << endl;
//            continue;
//        }
//        img = imread(img_file, CV_LOAD_IMAGE_COLOR);

//        matGray = objCal->grayScale(img); //0.001824
//        matThresh = objCal->thresholdMat(img); //0.001669
//        matResult = matThresh.clone();
//        Data result = objCal->calculateCenters(matOriginal, matResult, rows, cols);
//        result.matSrc;

//        vector< Point3f > obj;
//        for (int i = 0; i < board_height; i++) {
//            for (int j = 0; j < board_width; j++){
//                obj.push_back(Point3f((float)j * radius, (float)i * radius, 0));
//            }
//        }

//    }
}


void MainWindow::on_pushButton_clicked()
{

    if(verifyParameters())
    {
        const char* name = nameFile.c_str();

        //QImage
        QImage image, imageGray, imageBinary, imageEdge, imageFinal;
        //Mats
        Mat matOriginal,matProcess, matGray, matThresh;
        Data result;
        CvCapture* cap = cvCaptureFromAVI(name);
        IplImage* frame = cvQueryFrame( cap );

        int key = 0;
        int fps = ( int )cvGetCaptureProperty( cap, CV_CAP_PROP_FPS );
        //Reconocidos
        int countRecognized = 0;
        int total = 0;
        //Time
        unsigned t0, t1;
        double time = 0, timeTotal = 0;
        if ( !cap )
        {
            fprintf( stderr, "Cannot open AVI!\n" );
            return;
        }

        //namedWindow("josue", WINDOW_AUTOSIZE);
        //namedWindow("liz", WINDOW_AUTOSIZE);
        while( key != 'x' )
        {
            frame = cvQueryFrame( cap );
            if( !frame ) break;
            matOriginal = cv::cvarrToMat(frame);
            result.matSrc = matOriginal.clone();
            result.matContours = matOriginal.clone();


            t0=clock();
            objCal->grayScale(matGray, matOriginal);
            objCal->thresholdMat(matThresh, matGray);
            objCal->calculateCenters(result, matThresh.clone(), rows, cols);
            t1 = clock();

            time = (double(t1-t0)/CLOCKS_PER_SEC);
            total++;


            if(result.numValids == (rows * cols)){
                timeTotal += time;
                countRecognized++;
            }

            ui->lblTime->setText(QString::number(time));
            ui->lblNoREconocidos->setText(QString::number(total - countRecognized));
            ui->lblNumTotal->setText(QString::number(total));
            ui->lblReconodicos->setText(QString::number(countRecognized));

            //imshow("josue", result.matSrc);
            //imshow("liz", result.matContours);

            image = QImage(matOriginal.data,matOriginal.cols, matOriginal.rows, QImage::Format_RGB888);
            image = image.scaled(ui->lblOriginal->width(), ui->lblOriginal->height(), Qt::KeepAspectRatio);
            ui->lblOriginal->setPixmap(QPixmap::fromImage(image));

            //Gray
            imageGray = QImage( matGray.data,matGray.cols, matGray.rows, QImage::Format_Grayscale8);
            imageGray = imageGray.scaled(ui->lblGrayScale->width(), ui->lblGrayScale->height(), Qt::KeepAspectRatio);
            ui->lblGrayScale->setPixmap(QPixmap::fromImage(imageGray));

            //umbral
            imageBinary = QImage(matThresh.data,matThresh.cols, matThresh.rows, QImage::Format_Grayscale8);
            imageBinary = imageBinary.scaled(ui->lblThreshold->width(), ui->lblThreshold->height(), Qt::KeepAspectRatio);
            ui->lblThreshold->setPixmap(QPixmap::fromImage(imageBinary));

            //Canny
            imageEdge = QImage(result.matContours.data,result.matContours.cols, result.matContours.rows, QImage::Format_RGB888);
            imageEdge = imageEdge.scaled(ui->lblContour->width(), ui->lblContour->height(), Qt::KeepAspectRatio);
            ui->lblContour->setPixmap(QPixmap::fromImage(imageEdge));

            //Centros
            imageFinal = QImage( result.matSrc.data,result.matSrc.cols, result.matSrc.rows, QImage::Format_RGB888);
            imageFinal = imageFinal.scaled(ui->lblFinal->width(), ui->lblFinal->height(), Qt::KeepAspectRatio);
            ui->lblFinal->setPixmap(QPixmap::fromImage(imageFinal));

            key = cvWaitKey( 1000 / fps );
            //release
            matOriginal.release();
            matProcess.release();
            matGray.release();
            matThresh.release();


        }
        cvReleaseCapture( &cap );
        ui->lblAvgTime->setText(QString::number(timeTotal/countRecognized));
    }

}

bool MainWindow::verifyParameters()
{
    if(nameFile.length() <= 0)
    {
        QMessageBox::information(this, tr("No se puede abrir video."), "Error en el nombre del video.");
        return false;
    }
    else if(ui->txtFilas->text().length() <= 0)
    {
        QMessageBox::information(this, tr("Notificación."), "Ingrese número de filas del patrón.");
        return false;
    }
    else if(ui->txtColumnas->text().length() <= 0)
    {
        QMessageBox::information(this, tr("Notificación."), "Ingrese número de columnas del patrón");
        return false;
    }
    else
    {
        rows = ui->txtFilas->text().toInt();
        cols = ui->txtColumnas->text().toInt();
        return true;
    }


}

void MainWindow::on_openVideoBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Abrir video"), "",
                                                    tr("video (*.avi)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Error al abrir el video"),
                                     file.errorString());
            nameFile = "";
            ui->lblVideo->setText("");
            return;
        }
        else
        {
            nameFile = fileName.toStdString();
            ui->lblVideo->setText(fileName);
        }
    }
}
