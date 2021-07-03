#include "simulatedannealing.h"
#include <math.h>

inline double distanceSqr(tuple<int, int> a, tuple<int, int> b) {
    return (std::get<0>(a) - std::get<0>(b)) * (std::get<0>(a) - std::get<0>(b)) + (std::get<1>(a) - std::get<1>(b)) * (std::get<1>(a) - std::get<1>(b));
}

inline double distance(tuple<int, int> a, tuple<int, int> b) {
    return sqrt((std::get<0>(a) - std::get<0>(b)) * (std::get<0>(a) - std::get<0>(b)) + (std::get<1>(a) - std::get<1>(b)) * (std::get<1>(a) - std::get<1>(b)));
}

inline int random_int(int a, int b){
    return rand() % (b - a + 1) + a;
}

inline vector<tuple<int, int>> RandomPlacement(vector<tuple<int, int>> a, int n)
{
    vector<tuple<int, int>> b(a);
    for(int i = 0; i < n; i++)
        swap(b[i],b[rand()%n]);
    return b;
}



template<typename T>
int SimulatedAnnealing<T>::getCountItems(){
    return countItems_;
}

template<typename T>
int SimulatedAnnealing<T>::getCountBurns(){
    return countBurns_;
}


template<typename T>
bool SimulatedAnnealing<T>::isTemperatureHigh(){
    return temperature_>EPS;
}

template<typename T>
vector<tuple<int, int>> SimulatedAnnealing<T>::getBestWay(){
    return bestPoints_;
}

template<typename T>
void SimulatedAnnealing<T>::swapItemsInPoints(int ind1, int ind2)
{
    swap(tempPoints_[ind1],tempPoints_[ind2]);
}

template<typename T>
void SimulatedAnnealing<T>::Init(int countItems, vector<tuple<int, int>> startPoints,bool writeLog)
{
//tempPoints_std::vector<tuple<int, int>> points_(startPoints);
     tempPoints_= startPoints;

    startPoints_ = startPoints;

    bestPoints_ = startPoints;

    countItems_= countItems;
    bestPathCoast_ = INT_MAX;
    countBurns_ = 0;
    pathCoast_ = INT_MAX;
    temperature_ = 100;
}

template<typename T>
void SimulatedAnnealing<T>::setPoints(int countItems, vector<tuple<int, int>> startPoints)
{
    countItems_= countItems;
    tempPoints_ = startPoints;
    startPoints_ = startPoints;
    bestPoints_ = startPoints;
    bestPathCoast_ = INT_MAX;
    countBurns_ = 0;
}

template<typename T>
void SimulatedAnnealing<T>::updatePath(){
    pathCoast_ = 0;
    for(int i=1;i<countItems_;i++)
    {
        int x = std::get<0>(tempPoints_[i]);
        int y = std::get<1>(tempPoints_[i]);
        pathCoast_+=distanceSqr(tempPoints_[i], tempPoints_[i-1]);
    }
    pathCoast_+=distanceSqr(tempPoints_[countItems_-1], tempPoints_[0]);
    countBurns_++;
}

template<typename T>
void SimulatedAnnealing<T>::burn(){
    int itm1 = random_int(0, countItems_-1);
    int itm2 = random_int(0, countItems_-1);
    swapItemsInPoints(itm1, itm2);
    updatePath();
    log+= "замена "+to_string(itm1)+ " на "+to_string(itm2)+".  Температура "+to_string(temperature_)+".  ";
    log+= "лучший результат - "+to_string(bestPathCoast_)+ " новый - "+to_string(pathCoast_)+".   ";
    if(pathCoast_<bestPathCoast_) // новое решение лучше, запомним его
    {
        log+=". новое решение лучше  ";
        bestPathCoast_ = pathCoast_;
        bestPoints_.assign(tempPoints_.begin(), tempPoints_.end());
    }
    else
    {
        log+=". новое решение хуже  ";
        double pstar = 100*exp(-(pathCoast_-bestPathCoast_)/(temperature_*bestPathCoast_) );
        double pcrit = random_int(1, 100);
        log+=". p*  "+to_string(pstar)+ " pk "+to_string(pcrit)+".  ";
        if(pstar<pcrit) //ну и фиг с ним
        {
           log+=". так себе ход. вовращаем на  "+to_string(bestPathCoast_)+".   ";
           tempPoints_.assign(bestPoints_.begin(), bestPoints_.end());
          // drawWithPath();
        }
    }
    temperature_ = 0.8*temperature_;
}


template<typename T>
void SimulatedAnnealing<T>::findOptimalPath(){
//    ui->textBrowser_6->clear();
//    unsigned int start_time =  clock();

//    while(isTemperature>EPS)
//    {
//        burn();
//    }
//    unsigned int end_time = clock(); // конечное время
//    unsigned int search_time = end_time - start_time; // искомое время
//    ui->textBrowser_6->insertPlainText(QString::number(search_time/1000.0));
}
