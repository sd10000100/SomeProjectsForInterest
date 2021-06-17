#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>                                                                           ф
#include <string>
#include <algorithm>
#include <tuple>
#include <math.h>
#include <QThread>

using namespace std;

vector<tuple<int, int>> points;
vector<tuple<int, int>> startPoints;
vector<tuple<int, int>> bestPoints;
vector<vector<tuple<int, int>>> chromosomes;
int pathCoast = INT_MAX;
int bestPathCoast = INT_MAX ;
double temp = 100;
int countItems = 10;
int countBurns = 0;

struct Chromosome{
    vector<tuple<int, int>> points;


};

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand (2);
    for(int i=0;i<countItems;i++)
    {
        int x = rand() %301 - 100;
        int y = rand() %301 - 100;
        points.push_back(make_tuple(x,y));
    }
    std::vector<tuple<int, int>> startPoints(points);
    std::vector<tuple<int, int>> bestPoints(points);
    drawWithPath();
    bestPathCoast = pathCoast;
    countBurns = 0;
}

void MainWindow::drawWithPath()
{
    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
    QPen pen(Qt::green);
    scene->clear();
    ui->textBrowser->clear();
    double rad = 2;
    QPointF ptLast = QPointF(std::get<0>(points[0]),std::get<1>(points[0]));
    ui->textBrowser->insertPlainText(QString::number(ptLast.x()));
    ui->textBrowser->insertPlainText(",");
    ui->textBrowser->insertPlainText(QString::number(ptLast.y()));
    ui->textBrowser->insertPlainText("              ");
    scene->addEllipse(ptLast.x()-rad, ptLast.y()-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));
    pathCoast = 0;
    for(int i=1;i<countItems;i++)
    {
        int x = std::get<0>(points[i]);
        int y = std::get<1>(points[i]);
        QPointF pt = QPointF(x,y);
        ui->textBrowser->insertPlainText(QString::number(x));
        ui->textBrowser->insertPlainText(",");
        ui->textBrowser->insertPlainText(QString::number(y));
        ui->textBrowser->insertPlainText("              ");
        pathCoast+=distanceSqr(points[i], points[i-1]);
        scene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));
        scene->addLine(ptLast.x(),ptLast.y(),pt.x(),pt.y(),pen);//x
        ptLast = pt;
    }
    scene->addLine(ptLast.x(),ptLast.y(),std::get<0>(points[0]),std::get<1>(points[0]),pen);//x
    pathCoast+=distanceSqr(points[countItems-1], points[0]);

    ui->textBrowser_2->clear();
       ui->textBrowser_2->insertPlainText(QString::number(pathCoast));

    countBurns++;
    ui->textBrowser_5->clear();
       ui->textBrowser_5->insertPlainText(QString::number(countBurns));
    ui->graphicsView->setScene(scene);
}

//void MainWindow::on_pushButton_clicked()
//{
//    while(temp>0.0000000001)
//    {

//        burn();
////QThread::sleep(2);
//        //sleep(1000);
//    }
//}

void MainWindow::mutate()
{

}

void MainWindow::on_pushButton_clicked()
{
    chromosomes={};
    chromosomes[0] = RandomPlacement(points, countItems);
    chromosomes[1] = RandomPlacement(points, countItems);
    chromosomes[2] = RandomPlacement(points, countItems);
    while(temp>0.0000000001)
    {

        mutate();
//QThread::sleep(2);
        //sleep(1000);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


