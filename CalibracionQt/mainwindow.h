#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsPixmapItem>
#include <ctime>
#include <QFileDialog>
#include <QMessageBox>
#include "calibracion.h"


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

    void on_calibrateBtn_clicked();

    void calibration(vector<Mat> & calibrateFrames);

    void reset();

private:
    Ui::MainWindow *ui;

    Calibracion *objCal;
    string nameFile;
    int cols = 0;
    int rows = 0;
    int wResize = 420;
    int hResize = 236;
    double circleSpacing = 0.0;

    vector <Mat> calibrateFrames;
};

#endif // MAINWINDOW_H
