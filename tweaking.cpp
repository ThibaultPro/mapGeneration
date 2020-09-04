#include "tweaking.h"
#include <QPixmap>
#include <QSlider>
#include <QLabel>
#include <QSpacerItem>
#include <QObject>
#include <QWidget>
#include <QList>
#include <iostream>
#include <sstream>


Tweaking::Tweaking(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout * layout = new QVBoxLayout(this);


    QPushButton * smallMap = new QPushButton;
    QPushButton * normalMap = new QPushButton;
    QPushButton * bigMap = new QPushButton;

    smallMap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    normalMap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    bigMap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    smallMap->setText("Small");
    normalMap->setText("Normal");
    bigMap->setText("Big");

    smallMap->setIcon(QPixmap(":/images/CM_Continental.png"));
    smallMap->setIconSize(QSize(100, 100));

    normalMap->setIcon(QPixmap(":/images/CM_Continental.png"));
    normalMap->setIconSize(QSize(150, 150));

    bigMap->setIcon(QPixmap(":/images/CM_Continental.png"));
    bigMap->setIconSize(QSize(200, 200));

    QHBoxLayout * chooseSizeLayout = new QHBoxLayout;

    chooseSizeLayout->addWidget(smallMap);
    chooseSizeLayout->addWidget(normalMap);
    chooseSizeLayout->addWidget(bigMap);

    layout->addLayout(chooseSizeLayout);

    QObject::connect(smallMap, SIGNAL(clicked()), this, SLOT(createSmallEmptyMap()));
    QObject::connect(normalMap, SIGNAL(clicked()), this, SLOT(createNormalEmptyMap()));
    QObject::connect(bigMap, SIGNAL(clicked()), this, SLOT(createBigEmptyMap()));

    QLabel *octave = new QLabel;
    octave->setText("Tweak nOctave value (1, 9) :");
    layout->addWidget(octave);

    QSlider *nOctaveSlider = new QSlider(Qt::Horizontal);
    nOctaveSlider->setMaximum(8);
    nOctaveSlider->setMinimum(1);

    layout->addWidget(nOctaveSlider);

    QObject::connect(nOctaveSlider, &QSlider::valueChanged, this, &Tweaking::changeOctave);

    QLabel *dial = new QLabel;
    dial->setText("Change value of fSampleBias");;

    QDial *fSampleBiasDial = new QDial;
    fSampleBiasDial->setMaximum(5);
    fSampleBiasDial->setMinimum(-5);
    fSampleBiasDial->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QSpinBox *disp = new QSpinBox;
    disp->setMinimum(-20);

    QHBoxLayout *sampleBiasDialLayout = new QHBoxLayout;

    sampleBiasDialLayout->addWidget(dial);
    sampleBiasDialLayout->addWidget(fSampleBiasDial);
    sampleBiasDialLayout->addWidget(disp);

    layout->addLayout(sampleBiasDialLayout);

    QObject::connect(fSampleBiasDial, &QDial::valueChanged, disp, &QSpinBox::setValue);
    QObject::connect(fSampleBiasDial, &QDial::valueChanged, this, &Tweaking::changefSampleBias);

    QLabel *dial2 = new QLabel;
    dial2->setText("Change value for height of seas :");


    QDial *levelDial = new QDial;
    levelDial->setMaximum(5);
    levelDial->setMinimum(-5);
    levelDial->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QSpinBox *disp2 = new QSpinBox;
    disp2->setMinimum(-6);

    QHBoxLayout *levelDialLayout = new QHBoxLayout;
    levelDialLayout->addWidget(dial2);
    levelDialLayout->addWidget(levelDial);
    levelDialLayout->addWidget(disp2);

    layout->addLayout(levelDialLayout);

    QObject::connect(levelDial, &QDial::valueChanged, disp2, &QSpinBox::setValue);
    QObject::connect(levelDial, &QDial::valueChanged, this, &Tweaking::changeLevel);

    QSpacerItem *vertical = new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addItem(vertical);

    generatedMap = new QImage;

    map = new QLabel;

    QHBoxLayout *centerMap = new QHBoxLayout;
    QSpacerItem *left = new QSpacerItem(20, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    QSpacerItem *right = new QSpacerItem(80, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    centerMap->addItem(left);
    centerMap->addWidget(map);
    centerMap->addItem(right);

    layout->addLayout(centerMap);

    QObject::connect(this, SIGNAL(mapSizeChanged()), this, SLOT(updateMap()));

    QObject::connect(this, SIGNAL(parametersChanged()), this, SLOT(updateMap()));

    createNormalEmptyMap();

    qDebug()<<nOutputWidth;
}

QImage Tweaking::Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,4); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}

