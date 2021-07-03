#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <tuple>
#include <vector>

using namespace std;

template<typename T>
class SimulatedAnnealing{
public:
    void Init(int countItems, vector<tuple<int, int>> startPoints,bool writeLog = true);
    void setPoints(int countItems, vector<tuple<int, int>> startPoints);
    void updatePath();
    void burn();
    void findOptimalPath();
    void swapItemsInPoints(int ind1, int ind2);

    bool isTemperatureHigh();

    int getCountItems();
    int getCountBurns();
    vector<tuple<int, int>> getBestWay();
private:
    const double EPS = 1e-11;
    vector<tuple<int, int>> tempPoints_;
    vector<tuple<int, int>> startPoints_;
    vector<tuple<int, int>> bestPoints_;

    int pathCoast_ = INT_MAX;
    int bestPathCoast_ = INT_MAX ;

    double temperature_ = 100;
    int countItems_ = 10;
    int countBurns_ = 0;
    string log = "";
};


#endif // SIMULATEDANNEALING_H
