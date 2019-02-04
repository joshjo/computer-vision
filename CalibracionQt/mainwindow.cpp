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

void MainWindow::frontoParallel(vector<Mat> frames, vector<vector<Point2f>>points, Mat & K, Mat & D) {

    Point2f source [4];
    Point2f dest [4];
    Mat output, matGray, matThresh, matUndst, matPerspective;
    Data result;
    Mat Ki = K.clone();
    Mat Di = D.clone();
    double rms;
    int flag = 0;

    vector< Mat > rvecs, tvecs;

    for (int k = 0; k < 1; k++) {
        vector <vector<Point2f> > imgPoints;
        vector <vector<Point3f> > objPoints;

        for (int j = 0; j < points.size(); j++) {
            source[0] = points[j][0];
            source[1] = Point2f(points[j][19].x, points[j][0].y);
            source[2] = Point2f(points[j][0].x, points[j][19].y);
            source[3] = points[j][19];

            // Update this to rows
            dest[0] = points[j][0];
            dest[1] = points[j][4];
            dest[2] = points[j][15];
            dest[3] = points[j][19];

            Mat matOriginal = frames[j].clone();

            undistort(matOriginal, matUndst, Ki, Di);
            Mat lambda = getPerspectiveTransform(dest, source);
            warpPerspective(matUndst, matPerspective, lambda, output.size());
            Calibracion * cal = new Calibracion();

            result.matSrc = matPerspective.clone();
            result.matContours = matPerspective.clone();

            cal->grayScale(matGray, matPerspective);
            cal->thresholdMat(matThresh, matGray);
            cal->calculateCenters(result, matThresh.clone(), rows, cols);

            if(result.numValids == cols*rows){
                vector< Point3f > obj;
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        obj.push_back(Point3f((float)j * circleSpacing, (float)i * circleSpacing, 0));
                    }
                }
                objPoints.push_back(obj);
                imgPoints.push_back(result.centers);
            }
        }

        rms = calibrateCamera(objPoints, imgPoints, size, Ki, Di, rvecs, tvecs, flag);

        cout << "rms: " << rms << endl;

        cout << "imgPoints size: " << imgPoints.size() << endl;
        cout << "objPoints size: " << objPoints.size() << endl;
    }

}

