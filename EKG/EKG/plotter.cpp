#include "plotter.h"
#include <QElapsedTimer>
#include <cmath>
#include <QFrame>

float Plotter::skala = 0;

Plotter::Plotter(QSize size,QVector<QPointF> data)
{
    _size = size;
    _data = data;
    _scale = skala;
    left = 0;
    right = _size.width();
    range = right-left;
    widthSrc = _data.size();
    heightSrc = getAmplitude();
}
void Plotter::plot() {

    widthDest = widthSrc / skala;
    heightDest = heightSrc / skala;

    ratioX = widthSrc/widthDest;
    ratioY = heightSrc/heightDest;

    // Dalsza część kodu
    QElapsedTimer t;
    t.start();

    QVector<QPointF> tmp;

    float actualWidth = 0;
    int i = left;
    while( (actualWidth < _size.width() && i < _data.size() )){
        tmp.push_back(QPointF(i-left,_data.at(i).y()));
        actualWidth = _data.at(i).x()*ratioX;
        i++;
    }
    _size.setHeight(getAmplitude(&tmp));
    scale(&tmp);
    //_size.setWidth(tmp.size());
    //_size.setHeight(getAmplitude());
    QImage img(_size, QImage::Format_ARGB32);
    qDebug() <<"Size of the plot: " << _size;
   // img.fill(Qt::white);
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black,2);
    pen.setWidth(1);
    p.setPen(pen);
    p.setPen(QPen(Qt::black,1));
    p.drawPolyline(tmp.data(),tmp.size());
    qDebug() << "plotted in" << t.elapsed() << "msec";
    emit res(_size);
    qDebug() << "I emit: " <<_size;
    emit done(img);

    //emit cleanup();
}
void Plotter::invertData(QVector<QPointF> * v){
    for(QVector<QPointF>::iterator it = v->begin(); it != v->end(); ++it){
           it->setY(fabs((_size.height() - it->y())));
    }
}
float Plotter::getAmplitude(){
    float max = _data[0].y();
    float min = _data[0].y();
    float amplitude = 0;
    for(int i = 1; i<_data.size();++i){
        if(_data.at(i).y() > max)
            max = _data.at(i).y();
        if(_data.at(i).y() < min)
            min = _data.at(i).y();
    }
    amplitude = max - min;
    return amplitude;
}
float Plotter::getAmplitude(QVector<QPointF> *tmp){
    float max = tmp->at(0).y();
    float min = tmp->at(0).y();
    float amplitude = 0;
    for(int i = 1; i<tmp->size();++i){
        if(_data.at(i).y() > max)
            max = tmp->at(i).y();
        if(_data.at(i).y() < min)
            min = tmp->at(i).y();
    }
    amplitude = max - min;
    return amplitude;
}
void Plotter::scale(QVector<QPointF> *data){
    for(QVector<QPointF>::iterator it = data->begin(); it != data->end(); ++it){
            it->setY(it->y()*ratioY);
            it->setX(it->x()*ratioX);
    }
}
void Plotter::updateHeight(float f){
    //_size.setHeight(f);

    plot();
    //emit emituje();
}

float Plotter::getScale(){
    return _scale;
}
void Plotter::next(int shift)
{
    if(left+shift < _data.size() && right+shift < _data.size()){
        left += shift;
        right += shift;
    }
    else {
        right = _data.size();
        left = _data.size()-shift;
    }
    emit emituje();
}
void Plotter::replot(){
    emit emituje();
}
void Plotter::prev(int shift)
{
    if(left-shift > 0 && right-shift > 0)
    {
        left -= shift;
        right -= shift;
    }
    else
    {
        left = 0;
        right = left+range;
    }
    emit emituje();
}

int Plotter::getRange()
{
    return range;
}
