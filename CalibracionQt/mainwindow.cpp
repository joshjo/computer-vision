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
static double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                         const vector<vector<Point2f> >& imagePoints,
                                         const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                         const Mat& cameraMatrix , const Mat& distCoeffs,
                                         vector<float>& perViewErrors)
{
    vector<Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for( i = 0; i < (int)objectPoints.size(); ++i )
    {
        projectPoints( Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,
                       distCoeffs, imagePoints2);
        err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);

        int n = (int)objectPoints[i].size();
        perViewErrors[i] = (float) std::sqrt(err*err/n);
        totalErr        += err*err;
        totalPoints     += n;
    }

    return std::sqrt(totalErr/totalPoints);
}
void MainWindow::calibration(vector<Mat> & calibrateFrames) {
    float radius = 0.0243;
    Mat img, matGray, matThresh, matResult;

    vector< vector< Point3f > > object_points;
    vector< vector< Point2f > > image_points;
    int number;

    char img_file[100];

    double rms = 0;
    double avrTotal = 0;
    vector<float> reprojErrs;
    for(int i = 0; i < calibrateFrames.size(); i++) {
        img = calibrateFrames[i];

        Calibracion cal;

        Data result;
        result.matSrc = img.clone();
        result.matContours = img.clone();

        cal.grayScale(matGray, img);
        cal.thresholdMat(matThresh, matGray);
        matResult = matThresh.clone();

        cal.calculateCenters(result, matThresh.clone(), rows, cols);

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

    if (image_points.size() > 10 && (object_points.size() == image_points.size())) {
        Mat K;
        Mat D;
        vector< Mat > rvecs, tvecs;
        int flag = 0;
        flag |= CV_CALIB_FIX_K4;
        flag |= CV_CALIB_FIX_K5;
        if (image_points.size() > 25) {
            image_points.erase(image_points.begin() + 25, image_points.end());
            object_points.erase(object_points.begin() + 25, object_points.end());
        }
        //the final re-projection error.
        rms = calibrateCamera(object_points, image_points, img.size(), K, D, rvecs, tvecs, flag);
        avrTotal =  computeReprojectionErrors( object_points, image_points, rvecs, tvecs,
                                  K , D,reprojErrs);
        QString qstr = "";

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
    ui->lblRMS->setText(QString::number( rms ));
    cout << avrTotal << endl;
}


void MainWindow::on_calibrateBtn_clicked()
{
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

        int totalFrames = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_COUNT) / 10;

        int step = totalFrames / 35;

        cout << "totalFrames: " << totalFrames << endl;

        IplImage* frame = cvQueryFrame( cap );

        int key = 0;
        int fps = ( int )cvGetCaptureProperty( cap, CV_CAP_PROP_FPS );

        //Reconocidos
        int countRecognized = 0;
        int count = 0;
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
            count++;


            if(result.numValids > 0){
                timeTotal += time;
                countRecognized++;
            }

            ui->lblTime->setText(QString::number(time));
            ui->lblNoREconocidos->setText(QString::number(count - countRecognized));
            ui->lblNumTotal->setText(QString::number(count));
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
            if ((count % step) == 0) {
                calibrateFrames.push_back(matOriginal.clone());
            }
            //release
            matOriginal.release();
            matProcess.release();
            matGray.release();
            matThresh.release();




        }

        ui->lblAvgTime->setText(QString::number(timeTotal/countRecognized));
        cvReleaseCapture( &cap );

        calibration(calibrateFrames);


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
    reset();
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

void MainWindow::reset()
{
    ui->lblVideo->setText("");
    ui->txtColumnas->setText("");
    ui->txtFilas->setText("");
    ui->lblNumTotal->setText("");
    ui->lblReconodicos->setText("");
    ui->lblNoREconocidos->setText("");
    ui->lblTime->setText("");
    ui->lblAvgTime->setText("");
    ui->lblRMS->setText("0" );
    ui->calibrationFramesLabel->setText("0");
    ui->fxLabel->setText("0");
    ui->fyLabel->setText("0");
    ui->cxLabel->setText("0");
    ui->cyLabel->setText("0");
    ui->distortionLabel->setText("0");

    //Imagenes
    nameFile = "";
    ui->lblOriginal->clear();
    ui->lblGrayScale->clear();
    ui->lblThreshold->clear();
    ui->lblContour->clear();
    ui->lblFinal->clear();

}
