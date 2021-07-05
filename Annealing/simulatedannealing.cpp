#include "simulatedannealing.h"
#include <math.h>
#include <ctime>

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
double SimulatedAnnealing<T>::getTime(){
    return time_;
}

template<typename T>
void SimulatedAnnealing<T>::resetTemperature(){
    temperature_ = 100;
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
int SimulatedAnnealing<T>::getBestWayCoast(){
    return bestPathCoast_;
}

template<typename T>
void SimulatedAnnealing<T>::swapItemsInPoints(int ind1, int ind2)
{
    swap(tempPoints_[ind1],tempPoints_[ind2]);
}
template<typename T>
string SimulatedAnnealing<T>::getLog(){
    return log_;
}

template<typename T>
void SimulatedAnnealing<T>::init(int countItems, vector<tuple<int, int>> startPoints,bool writeLog)
{
    tempPoints_= startPoints;
    startPoints_ = startPoints;
    bestPoints_ = startPoints;
    countItems_= countItems;
    bestPathCoast_ = INT_MAX;
    countBurns_ = 0;
    pathCoast_ = INT_MAX;
    temperature_ = 100;
    time_ = 0;
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
    unsigned int start_time =  clock();

    //меняем местами 2 случайных элемента
    int itm1 = random_int(1, countItems_-1);
    int itm2 = random_int(1, countItems_-1);
    swapItemsInPoints(itm1, itm2);
    //считаем полученный путь
    updatePath();
    log_+= "замена "+to_string(itm1)+ " на "+to_string(itm2)+".  Температура "+to_string(temperature_)+".  ";
    log_+= "лучший результат - "+to_string(bestPathCoast_)+ " новый - "+to_string(pathCoast_)+".   ";
    if(pathCoast_<bestPathCoast_) // новое решение лучше, запомним его
    {
        log_+=". новое решение лучше  ";
        bestPathCoast_ = pathCoast_;
        bestPoints_.assign(tempPoints_.begin(), tempPoints_.end());
    }
    else // новое решение хуже. Дать ли шанс?
    {
        log_+=". новое решение хуже  ";
        double pstar = 100*exp(-(pathCoast_-bestPathCoast_)/(temperature_*bestPathCoast_) );
        double pcrit = random_int(1, 100);
        log_+=". p*  "+to_string(pstar)+ " pk "+to_string(pcrit)+".  ";
        if(pstar<pcrit) // ну и фиг с ним
        {
           log_+=". так себе ход. вовращаем на  "+to_string(bestPathCoast_)+".   ";
           tempPoints_.assign(bestPoints_.begin(), bestPoints_.end());
        }
    }
    log_+= "\n";
    temperature_ = 0.8*temperature_; // температура падает в любом случае
    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    time_ +=search_time;
}


template<typename T>
void SimulatedAnnealing<T>::findOptimalPath(){
    time_ = 0;
    unsigned int start_time =  clock();

    while(isTemperatureHigh())
    {
        burn();
    }
    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    time_ = search_time;
}
