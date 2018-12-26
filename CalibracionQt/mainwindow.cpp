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

    ui->txtColumnas->setValidator(new QIntValidator);
    ui->txtFilas->setValidator(new QIntValidator);

    objCal = new Calibracion();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    if(verifyParameters())
    {
        const char* name = nameFile.c_str();

        Mat matOriginal,matProcess, matGray, matThresh, matResult;
        CvCapture* cap = cvCaptureFromAVI(name);
        IplImage* frame = cvQueryFrame( cap );

        int key = 0;
        int i = 0;
        int fps = ( int )cvGetCaptureProperty( cap, CV_CAP_PROP_FPS );
        //Reconocidos
        int countNoReconocidos = 0;
        int total = 0;
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
            cout << " Time: " << time << endl;


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
            QImage imageEdge((unsigned char*) result.matContours.data,result.matContours.cols, result.matContours.rows, QImage::Format_RGB888);
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
            //release
            matOriginal.release();
            matProcess.release();
            matGray.release();
            matThresh.release();
            matResult.release();

        }
        cvReleaseCapture( &cap );

    }

}

void MainWindow::on_btnAbrir_clicked()
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
