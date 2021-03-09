#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "background.h"
#include <QScrollArea>

namespace Ui {
class ScrollArea;
}

class ScrollArea : public QScrollArea
{
    Q_OBJECT

public:

    explicit ScrollArea(QWidget *parent = 0);
    ~ScrollArea();
    static float _H;
    void addWaveform(Waveform *);
    void resizeBackground(float );
signals:
    void resizeBg(float);
    void sendBgHeight(float);
    void sendBgStatus(bool);
public slots:
    void res(QSize s);
    void getBgHeight(float);
    void getBgStatus(bool);
private:
    //QPushButton * b;
    QVBoxLayout *l;
    Ui::ScrollArea *ui;
    Background *bg;
};

#endif // SCROLLAREA_H
