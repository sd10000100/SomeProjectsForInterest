#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>
#include <math.h>
#include <QThread>

using namespace std;



vector<tuple<int, int>> points;
vector<tuple<int, int>> startPoints;
vector<tuple<int, int>> bestPoints;
int pathCoast = INT_MAX;
int bestPathCoast = INT_MAX ;
double temp = 100;
int countItems = 10;//что то где то не так
int countBurns = 0;

double distanceSqr(tuple<int, int> a, tuple<int, int> b) {
    return (std::get<0>(a) - std::get<0>(b)) * (std::get<0>(a) - std::get<0>(b)) + (std::get<1>(a) - std::get<1>(b)) * (std::get<1>(a) - std::get<1>(b));
}

vector<tuple<int, int>> RandomPlacement(vector<tuple<int, int>> a, int n)
{
    vector<tuple<int, int>> b(a);
    for(int i = 0; i < n; i++)
        swap(b[i],b[rand()%n]);
    return b;
}


void RandomPlacement(int ind1, int ind2)
{
    swap(points[ind1],points[ind2]);
}

//int calcPathLength()



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    for(int i=0;i<countItems;i++)
    {
        int x = rand() %301 - 100;
        int y = rand() %301 - 100;
        points.push_back(make_tuple(x,y));
    }
    std::vector<tuple<int, int>> startPoints(points);
    std::vector<tuple<int, int>> bestPoints(points);
//    startPoints.swap(points);
//    bestPoints.swap(points);
//startPoints = RandomPlacement(points,countItems);

   // draw();
    drawWithPath();
    bestPathCoast = pathCoast;
    countBurns = 0;
    //if (bestPathCoast == INT_MAX) pathCoast = bestPathCoast;
}

void MainWindow::draw()
{
    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    //Это как раз создана сцена. Сцена - это класс для работы с 2D графикой.
    //Теперь, раз это график, то построим координатные оси X и Y.
   // QPen pen(Qt::green);//Просто выбираем цвет для карандашика
    //scene->addLine(0,90,180,90,pen);//x
    //scene->addLine(90,0,90,180,pen);//y
    scene->clear();
    ui->textBrowser->clear();
    double rad = 2;
    for(int i=0;i<countItems;i++)
    {
        int x = std::get<0>(points[i]);
        int y = std::get<1>(points[i]);
        QPointF pt = QPointF(x,y);
        //ui->textBrowser->insertPlainText(QString::number(m[i].nextstate[j]));
        ui->textBrowser->insertPlainText(QString::number(x));
        ui->textBrowser->insertPlainText(",");
        ui->textBrowser->insertPlainText(QString::number(y));
        ui->textBrowser->insertPlainText("              ");

        scene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0,
            QPen(), QBrush(Qt::SolidPattern));
    }



    ui->graphicsView->setScene(scene);



}

void MainWindow::drawWithPath()
{
    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    //Это как раз создана сцена. Сцена - это класс для работы с 2D графикой.
    //Теперь, раз это график, то построим координатные оси X и Y.
    QPen pen(Qt::green);//Просто выбираем цвет для карандашика
    //scene->addLine(0,90,180,90,pen);//x
    //scene->addLine(90,0,90,180,pen);//y
    scene->clear();
    ui->textBrowser->clear();
    double rad = 2;

    QPointF ptLast = QPointF(std::get<0>(points[0]),std::get<1>(points[0]));
    ui->textBrowser->insertPlainText(QString::number(ptLast.x()));
    ui->textBrowser->insertPlainText(",");
    ui->textBrowser->insertPlainText(QString::number(ptLast.y()));
    ui->textBrowser->insertPlainText("              ");

    scene->addEllipse(ptLast.x()-rad, ptLast.y()-rad, rad*2.0, rad*2.0,
        QPen(), QBrush(Qt::SolidPattern));

    pathCoast = 0;
    for(int i=1;i<countItems;i++)
    {
        int x = std::get<0>(points[i]);
        int y = std::get<1>(points[i]);
        QPointF pt = QPointF(x,y);
        //ui->textBrowser->insertPlainText(QString::number(m[i].nextstate[j]));
        ui->textBrowser->insertPlainText(QString::number(x));
        ui->textBrowser->insertPlainText(",");
        ui->textBrowser->insertPlainText(QString::number(y));
        ui->textBrowser->insertPlainText("              ");
        pathCoast+=distanceSqr(points[i], points[i-1]);
        scene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0,
            QPen(), QBrush(Qt::SolidPattern));


        scene->addLine(ptLast.x(),ptLast.y(),pt.x(),pt.y(),pen);//x
        ptLast = pt;
        //scene->addLine(90,0,90,180,pen);//y
    }
    scene->addLine(ptLast.x(),ptLast.y(),std::get<0>(points[0]),std::get<1>(points[0]),pen);//x
    pathCoast+=distanceSqr(points[countItems-1], points[0]);

    ui->textBrowser_2->clear();
       ui->textBrowser_2->insertPlainText(QString::number(pathCoast));
      // if
