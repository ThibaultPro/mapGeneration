#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtWidgets>
#include <iostream>
#include <sstream>

#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/cvconfig.h>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE
using namespace cv;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage loadMap(QString filename);
    void createEmptyMap();

    QImage Mat2QImage(cv::Mat const& src);
    cv::Mat QImage2Mat(QImage const& src);
    void updateMap();

public slots:
    void displayMaps();

private:
    QImage *map;
    int R;
    QLabel *map1;
    QLabel *map2;
    QLabel *map3;
    QLabel *map4;
    QLabel *map5;
    QLabel *map6;


};
#endif // MAINWINDOW_H
