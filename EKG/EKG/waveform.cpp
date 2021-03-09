#include "waveform.h"
#include "plotter.h"

#include <QThread>
#include <QFileDialog>
#include <QFile>
#include <cmath>
#include <QRubberBand>
#include <QMessageBox>
#include <QKeyEvent>
#include <QPushButton>
#include <mainwindow.h>

int Waveform::counter = -1;
float Waveform::skala = 0;

Waveform::Waveform(QVector<QPointF> *data, QSize size) {
    layout = new  QVBoxLayout ;
    setLayout(layout);
    counter++;
    id = counter;
    _scale = skala;
    thread = new QThread;
    max = 0;
    min = 0;
    amplitude = 0;

    for(int i = 0; i< data->size();i++){
        if(data[id].at(i).y() > max)
            max = data[id].at(i).y();
        if(data[id].at(i).y() < min)
            min = data[id].at(i).y();
    }
    amplitude =  max - min;

    QSize _size(size.width()*0.97,max*_scale);
    //_size(size.width()*0.97,max*_scale);
    //_size.setWidth(size.width()*0.97);
    //_size.setHeight(max*_scale);
    setMinimumSize(_size);

    plotter = new Plotter(_size, data[id]);
    plotter->moveToThread(thread);
    connect(thread, SIGNAL(started()), plotter, SLOT(plot()));
    connect(plotter, SIGNAL(done(QImage)), this, SLOT(updatePlot(QImage)));
    connect(plotter, SIGNAL(cleanup()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), plotter, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(this,SIGNAL(plotReq(float)),plotter,SLOT(plot()));
    connect(this,SIGNAL(plotHeight(float)),plotter,SLOT(updateHeight(float)));

    connect(plotter,SIGNAL(emituje()),plotter,SLOT(plot()));
    //connect(plotter,SIGNAL(res(QSize)),this,SLOT(res(QSize)));

    setStyleSheet("background-color:none;");
    thread->start();
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rightPointRubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    leftPointRubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}

void Waveform::paintEvent(QPaintEvent * event) {
    //QPainter pixPaint(this);
    //pixPaint.drawPixmap(0,0,_size.width(),_size.height(),pix);
    QPainter p(this);
    if(!plot.isNull()){
        p.drawImage(QPoint(0, 0), plot);
        //resize(plot.size());
        //qDebug() << "Waveform size: " << size();
    }

}
void Waveform::mousePressEvent(QMouseEvent * event)
{
    if(!rubberBand->isHidden())
        rubberBand->hide();
    /*if(!rightPointRubberBand->isHidden())
        rightPointRubberBand->hide();
    if(!leftPointRubberBand->isHidden())
        leftPointRubberBand->hide();*/

    if(MainWindow::horizontalRubberBand){
        mypoint = event->pos();
        mypoint.setY(0);
        /*if(!rubberBand->isHidden()){
            rubberBand->hide();
        }*/
        rubberBand->setGeometry(QRect(mypoint, QSize()));
        rubberBand->show();
    }
    if(MainWindow::verticalRubberBand){
        mypoint = event->pos();
        mypoint.setX(0);
        /*if(!rubberBand->isHidden()){
            rubberBand->hide();
        }*/
        rubberBand->setGeometry(QRect(mypoint, QSize()));
        rubberBand->show();
    }
    if(MainWindow::pointRubberBand){
        if(event->buttons() == Qt::RightButton)
        {
            mypoint = event->pos();
            if(!rightPointRubberBand->isHidden())
                rightPointRubberBand->hide();
            QPoint p = mypoint;
            p.setX(mypoint.x()-7.5);
            p.setY(mypoint.y()-7.5);
            rightPointRubberBand->setGeometry(QRect(p,QSize(15,15)));
            rightPointRubberBand->show();
        }
        if(event->buttons() == Qt::LeftButton)
        {
            mypoint = event->pos();
            /*if(!leftPointRubberBand->isHidden())
                leftPointRubberBand->hide();*/
            QPoint p = mypoint;
            p.setX(mypoint.x()-7.5);
            p.setY(mypoint.y()-7.5);
            leftPointRubberBand->setGeometry(QRect(p,QSize(15,15)));
            leftPointRubberBand->show();
        }
    }

}
void Waveform::mouseMoveEvent(QMouseEvent * event )
{
    if(MainWindow::horizontalRubberBand){
        QPoint p = event->pos();
        p.setY(size().height());
        rubberBand->setGeometry(QRect(mypoint, p).normalized());//Area Bounding
    }
    if(MainWindow::verticalRubberBand){
        QPoint p = event->pos();
        p.setX(size().width());
        rubberBand->setGeometry(QRect(mypoint, p).normalized());//Area Bounding
    }

}
void Waveform::mouseReleaseEvent(QMouseEvent * event )
{
   //rubberBand->hide();// hide on mouse Release

}
void Waveform::updatePlot(QImage p){
   plot = p;
   repaint();

   /*pix.convertFromImage(plot,Qt::ColorOnly);
   //setPixmap(pix);
   repaint();*/
}
void Waveform::updateSize() {
    emit plotHeight(size().height());
}
void Waveform::replot() {
    plotter->replot();
}
void Waveform::next() {
    plotter->next(plotter->getRange());
}
void Waveform::prev() {
    plotter->prev(plotter->getRange());
}
