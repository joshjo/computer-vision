#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsPixmapItem>
#include <ctime>
#include <QFileDialog>
#include <QMessageBox>
#include "calibracion.h"

#define CAPTURING 1
#define CALIBRATING 2
#define CALIBRATED 0

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    bool verifyParameters();

    void on_openVideoBtn_clicked();

    void calibration(int width, int height);

    void reset();

    void frontoParallel(vector<Mat> frames, vector<vector<Point2f>>points, Mat & K, Mat & D, vector< Mat > rvecs,vector< Mat > tvecs);

    void revertPoints(vector<vector<Point2f>> &imagesPoints);


private:
    Ui::MainWindow *ui;

    Calibracion *objCal;
    string nameFile;
    int cols = 0;
    int rows = 0;
    int wResize = 420;
    int hResize = 236;
    double circleSpacing = 0.0;
    int idCamera = 0;

    int modeCalibrating;
    int framesCalibration;

    vector <vector<Point2f>> calibrateFramesVectors;
    vector <Mat> calibrateFrames;

    Size size;

    Mat K;
    Mat D;
};

#endif // MAINWINDOW_H
