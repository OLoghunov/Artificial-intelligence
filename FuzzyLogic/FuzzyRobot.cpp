//
// Created by brend on 13.12.2023.
//

#include "FuzzyRobot.h"

double FuzzyRobot::trapezMembFunction::getValue(double x) { //todo можно сделать обработку ошибки: неизвестный x(не подходит ни под одно условие)
    if(x <= this->x[0] or x >= this->x[3])
        return 0;
    else if (x >= this->x[1] and x <= this->x[2])
        return 1;
    else if (x > this->x[0] and x < this->x[1])
        return (x - this->x[0]) / (this->x[1] - this->x[0]);
    else if(x > this->x[2] and x < this->x[3])
        return (this->x[3] - x) / (this->x[3] - this->x[2]);
}

int FuzzyRobot::getDistance(int angle) {

    std::pair<int, int> step = getStep(angle);
    int distance{0};

    int x{coordinates[X]},y{coordinates[Y]};
    while(field[y][x] != '#'){//1 - obstacle
        x += step.first;
        y += step.second;
        if (x == finishCoordinates[X] and y == finishCoordinates[Y]){
            finishFound = angle;
        }
    }

    if(abs(angle) == TOP | abs(angle) == BOTTOM)
        return abs(coordinates[Y] - y);
    else if (abs(angle) == LEFT | abs(angle) == RIGHT)
        return abs(coordinates[X] - x);
    return -1;
}

double FuzzyRobot::getDistance(std::pair<int, int> pair1, std::pair<int, int> pair2)
{
    return sqrt(pow(pair1.first - pair2.first, 2) + pow(pair1.second - pair2.second, 2));
}

std::vector<double> FuzzyRobot::getFuzzificationValue(int value) {
    std::vector<double> values{};
    for (int i = 0; i < membFunctions.size(); ++i)
        values.push_back(membFunctions[i].getValue(value));

    return values;
}

int FuzzyRobot::getLinguisticValue(std::vector<double> fuzzyValue) {
    double maxValue = -1;
    int maxIndex = 0;
    for(int i = 0; i < fuzzyValue.size(); i++){
        if(fuzzyValue[i] > maxValue){
            maxValue = fuzzyValue[i];
            maxIndex = i;
        }
    }

    if(maxIndex == 0)
        return CLOSE;
    else if (maxIndex == 1)
        return MIDDLE;
    else
        return LONG;
}

void FuzzyRobot::solutionByRules() {
    int forward = getLinguisticValue(getFuzzificationValue(getDistance(angle)));
    int right = getLinguisticValue(getFuzzificationValue(getDistance((angle + 90) % 360 )));
    int left = getLinguisticValue(getFuzzificationValue(getDistance((angle - 90) % 360 )));

    if(finishFound != -1){
        if(finishFound == angle - 90)
            angle -= 90;
        else if (finishFound == angle + 90)
            angle += 90;
    }
    else if(forward == CLOSE){
        if(left == CLOSE and right == CLOSE)
            angle = (angle + 180) % 360;
        else if(right == CLOSE)
            angle -= 90;
        else if(left == CLOSE)
            angle += 90;
        else if(right != CLOSE and left != CLOSE){
            if(rand(gen))
                angle += 90;
            else
                angle -=90;
        }
    }
    else {    if(left != CLOSE and right != CLOSE)
        {
            auto position = std::pair(coordinates[X],coordinates[Y]);
            auto futureStep = getStep(angle);
            auto futurePositionFront = position;
            futurePositionFront.first += futureStep.first;
            futurePositionFront.second += futureStep.second;
            futureStep = getStep((angle + 90) % 360);
            auto futurePositionRight = position;
            futurePositionRight.first += futureStep.first;
            futurePositionRight.second += futureStep.second;
            futureStep = getStep((angle - 90) % 360);
            auto futurePositionLeft = position;
            futurePositionLeft.first += futureStep.first;
            futurePositionLeft.second += futureStep.second;
            auto frontPoint = getDistance(futurePositionFront, std::pair(finishCoordinates[X], finishCoordinates[Y]));
            auto rightPoint = getDistance(futurePositionRight, std::pair(finishCoordinates[X], finishCoordinates[Y]));        auto leftPoint = getDistance(futurePositionLeft, std::pair(finishCoordinates[X], finishCoordinates[Y]));
            if(leftPoint < frontPoint and leftPoint < rightPoint)
                angle = (angle - 90) % 360;
            if(rightPoint < leftPoint and rightPoint < frontPoint)
                angle = (angle + 90) % 360;
        }    else if(left == CLOSE){
            auto position = std::pair(coordinates[X],coordinates[Y]);        auto futureStep = getStep(angle);
            auto futurePositionFront = position;        futurePositionFront.first += futureStep.first;
            futurePositionFront.second += futureStep.second;        futureStep = getStep((angle + 90) % 360);
            auto futurePositionRight = position;        futurePositionRight.first += futureStep.first;
            futurePositionRight.second += futureStep.second;        auto frontPoint = getDistance(futurePositionFront, std::pair(finishCoordinates[X], finishCoordinates[Y]));
            auto rightPoint = getDistance(futurePositionRight, std::pair(finishCoordinates[X], finishCoordinates[Y]));        if(rightPoint < frontPoint)
                angle = (angle + 90) % 360;    }
        else if(right == CLOSE){        auto position = std::pair(coordinates[X],coordinates[Y]);
            auto futureStep = getStep(angle);        auto futurePositionFront = position;
            futurePositionFront.first += futureStep.first;        futurePositionFront.second += futureStep.second;
            futureStep = getStep((angle - 90) % 360);        auto futurePositionLeft = position;
            futurePositionLeft.first += futureStep.first;        futurePositionLeft.second += futureStep.second;
            auto frontPoint = getDistance(futurePositionFront, std::pair(finishCoordinates[X], finishCoordinates[Y]));        auto leftPoint = getDistance(futurePositionLeft, std::pair(finishCoordinates[X], finishCoordinates[Y]));
            if(leftPoint < frontPoint)            angle = (angle - 90) % 360;
        }}
    // if(stepsHistory.size() > 1  && dejaVu()){
    //     if(left != CLOSE and right != CLOSE){
    //         if(rand(gen))
    //             angle += 90;
    //         else
    //             angle -=90;
    //     }
    //     else if( left == CLOSE and right != CLOSE)
    //         angle += 90;
    //     else if(left != CLOSE and right == CLOSE)
    //         angle -= 90;
    // }
    angle %= 360;
}

