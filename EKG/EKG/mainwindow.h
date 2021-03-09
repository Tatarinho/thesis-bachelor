#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QThread>
#include <QKeyEvent>
#include <QToolButton>

#include "scrollarea.h"
#include "waveform.h"


using std::string;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createAction();
    void createMenu();
    bool loadFile();
    void findMin(); // zmienic nazwe
    void addWaves(int,QVector<QPointF> *, QSize);
    void setToolBar();
    static bool horizontalRubberBand;
    static bool verticalRubberBand;
    static bool pointRubberBand;
protected:
    void keyReleaseEvent(QKeyEvent *event);
public slots:
    void slotExit();
    void slotLoad();
    void receiveBgHeight(float);
    void disablePaper();
    void horizontalMeasurerSlot();
    void verticalMeasurerSlot();
    void pointMeasurerSlot();
signals:
    void sendBgStatus(bool);
private:
    float frequency;
    float T;
    float const MILIMETERS = 3.78f;
    //QHBoxLayout *l;
    QVBoxLayout * l;
    QPushButton *b;
    Ui::MainWindow *ui;
    ScrollArea *scrollArea;
    QVector<QPointF> data[12];
    int wavesCounter;
    QVector<Waveform*> waves;
    string fileName;
    QThread *wavesThread;
    QSize _size;
    QMenu *file;
    QAction * loadAction;
    QAction * exitAction;
    QAction * paperBtnAction;
    QAction * stdCursorAction;
    QAction * verticalMeasurerAction;
    QAction * horizontalMeasurerAction;
    QAction * pointMeasurerAction;
    QAction * plusButtonAction;
    QAction * minButtonAction;
    float bgHeight;
    QToolBar *toolBar;
    QToolButton * paper;
    QToolButton * stdCursor;

    QToolButton * horizontalMeasurer;
    QToolButton * verticalMeasurer;
    QToolButton * pointMeasurer;

    QToolButton * plusButton;
    QToolButton * minButton;


};

#endif // MAINWINDOW_H
