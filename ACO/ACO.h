//
// Created by brend on 08.10.2023.
//

#ifndef TEST_ACO_H
#define TEST_ACO_H
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>
#include <random>

using namespace std;

class ACO {

    const int _N;
    const double _alpha;
    const double _beta;
    const double _e;
    const double _Q;
    const double _tau0;
    double _length;
    vector<int> _path;

    struct Ant {
        explicit Ant(int startPosition);
        vector<int> tabuList;
    };

    vector<vector<double>> _distanceMatrix; // Матрица расстояний между городами
    vector<vector<double>> _pheromoneMatrix; // Матрица феромонов на рёбрах
    vector<Ant> _ants;


    void initializeMatrices();
    double calculateProbability(int, int, Ant);
    void constructAntSolutions();
    void resetTabuList(vector<Ant>);
    void updatePheromones();
public:

    void showPath();
    void setDistanceMatrix(vector<vector<double>>);
    ACO(int N, double alpha, double beta, double e, double Q, double tau0);
    int makeChoise(vector<double>);
    void showMatrices();
    void optimize(int);
    double getLength();
    vector<int> getPath();
    vector<vector<double>> getDistanceMatrix();
    vector<vector<double>> getPheromoneMatrix();

};


#endif //TEST_ACO_H
