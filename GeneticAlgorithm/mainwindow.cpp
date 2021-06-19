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

int pathCoast = INT_MAX;
int bestPathCoast = INT_MAX ;
double temp = 100;
int countItems = 10;
int countBurns = 1;

double distanceSqr(tuple<int, int> a, tuple<int, int> b) {
    return (std::get<0>(a) - std::get<0>(b)) * (std::get<0>(a) - std::get<0>(b)) + (std::get<1>(a) - std::get<1>(b)) * (std::get<1>(a) - std::get<1>(b));
}

struct Chromosome{



    int getPathCoast() const
    {
        return pathCoast;
    }

    Chromosome(vector<tuple<int, int>> initPoints)
    {
        //std::vector<tuple<int, int>> points(initPoints);
        points = initPoints;
        pathCoast = 0;
        for(int i=1;i<points.size();i++)
        {
            int x = std::get<0>(points[i]);
            int y = std::get<1>(points[i]);
            pathCoast+=distanceSqr(points[i], points[i-1]);
        }
        pathCoast+=distanceSqr(points[countItems-1], points[0]);
    }

    void RandomPlacement(int ind1, int ind2)
    {
        swap(points[ind1],points[ind2]);
    }

    void MutateByInversion(int numberOfStart, vector<tuple<int, int>>  newpoints)
    {
        //std::vector<tuple<int, int>> points(newpoints);
        points = newpoints;
        reverse( points.begin()+numberOfStart, points.end());
        pathCoast = 0;
        for(int i=1;i<points.size();i++)
        {
            int x = std::get<0>(points[i]);
            int y = std::get<1>(points[i]);
            pathCoast+=distanceSqr(points[i], points[i-1]);
        }
        pathCoast+=distanceSqr(points[countItems-1], points[0]);
    }

    int calcLength(){
        int tmp = 0;
        for(int i=1;i<points.size();i++)
        {
            int x = std::get<0>(points[i]);
            int y = std::get<1>(points[i]);
            tmp+=distanceSqr(points[i], points[i-1]);
        }
        tmp+=distanceSqr(points[countItems-1], points[0]);
        return tmp;
    }
    vector<tuple<int, int>> points;
    int pathCoast = INT_MAX;
private:



};

vector<Chromosome> chromosomes;

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
    countBurns = 1;
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
    //chromosomes

           // int worst = -1;
         //   int worstVal = -1;
         //   int i=0;
             Chromosome *best = nullptr;
             Chromosome *worst = nullptr;
    for (Chromosome &chromosome : chromosomes) {
            if(best==nullptr || best->pathCoast>=chromosome.pathCoast)
                    best = &chromosome;
            if(worst==nullptr || worst->pathCoast<=chromosome.pathCoast)
                    worst = &chromosome;
//            if(worst==-1 || worstVal>chromosome.getPathCoast())
//            {
//                worst = i;
//                worstVal = chromosome.getPathCoast();
//            }
//                    best = &chromosome;
//                    i++;
          //  ui->textBrowser_4->insertPlainText(QString::number(chromosome.getPathCoast()));
          //  ui->textBrowser_4->insertPlainText(",");

    }
 //ui->textBrowser_4->insertPlainText("                            ");
    worst->MutateByInversion(countBurns,best->points);
    //bestPathCoast =
//    auto it = std::find(v.begin(), v.end(), 5);
//    if(it != v.end())
//        v.erase(it);
    countBurns++;


}

void MainWindow::on_pushButton_clicked()
{
    chromosomes.push_back(points);//Chromosome(RandomPlacement(points, countItems)));
    chromosomes.push_back(Chromosome(RandomPlacement(points, countItems)));
    chromosomes.push_back(Chromosome(RandomPlacement(points, countItems)));
    while(countBurns<countItems-1)
    {
        mutate();
        //temp=0.9*temp;
        for (Chromosome &chromosome : chromosomes) {
                ui->textBrowser_4->insertPlainText(QString::number(chromosome.getPathCoast()));
                ui->textBrowser_4->insertPlainText(",");


        }
         ui->textBrowser_4->insertPlainText("                                    ");
    }
    Chromosome *best = nullptr;
    for (Chromosome &chromosome : chromosomes) {

            if(best==nullptr || best->getPathCoast()>=chromosome.getPathCoast())
                    best = &chromosome;
    }

    points = best->points;

    drawWithPath();
    countBurns = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}


