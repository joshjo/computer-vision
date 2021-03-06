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
vector<Point2f> distort(vector<Point2f> point, const Mat cameraMatrix, const Mat distCoeffs)
{
    vector<Point2f> ans;
    double fx = cameraMatrix.at<double>(0,0);
    double fy = cameraMatrix.at<double>(1,1);
    double cx = cameraMatrix.at<double>(0,2);
    double cy = cameraMatrix.at<double>(1,2);

    double k1 = distCoeffs.at<double>(0,0);
    double k2 = distCoeffs.at<double>(0,1);
    double p1 = distCoeffs.at<double>(0,2);
    double p2 = distCoeffs.at<double>(0,3);
    double k3 = distCoeffs.at<double>(0,4);

    for(size_t i = 0; i < point.size(); i++) {

        double x = (point[i].x - cx) / fx;
        double y = (point[i].y - cy) / fy;

        double r2 = x*x + y*y;

        // Radial distorsion
        double xDistort = x * (1 + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2);
        double yDistort = y * (1 + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2);

        // Tangential distorsion
        xDistort = xDistort + (2 * p1 * x * y + p2 * (r2 + 2 * x * x));
        yDistort = yDistort + (p1 * (r2 + 2 * y * y) + 2 * p2 * x * y);

        // Back to absolute coordinates.
        xDistort = xDistort * fx + cx;
        yDistort = yDistort * fy + cy;

        ans.push_back(Point2f(xDistort, yDistort));
    }
    return ans;
}

void MainWindow::iterativeCalibration(vector<Mat> frames,
                                vector<vector<Point2f>>imagesPoints,
                                Mat & K,
                                Mat & D,
                                vector< Mat > &rvecs,
                                vector< Mat > &tvecs) {

    Point2f source [4];
    Point2f dest [4];
    Mat matGray, matThresh, matUndst, matPerspective, matPerspective_inv, matOriginal;

    Mat Ki = K.clone();
    Mat Di = D.clone();
    std::vector<cv::Point2f> undistPointBuf;
    double rms;
    int flag = 0;
    int widthPattern = (5 + 1) * circleSpacing;
    int heightPattern = (4 + 1) * circleSpacing;

    vector<Mat> perspectives;

    vector<float> perViewErrors;
    Mat image = frames[0];

    // Obejct points
    std::vector<std::vector<cv::Point3f>> objectPoints(1);
    for(int i = 0 ; i < 4; i++){
        for(int j = 0; j < 5; j++){
            float x = j * circleSpacing;
            float y = i * circleSpacing;
            objectPoints[0].push_back(cv::Point3f(x,y,0) );
        }
    }
    objectPoints.resize(imagesPoints.size(), objectPoints[0]); //Num object points  == imagesPoint

    //Vector to homography
    vector <Point2f> destHomografy;
    float x, y;
    for(int i = 0 ; i < 4; i++){
        for(int j = 0; j < 5; j++){
            x = (j * circleSpacing) + circleSpacing;
            y = (i * circleSpacing)  + circleSpacing;
            destHomografy.push_back(Point2f(x,y));
        }
    }
    // Number of iterations
    for (int iter = 0; iter < 5; iter++) {

        vector<vector<Point2f>> imagePointsNew;
        // A .- get correct points
        for (int j = 0; j < imagesPoints.size();j++)
        {
            matOriginal = frames[j].clone();
            // Returns the new camera matrix based on the free scaling parameter.
            Mat optimalMatrix = getOptimalNewCameraMatrix(Ki, Di, size, 1.0);

            //1.- Undistortion image
            undistort(matOriginal, matUndst, Ki, Di, optimalMatrix);
            undistortPoints(imagesPoints[j], undistPointBuf, Ki, Di, cv::noArray(), optimalMatrix);

            vector <Point2f> dest = destHomografy;
            Mat H = findHomography(undistPointBuf, dest);
            warpPerspective(matUndst, matPerspective, H, Size(widthPattern, heightPattern));
            string a =  "" + to_string(j) + "_perp.jpg" ;//+ i + ".jpg";
            imwrite(a, matPerspective);//result.matContours);
            //3.- Localize points
            objCal->grayScale(matGray, matPerspective);
            objCal->thresholdMatv2(matThresh, matGray);
           // objCal->calculateCenters(result, matThresh.clone(), rows, cols);
            vector<Point2f> centers;
            findCirclesGrid(matThresh.clone(), Size(5, 4), centers);//, int flags=CALIB_CB_SYMMETRIC_GRID, const Ptr<FeatureDetector>& blobDetector=new SimpleBlobDetector() )

            // string a =  "" + to_string(k) + "_perspective" + to_string(j) + ".jpg" ;//+ i + ".jpg";
            // imwrite(a, matPerspective);//result.matContours);

            // 4.- Gettting point 2D correct
            if( centers.size() == rows*cols)
            {


                warpPerspective(matPerspective, matPerspective_inv, H.inv(), size);
                vector<Point2f> pointsCenterTrans;
                perspectiveTransform(centers, pointsCenterTrans, H.inv());

                //Correct points
                vector<Point2f> pointsCorrect;
                //distor points
                undistortPoints(pointsCenterTrans, pointsCorrect, optimalMatrix, -Di, noArray(), Ki);
                //vector<Point2f> centerDistor = distort(pointsCenterTrans, optimalMatrix,Ki);
                imagePointsNew.push_back(pointsCorrect);

                string a =  "" + to_string(j) + "_distort.jpg" ;//+ i + ".jpg";
                imwrite(a, matPerspective);//result.matContours);

                for(int i = 0; i < 20 ; i++)
                {
                    circle(matOriginal, pointsCenterTrans[i], 2, Scalar(255,0,0), -1, 8, 0);
                    circle(matOriginal, pointsCorrect[i], 2,  Scalar(0,255,0), -1, 8, 0);
                }
                 string b =  "" + to_string(j) + "_temp" + to_string(j) + ".jpg" ;//+ i + ".jpg";
                 imwrite(b, matOriginal);//result.matContours);

            }
        }
        Point2f last = imagesPoints.at(0).at(0);
        imagesPoints  = imagePointsNew;
        // 2 calculate  camera calibration with new points
        rms = calibrateCamera(objectPoints, imagesPoints, size, Ki, Di, rvecs, tvecs, flag);

        cout << iter << "  " << Ki.at<double>(0, 0) << " " << Ki.at<double>(1, 1) << " "
             << Ki.at<double>(0, 2) << " " << Ki.at<double>(1, 2) << " rms: " << rms << " ";
        cout << "New points: " << imagesPoints.at(0).at(0) << " Last: " << last <<endl;

        Mat newCamMat;
        undistort(image, newCamMat, Ki, Di);
        string a =  "" + to_string(iter) + "_und.jpg" ;//+ i + ".jpg";
        imwrite(a, newCamMat);
    }
    string a =  "" + to_string(0) + "_ori.jpg" ;//+ i + ".jpg";
    imwrite(a, frames[0]);//result.matContours);

}

