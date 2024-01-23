//
// Created by brend on 13.12.2023.
//

#ifndef FUZZYROBOT_FUZZYROBOT_H
#define FUZZYROBOT_FUZZYROBOT_H
#include <queue>
#include <math.h>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>


class FuzzyRobot {
private:

    int coordinates[2];
    int finishCoordinates[2];
    int angle;
    int finishFound;
    bool isFinished;

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen; // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> rand;

    std::vector<std::vector<char>> field;

    std::vector<std::pair<int, int>> stepsHistory;

    enum {
        X = (int)0,
        Y = 1
    };

    enum {
        TOP = (int)0,
        RIGHT = 90,
        BOTTOM = 180,
        LEFT = 270
    };

    enum {
        CLOSE = (int)0,
        MIDDLE,
        LONG
    };

    struct trapezMembFunction{
        double x[4]{0,0,0,0};

        trapezMembFunction(double x1, double x2, double x3, double x4){
            x[0] = x1;
            x[1] = x2;
            x[2] = x3;
            x[3] = x4;
        }

        double getValue(double x);
    };

    std::vector<trapezMembFunction> membFunctions;

    int getDistance(int angle);
    double getDistance(std::pair<int, int> pair1, std::pair<int, int> pair2);
    std::vector<double> getFuzzificationValue(int value);
    int getLinguisticValue(std::vector<double> fuzzyValue);
    void solutionByRules();
    bool dejaVu();
    void moveForward();
    std::pair<int,int> getStep(int angle);

public:
    FuzzyRobot();
    void setField(std::vector<std::vector<char>> field);
    void setFinishPosition(int finishX, int finishY);
    void setStartPosition(int startX, int startY);
    void makeStep(); //todo
    void showInConsole();

    std::pair<int, int> getCoordinates()
    {
        return {coordinates[0], coordinates[1]};
    }
};


#endif //FUZZYROBOT_FUZZYROBOT_H
