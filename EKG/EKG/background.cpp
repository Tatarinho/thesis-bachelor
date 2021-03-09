#include "background.h"
#include "ui_background.h"
#include <qpainter.h>

bool Background::isPaperNeeded = true;

Background::Background(QWidget *parent) :
    //QWidget(parent),
    QLabel(parent),
    ui(new Ui::Background)
{
    ui->setupUi(this);
    l = new QVBoxLayout;
    setLayout(l);
    //button = new QPushButton("dupa");
    //l->addWidget(button);
    setMinimumSize(300,100);
    //l->setSizeConstraint(QLayout::SetMinimumSize);
    setStyleSheet("background-color:transparent;");
    //connect(button, SIGNAL(clicked()), this, SLOT(res()));
    isPaperNeeded = true;
    isNeedToRepaint = true;
    //setAttribute(Qt::WA_OpaquePaintEvent);
}
Background::~Background()
{
    delete ui;
}
void Background::addWaveform(Waveform *_wave){
     //_wave->setParent(this);
    connect(_wave,SIGNAL(changeParentSize(QSize)),this,SLOT(res(QSize)));
    //layout->addWidget(_wave);
    l->addWidget(_wave);
    emit sendHeight(size().height());
    isNeedToRepaint = true;
}
void Background::res(float s){
   //int width = size().width() + 300;

   //float height = size().height()+s;
   float height = s;

   //setFixedWidth(width);
   setFixedHeight(height);
   isNeedToRepaint = true;

   emit sendHeight(size().height());
}
void Background::dupa(){
    repaint();
}
void Background::setStatus(bool b)
{
    isPaperNeeded = b;
    repaint();
}

/*void Background::getBgStatus(bool b)
{
    isPaperNeeded = b;
    //repaint();
}*/
void Background::paintEvent(QPaintEvent *) {
   if(isPaperNeeded){
       const float MILLIMETERS= 3.78;
       //const float PIXELS = 0.264583333;
       float T = MILLIMETERS;//Plotter::skala;
       //float T = MILLIMETERS/(1+Plotter::skala);
       //float T = PIXELS;
       int horizontalLines = size().height()/T;
       int verticalLines = size().width()/T;
       QPainter p(this);

       for(int i = 0; i < verticalLines; i++){
           if(i%5==0){
               p.setPen(QPen(Qt::darkRed,0.15));}
           else{
               p.setPen(QPen(Qt::darkRed,0.05));}
           //p.drawLine(QPointF(i*MILLIMETERS/(1+Plotter::skala),0),QPointF(i*MILLIMETERS/(1+Plotter::skala),size().height()));
           p.drawLine(QPointF(i*T,0),QPointF(i*T,size().height()));
       }
       for(int i = 0; i<horizontalLines; i++){
           if(i%5==0){
               p.setPen(QPen(Qt::darkRed,0.15));
           }
           else{
               p.setPen(QPen(Qt::darkRed,0.05));
           }
           //p.drawLine(QPointF(0,i*MILLIMETERS/(1+Plotter::skala)),QPointF(size().width(),i*MILLIMETERS/(1+Plotter::skala)));
           p.drawLine(QPointF(0,i*T),QPointF(size().width(),i*T));
       }
       //isNeedToRepaint = false;
   }
}