//    for(int i=1;i<100;i++)
//    {
//        int x = std::get<0>(points[i]);
//        int y = std::get<1>(points[i]);
//        QPointF pt = QPointF(x,y);
//        //ui->textBrowser->insertPlainText(QString::number(m[i].nextstate[j]));
//        ui->textBrowser->insertPlainText(QString::number(x));
//        ui->textBrowser->insertPlainText(",");
//        ui->textBrowser->insertPlainText(QString::number(y));
//        ui->textBrowser->insertPlainText("              ");

//        scene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0,
//            QPen(), QBrush(Qt::SolidPattern));
//    }

    countBurns++;
    ui->textBrowser_5->clear();
       ui->textBrowser_5->insertPlainText(QString::number(countBurns));
    ui->graphicsView->setScene(scene);



}

int random_1_100()
{
    return rand() % 100 + 1;
}

int random_0_countItems(){
    return rand() % 10;
}

void MainWindow::generateRandomWay()
{

points = RandomPlacement(points,countItems);

}



void MainWindow::burn()
{
    //generateRandomWay();
    int itm1 = random_0_countItems();
    int itm2 = random_0_countItems();
    RandomPlacement(itm1, itm2);


    drawWithPath();
    string d = "замена "+to_string(itm1)+ " на "+to_string(itm2)+".  Температура "+to_string(temp)+".  ";
    d+="лучший результат - "+to_string(bestPathCoast)+ " новый - "+to_string(pathCoast)+".   ";
    if(pathCoast<bestPathCoast) // новое решение лучше
    {
        d+=". новое решение лучше  ";
        bestPathCoast = pathCoast;
     // std::vector<tuple<int, int>> bestPoints(points);
      bestPoints.assign(points.begin(), points.end());
        //  bestPoints.swap(points);
       //bestPoints = points;
    }
    else
    {
        d+=". новое решение хуже  ";
        double pstar = 100*exp(-(pathCoast-bestPathCoast)/(temp*bestPathCoast) );
        double pcrit = random_1_100();
        d+=". p*  "+to_string(pstar)+ " pk "+to_string(pcrit)+".  ";
        if(pstar<pcrit)
        {
            d+=". так себе ход. вовращаем на  "+to_string(bestPathCoast)+".   ";
          // std::vector<tuple<int, int>> points(bestPoints);
           points.assign(bestPoints.begin(), bestPoints.end());
           //  points.swap(bestPoints);
            drawWithPath();
            //points = bestPoints;
        }
    }
    temp = 0.9*temp;
    ui->textBrowser_4->insertPlainText(QString::fromStdString(d));
    ui->textBrowser_3->clear();
       ui->textBrowser_3->insertPlainText(QString::number(temp));
}

void MainWindow::on_pushButton_clicked()
{
    while(temp>0.0000000001)
    {

        burn();
//QThread::sleep(2);
        //sleep(1000);
    }
}

MainWindow::~MainWindow()
{

    delete ui;
}


