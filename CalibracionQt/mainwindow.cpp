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


    scene = new QGraphicsScene(this);
    ui->gvOriginal->setScene(scene);

    sceneGray = new QGraphicsScene(this);
    ui->gvGrayScale->setScene(sceneGray);

    sceneBinary = new QGraphicsScene(this);
    ui->gvUmbral->setScene(sceneBinary);

    sceneEdge = new QGraphicsScene(this);
    ui->gvEdge->setScene(sceneEdge);

    sceneResultado = new QGraphicsScene(this);
    ui->gvResultado->setScene(sceneResultado);

    ui->txtColumnas->setValidator(new QIntValidator);
    ui->txtFilas->setValidator(new QIntValidator);

    objCal = new Calibracion();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calibration(vector<Mat> & calibrateFrames) {
    float radius = 0.0243;
    Mat img, matGray, matThresh, matResult;

    vector< vector< Point3f > > object_points;
    vector< vector< Point2f > > image_points;
    int number;

    char img_file[100];



    for(int i = 0; i < calibrateFrames.size(); i++) {
        img = calibrateFrames[i];

        Calibracion cal;
        matGray = cal.grayScale(img);
        matThresh = cal.thresholdMat(matGray);
        matResult = matThresh.clone();
        Data result = cal.calculateCenters(img, matResult, rows, cols);

        vector< Point3f > obj;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                obj.push_back(Point3f((float)j * radius, (float)i * radius, 0));
            }
        }

        if(result.numValids == (rows * cols)){
            object_points.push_back(obj);
            image_points.push_back(cal.corners);
        }
    }

    ui->calibrationFramesLabel->setText(QString::number(image_points.size()));
    if (image_points.size() > 10) {
        Mat K;
        Mat D;
        vector< Mat > rvecs, tvecs;
        int flag = 0;
        flag |= CV_CALIB_FIX_K4;
        flag |= CV_CALIB_FIX_K5;
        calibrateCamera(object_points, image_points, img.size(), K, D, rvecs, tvecs, flag);
        cout << "K: " << K << endl;
        cout << "D: " << D << endl;

//        ostringstream oss;
//        oss << "D = " << D << endl;
        string distortion = "";

        QString qstr = "x";

        for(int i = 0; i < D.rows; i++)
        {
            const double* Mi = D.ptr<double>(i);
            for(int j = 0; j < D.cols; j++) {
                qstr += QString::number(Mi[j]) + " \n";
            }
        }

        ui->fxLabel->setText( QString::number(K.at<double>(0, 0)) );
        ui->fyLabel->setText( QString::number(K.at<double>(1, 1)) );
        ui->cxLabel->setText( QString::number(K.at<double>(0, 2)) );
        ui->cyLabel->setText( QString::number(K.at<double>(1, 2)) );
        ui->distortionLabel->setText( qstr );
    }
}


void MainWindow::on_calibrateBtn_clicked()
{
}


void MainWindow::on_pushButton_clicked()
{

    if(verifyParameters())
    {
        const char* name = nameFile.c_str();

        Mat matOriginal, matProcess, matGray, matThresh, matResult;
        CvCapture* cap = cvCaptureFromAVI(name);

        int totalFrames = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_COUNT) / 10;

        int step = totalFrames / 35;

        cout << "totalFrames: " << totalFrames << endl;

        IplImage* frame = cvQueryFrame( cap );

        int key = 0;
        int i = 0;
        int fps = ( int )cvGetCaptureProperty( cap, CV_CAP_PROP_FPS );

        //Reconocidos
        int countNoReconocidos = 0;
        int count = 0;
        //Time
        unsigned t0, t1, tinit;
        double time;
        if ( !cap )
        {
            fprintf( stderr, "Cannot open AVI!\n" );
            return;
        }



        while( key != 'x' )
        {
            frame = cvQueryFrame( cap );
            if( !frame ) break;
            matOriginal = cv::cvarrToMat(frame);
            t0=clock();
            matGray = objCal->grayScale(matOriginal); //0.001824
            matThresh = objCal->thresholdMat(matGray); //0.001669
            matResult = matThresh.clone();
            Data result = objCal->calculateCenters(matOriginal, matResult, rows, cols); //0.006227
            t1 = clock();
            time = (double(t1-t0)/CLOCKS_PER_SEC);

            if(result.numValids != (rows * cols)){
                countNoReconocidos ++;
            }

            ui->lblTime->setText(QString::number(time));
            ui->lblNoREconocidos->setText(QString::number(countNoReconocidos));
            ui->lblNumTotal->setText(QString::number(count));
            ui->lblReconodicos->setText(QString::number(count - countNoReconocidos));

            // imshow("josue", result.matSrc);

//            QImage image((unsigned char*) matOriginal.data,matOriginal.cols, matOriginal.rows, QImage::Format_RGB888);
//            image = image.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
//            QPixmap pixmap = QPixmap::fromImage(image);
//            scene->addPixmap(pixmap);
//            ui->gvOriginal->setScene(scene);

//            //Gray
//            QImage imageGray((unsigned char*) matGray.data,matGray.cols, matGray.rows, QImage::Format_Grayscale8);
//            imageGray = imageGray.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
//            QPixmap pixmapGray = QPixmap::fromImage(imageGray);
//            sceneGray->addPixmap(pixmapGray);
//            ui->gvGrayScale->setScene(sceneGray);

//            //umbral
//            QImage imageBinary((unsigned char*) matThresh.data,matThresh.cols, matThresh.rows, QImage::Format_Grayscale8);
//            imageBinary = imageBinary.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
//            QPixmap pixmapBinary = QPixmap::fromImage(imageBinary);
//            sceneBinary->addPixmap(pixmapBinary);
//            ui->gvUmbral->setScene(sceneBinary);

//            //Canny
//            QImage imageEdge((unsigned char*) result.matContours.data,result.matContours.cols, result.matContours.rows, QImage::Format_RGB888);
//            imageEdge = imageEdge.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
//            QPixmap pixmapEdge = QPixmap::fromImage(imageEdge);
//            sceneEdge->addPixmap(pixmapEdge);
//            ui->gvEdge->setScene(sceneEdge);

//            //Centros
//            QImage imageCenter((unsigned char*) result.matSrc.data,result.matSrc.cols, result.matSrc.rows, QImage::Format_RGB888);
//            imageCenter = imageCenter.scaled(wResize, hResize, Qt::KeepAspectRatio); //pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
//            QPixmap pixmapCenter = QPixmap::fromImage(imageCenter);
//            sceneResultado->addPixmap(pixmapCenter);
//            ui->gvResultado->setScene(sceneResultado);

            key = cvWaitKey( 1000 / fps );
            count++;
            if ((count % step) == 0) {
                calibrateFrames.push_back(matOriginal.clone());
            }
            //release
            matOriginal.release();
//            matProcess.release();
//            matGray.release();
//            matThresh.release();
//            matResult.release();



        }
        calibration(calibrateFrames);

        cvReleaseCapture( &cap );

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
        objCal = new Calibracion();

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