void FuzzyRobot::moveForward() {
    std::pair<int, int> step = getStep(angle);
    coordinates[X] += step.first;
    coordinates[Y] += step.second;

}

std::pair<int,int> FuzzyRobot::getStep(int angle) {
    if(angle == 0 | angle == 360 | angle == -360)
        return std::pair<int, int>(0,-1);
    else if(angle == 90 | angle == -270)
        return std::pair<int, int>(1,0);
    else if(angle == 180 | angle == -180)
        return std::pair<int, int>(0,1);
    else if(angle == -90 | angle == 270)
        return std::pair<int, int>(- 1,0);
}

void FuzzyRobot::makeStep() {
    if(!isFinished){
        stepsHistory.push_back(std::pair<int,int>(coordinates[X],coordinates[Y]));
        if (stepsHistory.size() == 51)
            stepsHistory.erase(stepsHistory.begin());
        solutionByRules();
        if(getLinguisticValue(getFuzzificationValue(getDistance(angle))) != CLOSE)
            moveForward();
        if(coordinates[X] == finishCoordinates[X] and coordinates[Y] == finishCoordinates[Y]){
            isFinished = true;
            std::cout << "END!";
        }
    }
}

FuzzyRobot::FuzzyRobot() {
    isFinished = false;
    finishFound = -1;

    angle = RIGHT;

    membFunctions.push_back(trapezMembFunction(-1,0,1,2));
    membFunctions.push_back(trapezMembFunction(1,2,5,6));
    membFunctions.push_back(trapezMembFunction(5,6,50,51));
    gen = std::mt19937(rd());
    rand = std::uniform_int_distribution<>(0,1);
}

void FuzzyRobot::showInConsole() {
    for(int i = 0; i < field.size(); i++){
        for(int j = 0; j < field[i].size(); j++){
            if(coordinates[X] == j and coordinates[Y] == i)
                std::cout << "#";
            else if (finishCoordinates[X] == j and finishCoordinates[Y] == i)
                std::cout << "X";
            else
                std::cout << field[i][j];

        }
        std::cout << std::endl;
    }
}

bool FuzzyRobot::dejaVu() {
    auto position = std::pair<int, int>(coordinates[X], coordinates[Y]);
    auto futureStep = getStep(angle);
    auto futurePosition = position;
    futurePosition.first += futureStep.first;
    futurePosition.second += futureStep.second;
    for(auto i = stepsHistory.begin(); i < stepsHistory.end()-1; i++)
        if(*i == position and *(i+1) == futurePosition)
            return true;
    return false;
}

void FuzzyRobot::setField(std::vector<std::vector<char>> field)
{
    this->field = field;
}

void FuzzyRobot::setStartPosition(int startX, int startY)
{
    coordinates[X] = startX;
    coordinates[Y] = startY;
}

void FuzzyRobot::setFinishPosition(int finishX, int finishY)
{
    finishCoordinates[X] = finishX;
    finishCoordinates[Y] = finishY;
}
