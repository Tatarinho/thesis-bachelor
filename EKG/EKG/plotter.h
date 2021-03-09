#ifndef PLOTTER_H
#define PLOTTER_H

#include<QObject>
#include<QVector>
#include<QRectF>
#include<QSize>
#include<QPainter>
#include<QImage>
#include<QPointF>
#include<QFile>
#include<QDebug>
#include<QFileDialog>
#include<QString>


class Plotter : public QObject {
    Q_OBJECT
public:
    Plotter(QSize size, QVector<QPointF> data);
    void invertData(QVector<QPointF> *);
    void next(int shift);
    void prev(int shift);
    int getRange();
    void replot();
    float getScale();
    void scale(QVector<QPointF> *data);
    float getAmplitude();
    float getAmplitude(QVector<QPointF> * tmp);
    static float skala;
signals:
    void done(QImage);
    void emituje();
    void cleanup();
    void res(QSize);


public slots:
    void plot();
    void updateHeight(float);
private:
    const float MILLIMETERS = 3.78*4;
    QSize _size;
    float _scale;
    QVector<QPointF> _data;
    QImage *img;
    const int MAX_SIZE = 32768;

    float widthSrc;
    float heightSrc;
    float widthDest;
    float heightDest;

    float ratioX = widthSrc/widthDest;
    float ratioY = heightSrc/heightDest;

    int left;
    int right;
    int range;
};

#endif // PLOTTER_H
