#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotter.h"
#include "waveform.h"

#include <QDebug>
#include <fstream>
#include <cmath>

#include <QPalette>
using std::ifstream;

bool MainWindow::horizontalRubberBand = false;
bool MainWindow::verticalRubberBand = false;
bool MainWindow::pointRubberBand = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    l = new QHBoxLayout;
    l = new QVBoxLayout;
    centralWidget()->setLayout(l);
    toolBar = new QToolBar(this);
    toolBar->setStyleSheet("background-color:transparent;");
    toolBar->setOrientation(Qt::Horizontal);
    scrollArea = new ScrollArea;
    l->addWidget(toolBar);
    l->addWidget(scrollArea);

   // l->addWidget(scrollArea, 0, Qt::AlignCenter);

    wavesThread = new QThread;
    createAction();
    setToolBar();
    createMenu();
    connect(scrollArea,SIGNAL(sendBgHeight(float)),this,SLOT(receiveBgHeight(float)));
    connect(this,SIGNAL(sendBgStatus(bool)),scrollArea,SLOT(getBgStatus(bool)));

    //setStyleSheet("background-color:transparent;"); setStyleSheet("background:transparent;");
    //setAttribute(Qt::WA_TranslucentBackground);
    //setWindowFlags(Qt::FramelessWindowHint);
    //setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setToolBar()
{
    paper = new QToolButton;
    paper->setDefaultAction(paperBtnAction);
    paper->setIcon(QIcon(":/img/paper.png"));
    paper->setStyleSheet("background:darkred");
    paper->setGeometry(0,0,10,20);
    toolBar->addWidget(paper);

    /*stdCursor = new QToolButton;
    stdCursor->setDefaultAction(stdCursorAction);
    stdCursor->setIcon(QIcon(":/cursor.png"));
    toolBar->addWidget(stdCursor);*/

    horizontalMeasurer = new QToolButton;
    horizontalMeasurer->setDefaultAction(horizontalMeasurerAction);
    //measurerCursor->setIcon(QIcon(":/cursor-cross.png"));
    toolBar->addWidget(horizontalMeasurer);

    verticalMeasurer = new QToolButton;
    verticalMeasurer->setDefaultAction(verticalMeasurerAction);
    verticalMeasurer->setIcon(QIcon(":/img/arrow-vertical.png"));
    toolBar->addWidget(verticalMeasurer);

    pointMeasurer = new QToolButton;
    pointMeasurer->setDefaultAction(pointMeasurerAction);
    toolBar->addWidget(pointMeasurer);

    plusButton = new QToolButton;
    plusButton->setDefaultAction(plusButtonAction);
    plusButton->setIcon(QIcon(":/plus.png"));
    toolBar->addWidget(plusButton);

   /* minButton = new QToolButton;
    minButton->setDefaultAction(minButtonAction);
    minButton->setIcon(QIcon(":/minus.png"));
    toolBar->addWidget(minButton);*/

    toolBar->setIconSize(QSize(30, 30));
}
void MainWindow::receiveBgHeight(float f)
{
    bgHeight = f;

    foreach(Waveform *wf, waves){
        wf->updateSize();
    }
}
void MainWindow::createAction()
{
    loadAction = new QAction(tr("Load"),this);
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction,SIGNAL(triggered()),this,SLOT(slotLoad()));

    exitAction = new QAction(tr("Exit"),this);
    exitAction->setShortcut(QKeySequence::Close);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(slotExit()));

    paperBtnAction = new QAction(tr("Enable"), this);
    connect(paperBtnAction,SIGNAL(triggered()),this,SLOT(disablePaper()));

    horizontalMeasurerAction = new QAction(tr("HM"),this);
    connect(horizontalMeasurerAction,SIGNAL(triggered()),this,SLOT(horizontalMeasurerSlot()));

    verticalMeasurerAction = new QAction(tr("VM"), this);
    connect(verticalMeasurerAction,SIGNAL(triggered()),this,SLOT(verticalMeasurerSlot()));

    pointMeasurerAction = new QAction(tr("PM"),this);
    connect(pointMeasurerAction,SIGNAL(triggered()),this,SLOT(pointMeasurerSlot()));
}
void MainWindow::createMenu()
{
    file = menuBar() -> addMenu(tr("File"));
    file->addAction(loadAction);
    file->addAction(exitAction);
}
void MainWindow::slotExit()
{
    MainWindow::close();
}

