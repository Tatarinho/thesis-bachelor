#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QWidget>
#include<QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "waveform.h"

namespace Ui {
class Background;
}

class Background : public QLabel
{
    Q_OBJECT

public:
    explicit Background(QWidget *parent = 0);
    ~Background();
    void addWaveform(Waveform *_wave);
    static bool isPaperNeeded;
public slots:
    void res(float);
    void dupa();
    void setStatus(bool);
signals:
    void resizeParent(QSize s);
    void sendHeight(float);
protected:
    void paintEvent(QPaintEvent *);
private:
    Ui::Background *ui;
    //QPushButton *button;
    QVBoxLayout *l;
    bool x;
    bool isNeedToRepaint;
};

#endif // BACKGROUND_H