void MainWindow::calibration(int widthFrame, int heightFrame)
{

    //Part frame in 9 small areas
    double wSize = widthFrame / 3;
    double hSize = heightFrame / 3;

    //Points and frame near to referencedPoints
    vector<vector<Point2f>> listPointsSelected;
    vector<Mat> listMatSelected;
    vector<int> listId;

    // Referenced points - small area
    vector<Point2f> referencePoints;

    //Total = 9 points == 9 small areas
    for(int i = 0; i < 3 ; i ++)
    {
        referencePoints.push_back(Point2f(i*wSize,0*hSize));
        referencePoints.push_back(Point2f(i*wSize,1*hSize));
        referencePoints.push_back(Point2f(i*wSize,2*hSize));
    }


    /*Select frames near referenced points*/

    int numFrameSlected = 3;
    int coutFrame = 0;
    // Var auxiliar
    double disReferendecCorner = 0;
    double difAxisY = 0;
    double difAxisY2 = 0;
    double difAxisX = 0;
    double difAxisX2 = 0;

    for(int idxRef = 0; idxRef < referencePoints .size(); idxRef++)
    {
        coutFrame = 0;
        for(int idxFrame = 0; idxFrame < calibrateFrames.size(); idxFrame++)
        {
            if(coutFrame == numFrameSlected)
                break;
            // first point
            Point cornerMat = calibrateFramesVectors.at(idxFrame).at(15); //0

            //dispoint == size.width / 3

            difAxisY = abs(calibrateFramesVectors.at(idxFrame).at(15).y - calibrateFramesVectors.at(idxFrame).at(19).y);
            difAxisY2 = abs(calibrateFramesVectors.at(idxFrame).at(0).y - calibrateFramesVectors.at(idxFrame).at(4).y);

            difAxisX = abs(calibrateFramesVectors.at(idxFrame).at(0).x - calibrateFramesVectors.at(idxFrame).at(15).x);
            difAxisX2 = abs(calibrateFramesVectors.at(idxFrame).at(4).x - calibrateFramesVectors.at(idxFrame).at(19).x);

            // Distance corner references to point
            disReferendecCorner = sqrt(pow(referencePoints.at(idxRef).x - cornerMat.x,2) + pow(referencePoints.at(idxRef).y - cornerMat.y, 2));

            if(std::find(listId.begin(), listId.end(), idxFrame) == listId.end()
                    && difAxisX < 5 && difAxisX2 < 5 // vertical lines 3
                    && difAxisY < 5 && difAxisY2 < 5 //horizontal lines 3
                    && disReferendecCorner < wSize/2 - 10)
            {
                listMatSelected.push_back(calibrateFrames.at(idxFrame));
                listPointsSelected.push_back(calibrateFramesVectors.at(idxFrame));
                listId.push_back(idxFrame);
                coutFrame++;
            }

        }


        cout << "count : " << listMatSelected.size() << endl;

    }
    cout << "N. selected frames: " << listMatSelected.size() << endl;
    ui->calibrationFramesLabel->setText(QString::number(listPointsSelected.size()));
//    Scalar color(23,190,187);
//    for(int i = 0; i < listMatSelected.size(); i++)
//    {
//        string a =  "Gray_Image" + to_string( i) + ".jpg" ;//+ i + ".jpg";
//        Mat image = listMatSelected.at(i);
//        for(int i = 0; i < referencePoints.size(); i++)
//            circle(image, referencePoints[i], 2, color, -1, 8, 0);
//        imwrite(a, image);
//    }


    /************** INITIAL CALIBRATION *******************/

    if(listPointsSelected.size() <= 8)
        return;

    double rms = 0;
    double avrTotal = 0;
    vector<float> reprojErrs;
    vector< Mat > rvecs, tvecs;
    int flag = 0; //int flag = CV_CALIB_FIX_K4;flag |= CV_CALIB_FIX_K5;

    //Revert order points
    revertPoints(listPointsSelected);
    // Object points
    std::vector<std::vector<cv::Point3f>> objectPoints(1);
    for(int i = 0 ; i < 4; i++){
        for(int j = 0; j < 5; j++){
            float x = j * circleSpacing;
            float y = i * circleSpacing;
            objectPoints[0].push_back(cv::Point3f(x,y,0) );
            //std::cout<<"x: "<<x<<"y: "<<y <<std::endl;
        }
    }
    objectPoints.resize(listPointsSelected.size(), objectPoints[0]);

    //Initial Calibration camera
    rms = calibrateCamera(objectPoints, listPointsSelected, size, K, D, rvecs, tvecs, flag);

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


    ui->lblRMS->setText(QString::number( rms ));
    cout << avrTotal << endl;

    /*********************** ITERATIVE CALIBRATION ***********************/
    cout << "iterative calibration" << endl;
    iterativeCalibration(listMatSelected,listPointsSelected,K, D, rvecs, tvecs);

}