void MainWindow::disablePaper() {
    if(Background::isPaperNeeded){
        Background::isPaperNeeded = false;
        emit sendBgStatus(false);
    }
    else{
        Background::isPaperNeeded = true;
        emit sendBgStatus(true);
    }
}
void MainWindow::horizontalMeasurerSlot(){
    if(horizontalRubberBand){
        horizontalRubberBand = false;
    }
    else{
        horizontalRubberBand = true;
        verticalRubberBand = false;
        pointRubberBand = false;
    }
}
void MainWindow::verticalMeasurerSlot(){
    if(verticalRubberBand){
        verticalRubberBand = false;
    }
    else{
        verticalRubberBand = true;
        horizontalRubberBand = false;
        pointRubberBand = false;
    }

}
void MainWindow::pointMeasurerSlot(){
    if(pointRubberBand){
        pointRubberBand = false;
    }
    else{
        verticalRubberBand = false;
        horizontalRubberBand = false;
        pointRubberBand = true;
    }
}
void MainWindow::slotLoad()
{
    if(loadFile()){
        qDebug() << "Wczytalem plik!";
        findMin();

        _size = size();
        _size.setWidth(size().width()*1);
        _size.setHeight(size().height()*0.90);
        float scale = (_size.height())/ScrollArea::_H;
        scale *= 0.88;
        Waveform::skala = scale;
        Plotter::skala = scale;
        qDebug() << "skala:" << scale;
        //sc = new MyScroll(_size);
        //l->addWidget(scrollArea);
        addWaves(wavesCounter, data, _size);
    }
    else{
        qDebug() << "Nie wczytalem pliku!";
    }

}
bool MainWindow::loadFile()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)")).toStdString();

    for(int i = 0; i < 12; ++i){
        data[i].clear();
    }
    int x = 0;

    string line;
    QStringList lines;
    ifstream file(fileName.c_str());
    //QElapsedTimer t;
    //t.start();
    const float PIXELS = 0.264583333;
    if(file.is_open())
    {
        getline(file,line);
        wavesCounter = atoi(line.c_str());
        getline(file,line);
        frequency = atoi(line.c_str());
        T = 1/frequency;
        T*PIXELS;
        //T = 94.5 / frequency;
        getline(file,line);
        int m = atoi(line.c_str());
        for(int i = 0; i < m; ++i)
        {
            getline(file,line);
            lines = QString::fromStdString(line).split(QRegExp("\\s"));
            for(int j = 0; j < wavesCounter; ++j) {
                float _y = lines.at(j).toFloat()*PIXELS;
                data[j].push_back(QPointF(x,_y));//TUKURWA
            }
            x+=T;
        }
    }
    else{
        return 0;
    }
    //qDebug() << "loaded in" << t.elapsed() << "msec";
    file.close();

    return 1;
}
void MainWindow::findMin(){
    qreal min = 0;
    qreal max = 0;
    for(int i = 0; i<12; ++i) {
        for(int j = 0; j< data[i].size(); ++j){
            if(data[i].at(j).y() < min){
                min = data[i].at(j).y();
            }
            if(data[i].at(j).y() > max){
                max = data[i].at(j).y();
            }
        }
        if(min < 0){
            for(int j = 0; j<data[i].size(); ++j) {
                QPointF q(j, fabs(min));
                QPointF a(0,data[i].at(j).y());
                a = a+q;
                data[i].replace(j,a);
            }
        }
        else if(min > 0){
            for(int j = 0; j<data[i].size(); ++j) {
                QPointF q(j, fabs(min));
                QPointF a(0,data[i].at(j).y());
                a = a-q;
                data[i].replace(j,a);
            }
        }
        ScrollArea::_H += (max - min);
        min = 0;
        max = 0;
    }
}
void MainWindow::addWaves(int _nrOFWaves, QVector<QPointF>* data, QSize _size)
{
    int sum = 0;
    for(int i=0; i<_nrOFWaves;++i){
        Waveform *_wave = new Waveform(data,_size);
        waves.push_back(_wave);
        //sum+=  waves.at(i)->size().height();
    }
    //background = new Background(QSize(waves.at(0)->size().width(),sum+100));
     //myScroll->addWidget(background);
     //scrollArea->setWidget(background);
    //background->setParent(this);
    QVectorIterator<Waveform*> it(waves);
    while (it.hasNext()){
        scrollArea->addWaveform(it.next());
    }

    //myScroll->addWidget(background);


    wavesThread->start();
}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
   //qDebug() << "You Realesed Key " + event->text();
   if(event->key() == Qt::Key_Plus){
       qDebug() << Plotter::skala;
       float _h = (bgHeight*Plotter::skala*1.1)/Plotter::skala;
       scrollArea->resizeBackground(_h);
       Plotter::skala*=1.1;
       foreach(Waveform *wf, waves)
       {
          wf->replot();
       }
       qDebug() << Plotter::skala;
   }
   if(event->key() == Qt::Key_Minus){
       qDebug() << Plotter::skala;
       float _h = (bgHeight*Plotter::skala/1.1)/Plotter::skala;
       scrollArea->resizeBackground(_h);
       Plotter::skala/=1.1;
       foreach(Waveform *wf, waves)
       {
          wf->replot();
       }
       qDebug() << Plotter::skala;
   }
   if(event->key() == Qt::Key_Right)
   {
       foreach(Waveform *wf, waves)
          wf->next();
   }
   if(event->key() == Qt::Key_Left)
   {
       foreach(Waveform *wf, waves)
          wf->prev();
   }
}