void MainWindow::calibration(int width, int height)
{

    //Num frames per position
    int numFrame = 5;
    //Get posicion
    double wSize = width / 3;
    double hSize = height / 3;

    double minW = wSize / 2;// - 10;
    vector<Point2f> referencePoints;
    //Frames and points
    vector<vector<Point2f>> listReference;
    vector<Mat> listReferenceMat;
    vector<int> listId;

    for(int i = 0; i < 3 ; i ++)
    {
        referencePoints.push_back(Point2f(i*wSize,0*hSize));
        referencePoints.push_back(Point2f(i*wSize,1*hSize));
        referencePoints.push_back(Point2f(i*wSize,2*hSize));
    }
    cout << minW << endl;
    //Select best frame to calibracion (Se deberia selecionar todos y recien selecionar los mejores)
    for(int idxRef = 0; idxRef < referencePoints .size(); idxRef++)
    {
        double dist = 0;
        for(int idxFrame = 0; idxFrame < calibrateFrames.size(); idxFrame++)
        {
             imshow("temp", calibrateFrames.at(idxFrame));
            // first point
            Point cornerMat = calibrateFramesVectors.at(idxFrame).at(15); //0
            //distances
            double distPoint = sqrt(pow(calibrateFramesVectors.at(idxFrame).at(15).x - calibrateFramesVectors.at(idxFrame).at(19).x,2) +
                                 pow(calibrateFramesVectors.at(idxFrame).at(15).y - calibrateFramesVectors.at(idxFrame).at(19).y,2));
            double distY = abs(calibrateFramesVectors.at(idxFrame).at(15).y - calibrateFramesVectors.at(idxFrame).at(19).y);
            dist = sqrt(pow(referencePoints.at(idxRef).x - cornerMat.x,2) + pow(referencePoints.at(idxRef).y - cornerMat.y, 2));

            if(std::find(listId.begin(), listId.end(), idxFrame) == listId.end() &&
                    minW > dist && distY < 20 && distPoint < wSize
                    && listReferenceMat.size() < (idxRef*numFrame + numFrame)) //100
            {
                listReferenceMat.push_back(calibrateFrames.at(idxFrame));
                listReference.push_back(calibrateFramesVectors.at(idxFrame));
                listId.push_back(idxFrame);
            }

        }

    }
    cout << "tam" << listReferenceMat.size() << endl;
    /*Scalar color(23,190,187);
    for(int i = 0; i < listReferenceT.size(); i++)
    {
        string a =  "Gray_Image" + to_string( i) + ".jpg" ;//+ i + ".jpg";
         Mat image = listReferenceT.at(i);
         for(int i = 0; i < referencePoints.size(); i++)
            circle(image, referencePoints[i], 2, color, -1, 8, 0);
         imwrite(a, image);
    }
*/

    //Calibration
    ui->calibrationFramesLabel->setText(QString::number(listReference.size()));
    vector< vector< Point3f > > object_points;
    vector <vector <Point2f> > image_points;
    double rms = 0;
    double avrTotal = 0;
    if (listReference.size() > 25) {
        vector< Mat > rvecs, tvecs;
        int flag = 0;
        //        flag |= CV_CALIB_FIX_K4;
        //        flag |= CV_CALIB_FIX_K5;

        vector<float> reprojErrs;
        for(int k = 0; k < listReference.size(); k++) {

            vector< Point3f > obj;
            vector< Point2f > img;
            for (int i = 0; i < rows; i++) {
                vector< Point2f > rowImg;
                for (int j = 0; j < cols; j++) {
                    obj.push_back(Point3f((float)j * circleSpacing, (float)i * circleSpacing, 0));
                    rowImg.push_back(listReference[k][(cols * i) + j]);
                }
                sort(rowImg.begin(), rowImg.end(),
                     [](const Point2f &a, const Point2f &b)
                {
                    return a.x < b.x;
                }
                );
                img.insert(img.begin(), rowImg.begin(), rowImg.end());
            }
            image_points.push_back(img);
            object_points.push_back(obj);
        }
         cout  << "final iter k" << endl;
        //the final re-projection error.
        rms = calibrateCamera(object_points, image_points, size, K, D, rvecs, tvecs, flag);
        avrTotal =  computeReprojectionErrors( object_points, image_points, rvecs, tvecs, K , D,reprojErrs);
         cout  << "init fronto parallel" << endl;
        frontoParallel(listReferenceMat,listReference,K, D);
        cout  << "fin fronto parallel" << endl;
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


void MainWindow::on_pushButton_clicked()
{

    if(verifyParameters())
    {
        ui->calibrationFramesLabel->setText("0");
        ui->fxLabel->setText("0");
        ui->fyLabel->setText("0");
        ui->cxLabel->setText("0");
        ui->cyLabel->setText("0");
        ui->distortionLabel->setText("0");


        int countRecognized = 0;
        int count = 0;
        int totalFrames = 0;
        int width = 0, height = 0;
        //Time
        long t0, t1;
        double time = 0, timeTotal = 0;

        const char* name = nameFile.c_str();
        //QImage
        QImage image, imageGray, imageBinary, imageEdge, imageFinal;
        //Mats
        Mat matOriginal,matProcess, matGray, matThresh;
        Data result;
        CvCapture* cap;
        if(ui->rdnCamera->isChecked())
        {
            cout << "camera" << endl;
            cap = cvCaptureFromCAM(idCamera);
            totalFrames = 1500;
        }
        else
        {
            cap = cvCaptureFromAVI(name);
            totalFrames = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_COUNT) / 10; //?
            width =  cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH );
            height =  cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT );
        }

        cout << "totalFrames: " << totalFrames << endl;


        IplImage* frame = cvQueryFrame( cap );

        int key = 0;
        int fps = ( int )cvGetCaptureProperty( cap, CV_CAP_PROP_FPS );

        if ( !cap )
        {
            fprintf( stderr, "Cannot open AVI!\n" );
            return;
        }
        Size s(cvarrToMat(frame).size());
        size = s;
        //namedWindow("josue", WINDOW_AUTOSIZE);
        //namedWindow("liz", WINDOW_AUTOSIZE);
        int c = 0;

        while( key != 'x')// && c < totalFrames)
        {

            frame = cvQueryFrame( cap );
            if( !frame ) break;
            matOriginal = cv::cvarrToMat(frame);
            result.matSrc = matOriginal.clone();
            result.matContours = matOriginal.clone();


            t0=clock();
            objCal->grayScale(matGray, matOriginal);

            objCal->thresholdMat(matThresh, matGray);

            // objCal->thresholdIntegral(matThresh, matGray);
            objCal->calculateCenters(result, matThresh.clone(), rows, cols);
            t1 = clock();
            time = (double(t1-t0)/CLOCKS_PER_SEC);
            count++;


            if(result.numValids == cols*rows){
                timeTotal += time;
                countRecognized++;
                calibrateFramesVectors.push_back(result.centers);
                calibrateFrames.push_back(matOriginal.clone());


            }

            ui->lblTime->setText(QString::number(time));
            ui->lblNoREconocidos->setText(QString::number(count - countRecognized));
            ui->lblNumTotal->setText(QString::number(count));
            ui->lblReconodicos->setText(QString::number(countRecognized));

            //imshow("josue", result.matContours);
            //imshow("liz", result.matContours);

            image = QImage(matOriginal. data,matOriginal.cols, matOriginal.rows, QImage::Format_RGB888);
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
            c++;
        }
        ui->lblAvgTime->setText(QString::number(timeTotal/countRecognized));
        cvReleaseCapture( &cap );
        calibration(width, height);

    }

}

bool MainWindow::verifyParameters()
{
    if(nameFile.length() <= 0)
    {
        if(!ui->rdnCamera->isChecked())
        {
            QMessageBox::information(this, tr("Can't open the video."), "Invalid name");
            return false;
        } else {
            int maxTested = 10;
            bool res = false;
            for (int i = 0; i < maxTested; i++){
                VideoCapture tmpcamera(i);
                res = (tmpcamera.isOpened());
                tmpcamera.release();
                if (res) {
                    idCamera = i;
                    return true;
                }
            }
            if(!res)
            {
                QMessageBox::warning(this, tr("Error."), "No camera detected.");
                ui->rdnCamera->setChecked(false);
                return true;
            }
        }
    }

    if(ui->txtFilas->text().length() <= 0) {
        QMessageBox::information(this, tr("Error."), "Invalid rows");
        return false;
    }

    if(ui->txtColumnas->text().length() <= 0) {
        QMessageBox::information(this, tr("Error."), "Invalid columns");
        return false;
    }

    if(ui->circleSpacingTxt->text().length() <= 0) {
        QMessageBox::information(this, tr("Error."), "Invalid circle spacing");
        return false;
    } else {
        rows = ui->txtFilas->text().toInt();
        cols = ui->txtColumnas->text().toInt();
        circleSpacing = ui->circleSpacingTxt->text().toDouble();
        return true;
    }
}

void MainWindow::on_openVideoBtn_clicked()
{
    reset();
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Abrir video"), "",
                tr("video (*.avi *.mp4)")
                );
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


