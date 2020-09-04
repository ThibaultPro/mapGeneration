#ifndef TWEAKING_H
#define TWEAKING_H

#include <QtWidgets>
#include <QObject>
#include <QWidget>

#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/imgproc.hpp>

#include <opencv4/opencv2/imgcodecs.hpp>

using namespace cv;

class Tweaking: public QWidget
{
    Q_OBJECT
public:
    Tweaking(QWidget *parent = 0);
    void generatePerlinNoise(int nOctaveCount, float fScalingBias);
    cv::Mat QImage2Mat(QImage const& src);
    QImage Mat2QImage(cv::Mat const& src);

public slots:
    void createSmallEmptyMap();
    void createNormalEmptyMap();
    void createBigEmptyMap();
    void updateMap();
    void changeOctave(int value);
    void changefSampleBias(int value);
    void changeLevel(int value);
    void generateBunchOfMaps();

signals:
    void mapSizeChanged();
    void parametersChanged();
    void mapsGenerated();

private:
    QLabel *map;
    QImage * generatedMap;

    float *fNoiseSeed2D;
    float *fPerlinNoise2D;
    int nOutputWidth;
    int nOutputHeight;
    int nOctaveCount = 3;
    float fScalingBias = 1.0f;

    float eN = 1;
};

#endif // TWEAKING_H
