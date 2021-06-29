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
int countBurns2 = 1;
int countBurns = 1;
int crossoveringCounter = 0;

//int random_0_countItems(){
//    return rand() % countItems;
//}


//int random_1_100()
//{
//    return rand() % 100 + 1;
//}

int random_int(int a, int b){
    return rand() % (b - a + 1) + a;
}

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

struct Chromosome{



    int getPathCoast() const
    {
        return pathCoast;
    }

    Chromosome()
    {
        //std::vector<tuple<int, int>> points(initPoints);
        points = {};
        pathCoast = 0;
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

    void MutateByRandom2Items()
    {
        RandomPlacement(random_int(1, countItems-1), random_int(1, countItems-1));
        pathCoast = 0;
        for(int i=1;i<points.size();i++)
        {
            int x = std::get<0>(points[i]);
            int y = std::get<1>(points[i]);
            pathCoast+=distanceSqr(points[i], points[i-1]);
        }
        pathCoast+=distanceSqr(points[countItems-1], points[0]);
    }

    void MutateByShuffle()
    {
       // points = newpoints;
        random_shuffle ( points.begin(), points.end() );
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


Chromosome crossbreeding(int breakIndex,const Chromosome ch1, const Chromosome ch2)
{
    int i=0;
    Chromosome newCh = Chromosome();

    std::vector<tuple<int, int>> tempPoints1(ch1.points);
    while(i<breakIndex)
    {
        newCh.points.push_back(ch1.points[i]);
        auto it = std::find(tempPoints1.begin(), tempPoints1.end(), newCh.points[i]);
        if(it != tempPoints1.end())
            tempPoints1.erase(it);
        i++;
    }
    while (i<ch2.points.size())
    {
        auto it = std::find(tempPoints1.begin(), tempPoints1.end(), ch2.points[i]);
        if(it != tempPoints1.end())
        {
            newCh.points.push_back(ch2.points[i]);
            tempPoints1.erase(it);
        }
        i++;
    }
    while (tempPoints1.size()!=0)
    {
        newCh.points.push_back(tempPoints1[0]);

        auto it2 = std::find(tempPoints1.begin(), tempPoints1.end(), newCh.points[i]);
        tempPoints1.erase(tempPoints1.begin());
        i++;
    }

    newCh.pathCoast = newCh.calcLength();
    return newCh;
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

   // countBurns++;
    ui->textBrowser_5->clear();
       ui->textBrowser_5->insertPlainText(QString::number(countBurns2));
    ui->graphicsView->setScene(scene);
}



void MainWindow::mutate()
{
    Chromosome *worst = nullptr;
    while(chromosomes.size()>countItems)
    {
        int p = 0;int i=0;worst = nullptr;
        for (Chromosome &chromosome : chromosomes) {
                if(worst==nullptr || worst->pathCoast<=chromosome.pathCoast)
                       { worst = &chromosome;p=i;}
                i++;

        }
        chromosomes.erase(chromosomes.begin() + p);

    }

             Chromosome *best = nullptr;

    for (Chromosome &chromosome : chromosomes) {
            if(best==nullptr || best->pathCoast>=chromosome.pathCoast)
                    best = &chromosome;
            if(worst==nullptr || worst->pathCoast<=chromosome.pathCoast)
                    worst = &chromosome;


    }

    int iter1 = random_int(1, countItems-1);
    int iter2 = random_int(1, countItems-1);
    auto autotmp1  = crossbreeding(countItems/2,chromosomes[iter1], chromosomes[iter2]);
    auto autotmp2  = crossbreeding(countItems/2,chromosomes[iter2], chromosomes[iter1]);


    chromosomes.push_back(autotmp1);
    chromosomes.push_back(autotmp2);
    int randitm = random_int(1, 100);
    //if( randitm<40)
        worst->MutateByInversion(countBurns,best->points);



    countBurns++;
    countBurns2++;


}

void MainWindow::on_pushButton_clicked()
{


    //while(countBurns<countItems-1)
    while(countBurns2<230)
    {
        chromosomes.push_back(points);//Chromosome(RandomPlacement(points, countItems)));
        for(int i=1;i<countItems;i++)
            chromosomes.push_back(Chromosome(RandomPlacement(points, countItems)));

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
  //  ui->textBrowser_4->insertPlainText("                                    ");
}

MainWindow::~MainWindow()
{
    delete ui;
}


