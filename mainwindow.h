#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <list>
//#include "snake.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const int widht=25*10;
    const int height=25*10;
    const int WIDHTPOLE=25;
    const int HEIGHTPOLE=25;
    const int prep=10; //количество препятствий
    const int eda=2;
    static const int vrema=200; //скорость апдейта
    int napravlenie=3; // количество возможных направлений змейки (4 штуки)

    bool game=true;

    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QImage yel=QImage(":/pic/ye.png");
    QImage blue=QImage(":/pic/blue.png");
    QImage green=QImage(":/pic/green.png");
    QImage up=QImage(":/pic/up.png");
    QImage black=QImage(":/pic/black.png");
    QImage down=QImage(":/pic/down.png");
    QImage left=QImage(":/pic/left.png");
    QImage right=QImage(":/pic/right.png");


    Ui::MainWindow *ui;

    struct Komp //компоненты змейки
    {

        int i;
        int j;
        int kol;//цвет
        bool operator == (Komp w)
        {
            return ((w.i==i)&&(w.j==j));
        }
       // bool operator != (Komp ne)
       // {
       //     return (!(*this, ne));
       // }
    };
    QList <Komp> Zmiy;
    QList <Komp> Eda;
    QList <Komp> Prep;

    QList <int> Col;
    int color=3; //3 цвета для клетки: 0-желтый, 1-зелений, 2-синий

    void dobav(); //функция добавления еды


public slots:
    void update();
};

#endif // MAINWINDOW_H