void MainWindow::revertPoints(vector<vector<Point2f>> &imagesPoints)
{
    for (int j = 0; j < imagesPoints.size();j++)
    {
        std::vector<cv::Point2f> pointTemp;
        pointTemp.push_back(imagesPoints.at(j).at(15));
        pointTemp.push_back(imagesPoints.at(j).at(16));
        pointTemp.push_back(imagesPoints.at(j).at(17));
        pointTemp.push_back(imagesPoints.at(j).at(18));
        pointTemp.push_back(imagesPoints.at(j).at(19));


        pointTemp.push_back(imagesPoints.at(j).at(10));
        pointTemp.push_back(imagesPoints.at(j).at(11));
        pointTemp.push_back(imagesPoints.at(j).at(12));
        pointTemp.push_back(imagesPoints.at(j).at(13));
        pointTemp.push_back(imagesPoints.at(j).at(14));


        pointTemp.push_back(imagesPoints.at(j).at(5));
        pointTemp.push_back(imagesPoints.at(j).at(6));
        pointTemp.push_back(imagesPoints.at(j).at(7));
        pointTemp.push_back(imagesPoints.at(j).at(8));
        pointTemp.push_back(imagesPoints.at(j).at(9));

        pointTemp.push_back(imagesPoints.at(j).at(0));
        pointTemp.push_back(imagesPoints.at(j).at(1));
        pointTemp.push_back(imagesPoints.at(j).at(2));
        pointTemp.push_back(imagesPoints.at(j).at(3));
        pointTemp.push_back(imagesPoints.at(j).at(4));

        imagesPoints.at(j) = pointTemp;

    }
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
        int widthFrame = 0, heightFrame = 0;
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
            widthFrame =  cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH );
            heightFrame =  cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT );
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

        while( key != 'x' || c < totalFrames)// && c < totalFrames)
        {
            c++;

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
        calibration(widthFrame, heightFrame);

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