cv::Mat Tweaking::QImage2Mat(QImage const& src)
{
     cv::Mat result(src.height(),src.width(),CV_8UC4,(uchar*)src.bits(),src.bytesPerLine());
     //cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     //cvtColor(tmp, result, 4);

     return result;
}

void Tweaking::createSmallEmptyMap()
{
    nOutputWidth = 200;
    nOutputHeight = 200;

    fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
    fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];

    for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand()/ (float)RAND_MAX;

    cv::Mat image = cv::Mat::zeros(nOutputWidth, nOutputHeight, CV_8UC4);

    if (image.empty())
    {
        qDebug()<<"could not open image";

    }
    *generatedMap = Mat2QImage(image);

    map->setPixmap(QPixmap::fromImage(*generatedMap));

    emit mapSizeChanged();
}

void Tweaking::createNormalEmptyMap()
{
    nOutputWidth = 300;
    nOutputHeight = 300;

    fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
    fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];

    for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand()/ (float)RAND_MAX;

    cv::Mat image = cv::Mat::zeros(nOutputWidth, nOutputHeight, CV_8UC4);

    if (image.empty())
    {
        qDebug()<<"could not open image";

    }
    *generatedMap = Mat2QImage(image);

    map->setPixmap(QPixmap::fromImage(*generatedMap));

    emit mapSizeChanged();
}

void Tweaking::createBigEmptyMap()
{
    nOutputWidth = 400;
    nOutputHeight = 400;

    fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
    fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];

    for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand()/ (float)RAND_MAX;

    cv::Mat image = cv::Mat::zeros(nOutputWidth, nOutputHeight, CV_8UC4);

    if (image.empty())
    {
        qDebug()<<"could not open image";
    }
    *generatedMap = Mat2QImage(image);

    map->setPixmap(QPixmap::fromImage(*generatedMap));

    emit mapSizeChanged();
}

void Tweaking::generatePerlinNoise(int nOctaveCount, float fScalingBias)
{
    for (int x = 0; x < nOutputWidth; x++)
    {
        for (int y = 0; y < nOutputHeight; y++)
        {
            float fNoise = 0.0f;
            float fScaleAcc = 0.0f;
            float fScale = 1.0f;

            for (int o = 0; o < nOctaveCount; o++)
            {
                int nPitch = nOutputWidth >> o;

                int nSampleX1 = (x / nPitch) * nPitch;
                int nSampleY1 = (y / nPitch) * nPitch;

                int nSampleX2 = (nSampleX1 + nPitch) % nOutputWidth;
                int nSampleY2 = (nSampleY1 + nPitch) % nOutputWidth;

                float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
                float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

                float fSampleT = (1.0f - fBlendX) * fNoiseSeed2D[nSampleY1 * nOutputWidth + nSampleX1] + fBlendX * fNoiseSeed2D[nSampleY1 * nOutputWidth + nSampleX2];
                float fSampleB = (1.0f - fBlendX) * fNoiseSeed2D[nSampleY2 * nOutputWidth + nSampleX1] + fBlendX * fNoiseSeed2D[nSampleY2 * nOutputWidth + nSampleX2];

                fScaleAcc += fScale;
                fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
                fScale = fScale / fScalingBias;
            }
            // Scale to seed range

            fPerlinNoise2D[y * nOutputWidth + x] = (fNoise / fScaleAcc);
        }
    }
    return;
}

