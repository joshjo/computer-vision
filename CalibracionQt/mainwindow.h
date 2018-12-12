#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsPixmapItem>
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

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *sceneGray;
    QGraphicsScene *sceneBinary;
    QGraphicsScene *sceneDilate;
    QGraphicsScene *sceneEdge;
    Calibracion *objCal;
    int wResize = 512;
    int hResize = 288;
};

#endif // MAINWINDOW_H
