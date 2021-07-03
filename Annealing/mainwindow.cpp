#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>
#include <math.h>
#include <QThread>
#include "simulatedannealing.cpp"

using namespace std;


SimulatedAnnealing<tuple<int,int>> annealMethod;
//vector<tuple<int, int>> points;
//vector<tuple<int, int>> startPoints;
//vector<tuple<int, int>> bestPoints;
//int pathCoast = INT_MAX;
//int bestPathCoast = INT_MAX ;
//constexpr double EPS = 1e-11;
//double temp = 100;
//int countItems = 10;//что то где то не так
//int countBurns = 0;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    std::vector<tuple<int, int>> points;
    ui->setupUi(this);

    srand (2423534654);
    int countItems = 10;
    for(int i=0;i<countItems;i++)
    {
        int x = random_int(-100, 200);
        int y = random_int(-100, 200);
        points.push_back(make_tuple(x,y));
    }
    annealMethod.Init(countItems, points);
    drawWithPath();
}


void MainWindow::drawWithPath()
{
    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    QPen pen(Qt::green);
    scene->clear();
    ui->textBrowser->clear();
    double rad = 2;

    auto points = annealMethod.getBestWay();
    QPointF ptLast = QPointF(std::get<0>(points[0]),std::get<1>(points[0]));
    ui->textBrowser->insertPlainText(QString::number(ptLast.x()));
    ui->textBrowser->insertPlainText(",");
    ui->textBrowser->insertPlainText(QString::number(ptLast.y()));
    ui->textBrowser->insertPlainText("              ");

    scene->addEllipse(ptLast.x()-rad, ptLast.y()-rad, rad*2.0, rad*2.0,
        QPen(), QBrush(Qt::SolidPattern));

    int pathCoast = 0;
    for(int i=1;i<annealMethod.getCountItems();i++)
    {
        int x = std::get<0>(points[i]);
        int y = std::get<1>(points[i]);
        QPointF pt = QPointF(x,y);
        ui->textBrowser->insertPlainText(QString::number(x));
        ui->textBrowser->insertPlainText(",");
        ui->textBrowser->insertPlainText(QString::number(y));
        ui->textBrowser->insertPlainText("              ");
        pathCoast+=distanceSqr(points[i], points[i-1]);
        scene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0,
            QPen(), QBrush(Qt::SolidPattern));
        scene->addLine(ptLast.x(),ptLast.y(),pt.x(),pt.y(),pen);//x
        ptLast = pt;
    }
    scene->addLine(ptLast.x(),ptLast.y(),std::get<0>(points[0]),std::get<1>(points[0]),pen);//x
    pathCoast+=distanceSqr(points[annealMethod.getCountItems()-1], points[0]);

    ui->textBrowser_2->clear();
    ui->textBrowser_2->insertPlainText(QString::number(pathCoast));

    ui->textBrowser_5->clear();
    ui->textBrowser_5->insertPlainText(QString::number(annealMethod.getCountBurns()));
    ui->graphicsView->setScene(scene);
}


//void MainWindow::calcPath(){
//    pathCoast = 0;
//    for(int i=1;i<countItems;i++)
//    {
//        int x = std::get<0>(points[i]);
//        int y = std::get<1>(points[i]);
//        pathCoast+=distanceSqr(points[i], points[i-1]);
//    }
//    pathCoast+=distanceSqr(points[countItems-1], points[0]);
//    countBurns++;
//}

//void MainWindow::burn()
//{
//    int itm1 = random_int(0, countItems-1);
//    int itm2 = random_int(0, countItems-1);
//    RandomPlacement(itm1, itm2);

//    calcPath();
//    string d = "замена "+to_string(itm1)+ " на "+to_string(itm2)+".  Температура "+to_string(temp)+".  ";
//    d+="лучший результат - "+to_string(bestPathCoast)+ " новый - "+to_string(pathCoast)+".   ";
//    if(pathCoast<bestPathCoast) // новое решение лучше, запомним его
//    {
//        d+=". новое решение лучше  ";
//        bestPathCoast = pathCoast;
//        bestPoints.assign(points.begin(), points.end());
//    }
//    else
//    {
//        d+=". новое решение хуже  ";
//        double pstar = 100*exp(-(pathCoast-bestPathCoast)/(temp*bestPathCoast) );
//        double pcrit = random_int(1, 100);
//        d+=". p*  "+to_string(pstar)+ " pk "+to_string(pcrit)+".  ";
//        if(pstar<pcrit) //ну и фиг с ним
//        {
//           d+=". так себе ход. вовращаем на  "+to_string(bestPathCoast)+".   ";
//           points.assign(bestPoints.begin(), bestPoints.end());
//           drawWithPath();
//        }
//    }
//    temp = 0.8*temp;
//}

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser_6->clear();
    unsigned int start_time =  clock();

    while(annealMethod.isTemperatureHigh())
    {
        annealMethod.burn();
    }
    drawWithPath();
    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    ui->textBrowser_6->insertPlainText(QString::number(search_time/1000.0));
}

MainWindow::~MainWindow()
{
    delete ui;
}


