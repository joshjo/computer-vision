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
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_11;
    QHBoxLayout *horizontalLayout_16;
    QHBoxLayout *horizontalLayout_15;
    QPushButton *openVideoBtn;
    QLabel *lblVideo;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_14;
    QHBoxLayout *horizontalLayout_8;
    QFormLayout *formLayout_2;
    QLabel *label_13;
    QLineEdit *txtFilas;
    QFormLayout *formLayout;
    QLineEdit *txtColumnas;
    QLabel *label_14;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout_17;
    QPushButton *calibrateBtn;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_11;
    QLabel *lblNumTotal;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_9;
    QLabel *lblReconodicos;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_10;
    QLabel *lblNoREconocidos;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_7;
    QLabel *lblTime;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QLabel *lblAvgTime;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_13;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_2;
    QLabel *lblGrayScale;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_4;
    QLabel *lblContour;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_6;
    QLabel *lblFinal;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_3;
    QLabel *lblThreshold;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QLabel *lblOriginal;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout_4;
    QLabel *label_51;
    QLabel *calibrationFramesLabel;
    QLabel *label_8;
    QLabel *fxLabel;
    QLabel *label_12;
    QLabel *fyLabel;
    QLabel *label_15;
    QLabel *cxLabel;
    QLabel *label_16;
    QLabel *cyLabel;
    QLabel *label_17;
    QLabel *distortionLabel;
    QLabel *label_18;
    QLabel *lblWaiting;
    QMenuBar *menuBar;
    QMenu *menuOpen_video;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1366, 810);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_9 = new QVBoxLayout(centralWidget);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        horizontalLayout_11 = new QHBoxLayout(groupBox_2);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        openVideoBtn = new QPushButton(groupBox_2);
        openVideoBtn->setObjectName(QStringLiteral("openVideoBtn"));

        horizontalLayout_15->addWidget(openVideoBtn);

        lblVideo = new QLabel(groupBox_2);
        lblVideo->setObjectName(QStringLiteral("lblVideo"));

        horizontalLayout_15->addWidget(lblVideo);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer);


        horizontalLayout_16->addLayout(horizontalLayout_15);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_13);

        txtFilas = new QLineEdit(groupBox_2);
        txtFilas->setObjectName(QStringLiteral("txtFilas"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, txtFilas);


        horizontalLayout_8->addLayout(formLayout_2);

        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        txtColumnas = new QLineEdit(groupBox_2);
        txtColumnas->setObjectName(QStringLiteral("txtColumnas"));

        formLayout->setWidget(0, QFormLayout::FieldRole, txtColumnas);

        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QStringLiteral("label_14"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_14);


        horizontalLayout_8->addLayout(formLayout);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_12->addWidget(pushButton);


        horizontalLayout_8->addLayout(horizontalLayout_12);


        horizontalLayout_14->addLayout(horizontalLayout_8);


        horizontalLayout_16->addLayout(horizontalLayout_14);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        calibrateBtn = new QPushButton(groupBox_2);
        calibrateBtn->setObjectName(QStringLiteral("calibrateBtn"));

        horizontalLayout_17->addWidget(calibrateBtn);


        horizontalLayout_16->addLayout(horizontalLayout_17);


        horizontalLayout_11->addLayout(horizontalLayout_16);


        verticalLayout_9->addWidget(groupBox_2);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        horizontalLayout_7 = new QHBoxLayout(groupBox);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_4->addWidget(label_11);

        lblNumTotal = new QLabel(groupBox);
        lblNumTotal->setObjectName(QStringLiteral("lblNumTotal"));
        lblNumTotal->setAutoFillBackground(true);

        horizontalLayout_4->addWidget(lblNumTotal);


        horizontalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_6->addWidget(label_9);

        lblReconodicos = new QLabel(groupBox);
        lblReconodicos->setObjectName(QStringLiteral("lblReconodicos"));
        lblReconodicos->setAutoFillBackground(true);

        horizontalLayout_6->addWidget(lblReconodicos);


        horizontalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_3->addWidget(label_10);

        lblNoREconocidos = new QLabel(groupBox);
        lblNoREconocidos->setObjectName(QStringLiteral("lblNoREconocidos"));
        lblNoREconocidos->setAutoFillBackground(true);

        horizontalLayout_3->addWidget(lblNoREconocidos);


        horizontalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_5->addWidget(label_7);

        lblTime = new QLabel(groupBox);
        lblTime->setObjectName(QStringLiteral("lblTime"));
        lblTime->setAutoFillBackground(true);

        horizontalLayout_5->addWidget(lblTime);


        horizontalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_2->addWidget(label_5);

        lblAvgTime = new QLabel(groupBox);
        lblAvgTime->setObjectName(QStringLiteral("lblAvgTime"));

        horizontalLayout_2->addWidget(lblAvgTime);


        horizontalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_7->addLayout(horizontalLayout);


        verticalLayout_9->addWidget(groupBox);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout_13 = new QHBoxLayout(groupBox_3);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        verticalLayout_5->addWidget(label_2);

        lblGrayScale = new QLabel(groupBox_3);
        lblGrayScale->setObjectName(QStringLiteral("lblGrayScale"));
        lblGrayScale->setAutoFillBackground(true);

        verticalLayout_5->addWidget(lblGrayScale);


        gridLayout_2->addLayout(verticalLayout_5, 0, 1, 1, 1);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        verticalLayout_7->addWidget(label_4);

        lblContour = new QLabel(groupBox_3);
        lblContour->setObjectName(QStringLiteral("lblContour"));
        lblContour->setAutoFillBackground(true);

        verticalLayout_7->addWidget(lblContour);


        gridLayout_2->addLayout(verticalLayout_7, 1, 0, 1, 1);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);

        verticalLayout_8->addWidget(label_6);

        lblFinal = new QLabel(groupBox_3);
        lblFinal->setObjectName(QStringLiteral("lblFinal"));
        lblFinal->setAutoFillBackground(true);

        verticalLayout_8->addWidget(lblFinal);


        gridLayout_2->addLayout(verticalLayout_8, 1, 1, 1, 1);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        verticalLayout_6->addWidget(label_3);

        lblThreshold = new QLabel(groupBox_3);
        lblThreshold->setObjectName(QStringLiteral("lblThreshold"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lblThreshold->sizePolicy().hasHeightForWidth());
        lblThreshold->setSizePolicy(sizePolicy2);
        lblThreshold->setAutoFillBackground(true);

        verticalLayout_6->addWidget(lblThreshold);


        gridLayout_2->addLayout(verticalLayout_6, 0, 2, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        verticalLayout_4->addWidget(label);

        lblOriginal = new QLabel(groupBox_3);
        lblOriginal->setObjectName(QStringLiteral("lblOriginal"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(45);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lblOriginal->sizePolicy().hasHeightForWidth());
        lblOriginal->setSizePolicy(sizePolicy3);
        lblOriginal->setAutoFillBackground(true);

        verticalLayout_4->addWidget(lblOriginal);


        gridLayout_2->addLayout(verticalLayout_4, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(groupBox_3);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        verticalLayout = new QVBoxLayout(groupBox_4);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formLayout_4 = new QFormLayout();
        formLayout_4->setSpacing(6);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        label_51 = new QLabel(groupBox_4);
        label_51->setObjectName(QStringLiteral("label_51"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_51);

        calibrationFramesLabel = new QLabel(groupBox_4);
        calibrationFramesLabel->setObjectName(QStringLiteral("calibrationFramesLabel"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, calibrationFramesLabel);

        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_8);

        fxLabel = new QLabel(groupBox_4);
        fxLabel->setObjectName(QStringLiteral("fxLabel"));

        formLayout_4->setWidget(1, QFormLayout::FieldRole, fxLabel);

        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QStringLiteral("label_12"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, label_12);

        fyLabel = new QLabel(groupBox_4);
        fyLabel->setObjectName(QStringLiteral("fyLabel"));

        formLayout_4->setWidget(2, QFormLayout::FieldRole, fyLabel);

        label_15 = new QLabel(groupBox_4);
        label_15->setObjectName(QStringLiteral("label_15"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, label_15);

        cxLabel = new QLabel(groupBox_4);
        cxLabel->setObjectName(QStringLiteral("cxLabel"));

        formLayout_4->setWidget(3, QFormLayout::FieldRole, cxLabel);

        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName(QStringLiteral("label_16"));

        formLayout_4->setWidget(4, QFormLayout::LabelRole, label_16);

        cyLabel = new QLabel(groupBox_4);
        cyLabel->setObjectName(QStringLiteral("cyLabel"));

        formLayout_4->setWidget(4, QFormLayout::FieldRole, cyLabel);

        label_17 = new QLabel(groupBox_4);
        label_17->setObjectName(QStringLiteral("label_17"));

        formLayout_4->setWidget(5, QFormLayout::LabelRole, label_17);

        distortionLabel = new QLabel(groupBox_4);
        distortionLabel->setObjectName(QStringLiteral("distortionLabel"));

        formLayout_4->setWidget(5, QFormLayout::FieldRole, distortionLabel);

        label_18 = new QLabel(groupBox_4);
        label_18->setObjectName(QStringLiteral("label_18"));

        formLayout_4->setWidget(6, QFormLayout::LabelRole, label_18);

        lblWaiting = new QLabel(groupBox_4);
        lblWaiting->setObjectName(QStringLiteral("lblWaiting"));

        formLayout_4->setWidget(6, QFormLayout::FieldRole, lblWaiting);


        verticalLayout->addLayout(formLayout_4);


        gridLayout_2->addWidget(groupBox_4, 1, 2, 1, 1);


        horizontalLayout_13->addLayout(gridLayout_2);


        verticalLayout_9->addWidget(groupBox_3);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1366, 22));
        menuOpen_video = new QMenu(menuBar);
        menuOpen_video->setObjectName(QStringLiteral("menuOpen_video"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuOpen_video->menuAction());
        menuOpen_video->addAction(actionOpen);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Actions", Q_NULLPTR));
        openVideoBtn->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        lblVideo->setText(QString());
        label_13->setText(QApplication::translate("MainWindow", "Rows:", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "Columns:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
        calibrateBtn->setText(QApplication::translate("MainWindow", "Calibrate", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "Frames:", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "Processed:", Q_NULLPTR));
        lblNumTotal->setText(QString());
        label_9->setText(QApplication::translate("MainWindow", "Recognized:", Q_NULLPTR));
        lblReconodicos->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Unrecognized:", Q_NULLPTR));
        lblNoREconocidos->setText(QString());
        label_7->setText(QApplication::translate("MainWindow", "Time(sec):", Q_NULLPTR));
        lblTime->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "Avg. Time (sec.): ", Q_NULLPTR));
        lblAvgTime->setText(QString());
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Process", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Grayscale", Q_NULLPTR));
        lblGrayScale->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "Borders", Q_NULLPTR));
        lblContour->setText(QString());
        label_6->setText(QApplication::translate("MainWindow", "Final result", Q_NULLPTR));
        lblFinal->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Threshold", Q_NULLPTR));
        lblThreshold->setText(QString());
        label->setText(QApplication::translate("MainWindow", "Original", Q_NULLPTR));
        lblOriginal->setText(QString());
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Calibration Results:", Q_NULLPTR));
        label_51->setText(QApplication::translate("MainWindow", "Frames used for calibration", Q_NULLPTR));
        calibrationFramesLabel->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "fx", Q_NULLPTR));
        fxLabel->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "fy", Q_NULLPTR));
        fyLabel->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "cx", Q_NULLPTR));
        cxLabel->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_16->setText(QApplication::translate("MainWindow", "cy", Q_NULLPTR));
        cyLabel->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_17->setText(QApplication::translate("MainWindow", "Distortion", Q_NULLPTR));
        distortionLabel->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_18->setText(QString());
        lblWaiting->setText(QString());
        menuOpen_video->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
