#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "plotter.h"
#include <QLabel>
#include <QRubberBand>
#include <QHBoxLayout>

class Waveform : public QLabel
{
    Q_OBJECT
public:
    Waveform(QVector<QPointF> * data);
    Waveform(QVector<QPointF> * data, QSize size);
    void setSize(QVector<QPointF> * _data);
    void replot();
    static int counter;
    void next();
    void prev();
    static float skala;
    void setFixedSize(const QSize &);
    void updateSize();
protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent *ev);
public slots:
    void updatePlot(QImage p);
signals:
    void changeParentSize(float);
    void plotReq();
    void plotHeight(float);
private:
    int id;
    QImage plot;
    float _scale;
    float _max;
    float _min;
    QSize _size;
    QThread * thread;
    Plotter * plotter;
    qreal max;
    qreal min;
    QRubberBand * rectangleRubberBand;
    QVBoxLayout  *layout;
    QPixmap pix;
    qreal amplitude;
    QRubberBand *rubberBand;
    QRubberBand *leftPointRubberBand;
    QRubberBand *rightPointRubberBand;

    QPoint mypoint;

};

#endif // WAVEFORM_H
