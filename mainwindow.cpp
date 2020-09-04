#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QSplitter>
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QPixmap>
#include <iostream>
#include <sstream>


#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/opencv_modules.hpp>

#include "tweaking.h"

using namespace cv;
using Matf = cv::Mat_<float>;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget * mapGenerator = new QWidget(this);

    QSplitter * h1Splitter = new QSplitter(mapGenerator);
    QSplitter * h2Splitter = new QSplitter(mapGenerator);
    QSplitter * h3Splitter = new QSplitter(mapGenerator);

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);

    vSplitter->addWidget(h1Splitter);
    vSplitter->addWidget(h2Splitter);
    vSplitter->addWidget(h3Splitter);

    map = new QImage;

    map1 = new QLabel;
    map2 = new QLabel;
    map3 = new QLabel;
    map4 = new QLabel;
    map5 = new QLabel;
    map6 = new QLabel;

    createEmptyMap();
    map1->setText("Seed(1)");
    map1->setPixmap(QPixmap::fromImage(*map));

    map2->setPixmap(QPixmap::fromImage(*map));
    map3->setPixmap(QPixmap::fromImage(*map));
    map4->setPixmap(QPixmap::fromImage(*map));
    map5->setPixmap(QPixmap::fromImage(*map));
    map6->setPixmap(QPixmap::fromImage(*map));

    h1Splitter->addWidget(map1);
    map1->show();

    h1Splitter->addWidget(map2);
    map2->show();

    h2Splitter->addWidget(map3);
    map3->show();

    h2Splitter->addWidget(map4);
    map4->show();

    h3Splitter->addWidget(map5);
    map5->show();

    h3Splitter->addWidget(map6);
    map6->show();

    Tweaking *adjustWidget = new Tweaking();

    QWidget *centralGeneratingWidget = new QWidget;

    QSpacerItem *horizontalAdjustementUp = new QSpacerItem(150,0,QSizePolicy::Expanding, QSizePolicy::Expanding);
    QPushButton *generatingButton = new QPushButton;
    generatingButton->setText("Generate Maps");
    generatingButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    generatingButton->setFocusPolicy(Qt::StrongFocus);
    generatingButton->setFixedHeight(150);

    QSpacerItem *adjustementDown = new QSpacerItem(150,0,QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addItem(horizontalAdjustementUp);
    centralLayout->addWidget(generatingButton);
    centralLayout->addItem(adjustementDown);

    centralGeneratingWidget->setLayout(centralLayout);

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(adjustWidget);
    layout->addWidget(centralGeneratingWidget);
    layout->addWidget(vSplitter);
    mapGenerator->setLayout(layout);

    setCentralWidget(mapGenerator);
    setWindowTitle(tr("Maps"));

    QObject::connect(generatingButton, SIGNAL(clicked()), adjustWidget, SLOT(generateBunchOfMaps()));
    QObject::connect(adjustWidget, SIGNAL(mapsGenerated()), this, SLOT(displayMaps()));
}

QImage MainWindow::loadMap(QString filename)
{
    QImage * image = new QImage;
    image->load(filename);


    // Scale the image to given size
    *image = image->scaled(QSize(300, 300), Qt::KeepAspectRatio);

    return *image;
}

QImage MainWindow::Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,4); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}


cv::Mat MainWindow::QImage2Mat(QImage const& src)
{
     cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result,4);
     return result;
}

void MainWindow::createEmptyMap()
{
    //Matf image = Matf::zeros(300, 300);
    cv::Mat image = cv::Mat::zeros(300, 300, CV_8UC3);

    if (image.empty())
    {
        qDebug()<<"could not open image";

    }

    *map = Mat2QImage(image);
}

void MainWindow::displayMaps()
{
    QImage *maps = new QImage[6];

    for (int i = 0; i<6; ++i)
    {
        std::string filename;
        std::string text;
        std::ostringstream temp1;
        std::ostringstream temp2;

        temp1 <<":/maps/map"<<i<<".png";
        filename = temp1.str();

        temp2<<"Seed ("<<i+1<<")";
        text = temp2.str();

        QString strFilename = QString::fromUtf8(filename.c_str());
        QString strDisplay = QString::fromUtf8(text.c_str());

        maps[i].load(strFilename);
        QPainter *p = new QPainter(&maps[i]);
        p->setPen(QPen(Qt::white));
        p->setFont(QFont("Times", 16, QFont::Bold));
        p->drawText(map->rect(), Qt::AlignCenter, strDisplay);
    }

    map1->setPixmap(QPixmap::fromImage(maps[0]));
    map2->setPixmap(QPixmap::fromImage(maps[1]));
    map3->setPixmap(QPixmap::fromImage(maps[2]));
    map4->setPixmap(QPixmap::fromImage(maps[3]));
    map5->setPixmap(QPixmap::fromImage(maps[4]));
    map6->setPixmap(QPixmap::fromImage(maps[5]));

}


MainWindow::~MainWindow()
{
}


