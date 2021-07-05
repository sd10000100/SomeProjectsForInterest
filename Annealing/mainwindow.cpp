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
double all_time=0;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    srand (2423534654);
    // Загоняем точки в дело
    std::vector<tuple<int, int>> points;
    int countItems = 20;
    for(int i=0;i<countItems;i++)
    {
        int x = random_int(-100, 200);
        int y = random_int(-100, 200);
        points.push_back(make_tuple(x,y));
    }
    annealMethod.init(countItems, points);
    drawWithPath();
}


// Визуализация пути по точкам
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
    ui->textBrowser->insertPlainText("\n");


    scene->addEllipse(ptLast.x()-rad, ptLast.y()-rad, rad*2.0, rad*2.0,
        QPen(), QBrush(Qt::SolidPattern));
    for(int i=1;i<annealMethod.getCountItems();i++)
    {
        int x = std::get<0>(points[i]);
        int y = std::get<1>(points[i]);
        QPointF pt = QPointF(x,y);
        ui->textBrowser->insertPlainText(QString::number(x));
        ui->textBrowser->insertPlainText(",");
        ui->textBrowser->insertPlainText(QString::number(y));
        ui->textBrowser->insertPlainText("\n");
        scene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0,
            QPen(), QBrush(Qt::SolidPattern));
        scene->addLine(ptLast.x(),ptLast.y(),pt.x(),pt.y(),pen);//x
        ptLast = pt;
    }
    scene->addLine(ptLast.x(),ptLast.y(),std::get<0>(points[0]),std::get<1>(points[0]),pen);
    ui->textBrowser_2->clear();
    ui->textBrowser_2->insertPlainText(QString::number(annealMethod.getBestWayCoast()));
    ui->textBrowser_5->clear();
    ui->textBrowser_5->insertPlainText(QString::number(annealMethod.getCountBurns()));
    ui->graphicsView->setScene(scene);

}


void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser_6->clear();
    annealMethod.resetTemperature();
    annealMethod.findOptimalPath();
    drawWithPath();
    ui->textBrowser_4->clear();
    ui->textBrowser_4->insertPlainText(QString::fromStdString(annealMethod.getLog()));
    all_time+=annealMethod.getTime();
    ui->textBrowser_6->insertPlainText(QString::number(all_time/1000.0));

}

MainWindow::~MainWindow()
{
    delete ui;
}


