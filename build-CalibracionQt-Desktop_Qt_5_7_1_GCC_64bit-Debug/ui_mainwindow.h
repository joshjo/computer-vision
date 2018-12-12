/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGraphicsView *gvOriginal;
    QGraphicsView *gvGrayScale;
    QGraphicsView *gvUmbral;
    QGraphicsView *gvDilate;
    QGraphicsView *gvEdge;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1200, 700);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gvOriginal = new QGraphicsView(centralWidget);
        gvOriginal->setObjectName(QStringLiteral("gvOriginal"));
        gvOriginal->setGeometry(QRect(10, 40, 365, 205));
        gvGrayScale = new QGraphicsView(centralWidget);
        gvGrayScale->setObjectName(QStringLiteral("gvGrayScale"));
        gvGrayScale->setGeometry(QRect(410, 40, 365, 205));
        gvUmbral = new QGraphicsView(centralWidget);
        gvUmbral->setObjectName(QStringLiteral("gvUmbral"));
        gvUmbral->setGeometry(QRect(810, 40, 365, 205));
        gvDilate = new QGraphicsView(centralWidget);
        gvDilate->setObjectName(QStringLiteral("gvDilate"));
        gvDilate->setGeometry(QRect(10, 310, 365, 205));
        gvEdge = new QGraphicsView(centralWidget);
        gvEdge->setObjectName(QStringLiteral("gvEdge"));
        gvEdge->setGeometry(QRect(410, 310, 365, 205));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 67, 17));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(410, 20, 111, 17));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(810, 20, 67, 17));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 290, 67, 17));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(410, 290, 67, 17));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(1070, 540, 89, 25));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Original", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Escala de Grises", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Threshold", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Dilataci\303\263n", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Bordes", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "Inicio", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
