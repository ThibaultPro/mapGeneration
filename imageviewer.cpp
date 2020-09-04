#include "imageviewer.h"

#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>

ImageViewer::ImageViewer(QWidget *parent)
   : QMainWindow(parent), imageLabel(new QLabel)
   , scrollArea(new QScrollArea)
{
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);
    setCentralWidget(scrollArea);

    createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

