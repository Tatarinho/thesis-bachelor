#include "scrollarea.h"
#include "ui_scrollarea.h"

#include "QPushButton"


float ScrollArea::_H = 0;

ScrollArea::ScrollArea(QWidget *parent) :

    ui(new Ui::ScrollArea)
{
    ui->setupUi(this);

    bg = new Background();
    setWidget(bg);
    setWidgetResizable(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setStyleSheet("background-color:transparent;");
    //connect(this,SIGNAL(sendBgStatus(bool)),bg,SLOT(getBgStatus(bool)));
    connect(this,SIGNAL(sendBgStatus(bool)),bg,SLOT(setStatus(bool)));
    connect(bg,SIGNAL(resizeParent()),this,SLOT(res()));
    connect(this,SIGNAL(resizeBg(float)),bg,SLOT(res(float)));
    connect(bg,SIGNAL(sendHeight(float)),this,SLOT(getBgHeight(float)));
}
ScrollArea::~ScrollArea()
{
    delete ui;
}
void ScrollArea::resizeBackground(float f)
{
    bg->repaint();
    emit resizeBg(f);
}
void ScrollArea::addWaveform(Waveform * wf){
    bg->addWaveform(wf);
}
void ScrollArea::getBgHeight(float f)
{
    emit sendBgHeight(f);
}
void ScrollArea::getBgStatus(bool b)
{
    emit sendBgStatus(b);
}

void ScrollArea::res(QSize s){
    layout()->activate();
}
