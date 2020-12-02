//palagutina varvara 2MO
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer=new QTimer(this);  
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(vrema);
    srand(time(0));
    setFixedSize(widht, height);
    //qDebug("start");

    int col=rand()%color; // рандомный цвет клетки

    Komp a={WIDHTPOLE/2,HEIGHTPOLE/2}; //голова змеи
    Zmiy.append(a);
    Col.append(3); //изменить цвет на рандом

    Komp b={WIDHTPOLE/2,HEIGHTPOLE/2+1}; //первый элемент змеи после головы
    Zmiy.append(b);
    Col.append(rand()%color); //измение цвета на рандом

    Komp c={WIDHTPOLE/2,HEIGHTPOLE/2+2}; //второй элемент змеи после головы
    Zmiy.append(c);
    Col.append(rand()%color); //изменение цвета на рандом


    for (int t=0; t<prep; t++) //пояление препятствий
    {
        Komp c={rand()%WIDHTPOLE,rand()%HEIGHTPOLE};
        if ((Prep.contains(c))||((c.i==Zmiy[0].i)&&(c.j==((Zmiy[0].j)-1)))) t--; //проверка, чтобы препятствия не появлись на змейке или прямо перед ней
        else Prep.append(c);
    }

    for (int r=0; r<eda; r++)  //появление еды
    {
        dobav();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    //qDebug("painter");
    if (game)
    {
        QPainter pa (this);

        int kletk = 10; //размер клетки в пикселях

        for (int k=0; k<Zmiy.size(); k++)
        {
            QImage r; 


            r=(Col[k]==0?yel:Col[k]==1?green:Col[k]==2?blue:Col[k]==3?up:Col[k]==4?down:Col[k]==5?left:Col[k]==6?right:yel);
            pa.drawImage(QRect(Zmiy[k].i*kletk, Zmiy[k].j*kletk, kletk, kletk), r);
        }

        for (int k=0; k<Eda.size(); k++)
        {
            QImage r;

            r=(Eda[k].kol==0?yel:Eda[k].kol==1?green:Eda[k].kol==2?blue:yel);
            pa.drawImage(QRect(Eda[k].i*kletk, Eda[k].j*kletk, kletk, kletk), r);
        }

        for (int k=0; k<Prep.size(); k++)
        {
            pa.drawImage(QRect(Prep[k].i*kletk, Prep[k].j*kletk, kletk, kletk), black);
        }
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event) //изменение направления движения + "поворот" головы
{
    if (game)
    {
        if (event->key()==Qt::Key_Left)
            if (napravlenie != 2) { napravlenie = 1; Col[0]=5;} //поворот налево

        if (event->key()==Qt::Key_Right)
            if (napravlenie != 1) { napravlenie = 2; Col[0]=6;} // повотор направо

        if (event->key()==Qt::Key_Up)
            if (napravlenie != 4) { napravlenie = 3; Col[0]=3;} // поворот вверх

        if (event->key()==Qt::Key_Down)
            if (napravlenie != 3) { napravlenie = 4; Col[0]=4;} // поворот вниз
    }
}

void MainWindow::dobav()//добавляет в список еды 1 еду в любую свободную клетку
{
    //qDebug("dobav");
    if (Eda.size()>=eda) return;
    bool y=false;
    do
    {
        int q=rand()%WIDHTPOLE;
        int r=rand()%HEIGHTPOLE;
        int vokrug=0;
        Komp x={q,r,rand()%color};

        for (int t=0; t<Prep.size(); t++) //проверка, чтобы еда не застревала между препятствиями и краями поля
        {
            if ((x.i==Prep[t].i)&&(x.j==((Prep[t].j)-1))) vokrug++;
            if ((x.i==Prep[t].i)&&(x.j==((Prep[t].j)+1))) vokrug++;
            if ((x.i==Prep[t].i+1)&&(x.j==((Prep[t].j)))) vokrug++;
            if ((x.i==Prep[t].i-1)&&(x.j==((Prep[t].j)))) vokrug++;
            if (x.i-1<0) vokrug++;
            if (x.j-1<0) vokrug++;
            if (x.i+1>WIDHTPOLE-1) vokrug++;
            if (x.i+1>HEIGHTPOLE-1) vokrug++;
        }

        if (!(Prep.contains(x)||Zmiy.contains(x)||Eda.contains(x)||vokrug>=3))//проверка, чтобы еда не появлялась на месте препятствий, змейки и другой еды и не застревала
            {
                Eda.append(x);
                y=true;
            }
    } while (y==false);
}

void MainWindow::update()
{


    Komp newhead;
    if (napravlenie == 1) //поворот налево
        newhead={Zmiy[0].i-1, Zmiy[0].j};
    if (napravlenie == 2) // повотор направо
        newhead={Zmiy[0].i+1, Zmiy[0].j};
    if (napravlenie == 3) // поворот вверх
        newhead={Zmiy[0].i, Zmiy[0].j-1};
    if (napravlenie == 4) // поворот вниз
        newhead={Zmiy[0].i, Zmiy[0].j+1};


    if ((newhead.i<0)||(newhead.j<0)||(newhead.i>WIDHTPOLE-1)||(newhead.j>HEIGHTPOLE-1)) game=0; //проверка на врезания в края поля

    if (Prep.contains(newhead)||Zmiy.contains(newhead)) game=0; //проверка, чтобы змей не мог проходить сквозь себя и не врезался в препятствия

    if (game==0)        //завершает игру, если змей врезался во что-то
    {
        QMessageBox::information(this, "ERROR", "GAME OVER");
        qApp->exit();
    }

    Zmiy.prepend(newhead); //функция перемещения
    bool siel=false;
    int u;
    for (u=0; u<Eda.size(); u++)// функция поедания еды
        if ((newhead.i==Eda[u].i)&&(newhead.j==Eda[u].j))
        {
            siel=true;
            break;
        }
    if (siel) //проверка съел или нет
    {
        Col.append(Eda[u].kol);
        Eda.removeAt(u);
        dobav();

        if ((Col[Zmiy.size()-1]==Col[Zmiy.size()-2])&&(Col[Zmiy.size()-1]==Col[Zmiy.size()-3])) //выполение условия, если 3 последние клетки одного цвета, то они схлопываются в одну
        {
            Zmiy.removeLast();
            Col.removeLast();
            Zmiy.removeLast();
            Col.removeLast();
        }
    }
    else Zmiy.removeLast();
    repaint();
}