void Tweaking::updateMap()
{
    generatePerlinNoise(nOctaveCount, fScalingBias);

    cv::Mat mapImg = cv::Mat::zeros(nOutputWidth, nOutputHeight, CV_8UC4);

    cv::MatIterator_<cv::Vec4b> it;

    int i=0;

    for (it = mapImg.begin<cv::Vec4b>(); it!=mapImg.end<cv::Vec4b>(); ++it)
    {
        int pixel_bw = (int)((pow(fPerlinNoise2D[i], eN))*6);
        switch (pixel_bw)
        {
            case 0 :
                (*it)[0]=252;
                (*it)[1]=86;
                (*it)[2]=3;
                break;
            case 1 :
                (*it)[0]=3;
                (*it)[1]=244;
                (*it)[2]=252;
                break;
            case 2 :
                (*it)[0]=3;
                (*it)[1]=252;
                (*it)[2]=157;
                break;
            case 3 :
                (*it)[0]=0;
                (*it)[1]=255;
                (*it)[2]=0;
                break;
            case 4 :
                (*it)[0]=0;
                (*it)[1]=255;
                (*it)[2]=0;
                break;

            case 5 :
                (*it)[0]=0;
                (*it)[1]=255;
                (*it)[2]=125;
                break;

            case 6 :
                (*it)[0]=0;
                (*it)[1]=255;
                (*it)[2]=132;

                break;

            default:
                (*it)[1]=0;
                (*it)[1]=0;
                (*it)[2]=255;
                break;
        }
        ++i;
    }

    *generatedMap = Mat2QImage(mapImg);

    map->setPixmap(QPixmap::fromImage(*generatedMap));
}

void Tweaking::changeOctave(int value)
{
    nOctaveCount = value;
    emit parametersChanged();
}

void Tweaking::changefSampleBias(int value)
{
    fScalingBias = exp(value);
    emit parametersChanged();
}

void Tweaking::changeLevel(int value)
{
    if (value<0)
    {
        value = - value;
        eN = 1/(1 + value/5);
        qDebug()<<eN;
    }
    else if (value >0)
    {
        eN = value;
    }
    emit parametersChanged();
}

void Tweaking::generateBunchOfMaps()
{
    qDebug()<<"was called";
    for (int i =0; i<6; ++i)
    {
        fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
        fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];

        for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand()/ (float)RAND_MAX;

        generatePerlinNoise(nOctaveCount, fScalingBias);

        cv::Mat mapImg = cv::Mat::zeros(nOutputWidth, nOutputHeight, CV_8UC4);

        cv::MatIterator_<cv::Vec4b> it;

        int j=0;

        for (it = mapImg.begin<cv::Vec4b>(); it!=mapImg.end<cv::Vec4b>(); ++it)
        {
            int pixel_bw = (int)((pow(fPerlinNoise2D[j], eN))*6);
            switch (pixel_bw)
            {
                case 0 :
                    (*it)[0]=252;
                    (*it)[1]=86;
                    (*it)[2]=3;
                    break;
                case 1 :
                    (*it)[0]=3;
                    (*it)[1]=244;
                    (*it)[2]=252;
                    break;
                case 2 :
                    (*it)[0]=3;
                    (*it)[1]=252;
                    (*it)[2]=157;
                    break;
                case 3 :
                    (*it)[0]=0;
                    (*it)[1]=255;
                    (*it)[2]=0;
                    break;
                case 4 :
                    (*it)[0]=0;
                    (*it)[1]=255;
                    (*it)[2]=0;
                    break;
                case 5 :
                    (*it)[0]=0;
                    (*it)[1]=255;
                    (*it)[2]=125;
                    break;
                case 6 :
                    (*it)[0]=0;
                    (*it)[1]=255;
                    (*it)[2]=132;
                    break;
                default:
                    (*it)[1]=0;
                    (*it)[1]=0;
                    (*it)[2]=255;
                    break;
            }
            ++j;
        }

        std::string filename;
        std::ostringstream temp;
        temp <<"maps/map"<<i<<".png";

        filename = temp.str();

        QString str = QString::fromUtf8(filename.c_str());
        QImage newMap = Mat2QImage(mapImg);

        newMap.save(str);

    }
    emit mapsGenerated();
}
