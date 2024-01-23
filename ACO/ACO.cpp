#include "ACO.h"

ACO::ACO(int N, double alpha, double beta, double e, double Q, double tau0)
    : _N(N), _alpha(alpha), _beta(beta), _e(e), _Q(Q), _tau0(tau0) {
    initializeMatrices();
    _length = numeric_limits<double>::max();
}

ACO::Ant::Ant(int startPosition) {
    tabuList.push_back(startPosition);
}

void ACO::initializeMatrices() {

    _distanceMatrix = vector<vector<double>>(_N, vector<double>(_N, 0));
    _pheromoneMatrix = vector<vector<double>>(_N, vector<double>(_N, 0 ));

    // заполнение матрицы расстояний и матрицы феромонов
    for (int i = 0; i < _N; i++) {
        _ants.push_back(Ant(i));
        for (int j = 0; j < _N; j++) {
            if (i != j) {
                if(_distanceMatrix[i][j] == 0 or _distanceMatrix[j][i] == 0){
                    int distance = rand() % 100 + 1; // Случайные расстояния
                    _distanceMatrix[i][j] = distance;
                    _distanceMatrix[j][i] = distance;
                }
                _pheromoneMatrix[i][j] = _tau0; // Начальный уровень феромона
            } else {
                _distanceMatrix[i][j] = 0.0;
                _pheromoneMatrix[i][j] = 0.0;
            }
        }
    }
}

void ACO::showMatrices() {
    std::cout << "Distance matrix: \n";
    for(int i = 0; i < _N; i++){
        for(int j = 0; j< _N; j++){
            std::cout << _distanceMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\nPheromone matrix: \n";
    for(int i = 0; i < _N; i++){
        for(int j = 0; j< _N; j++){
            std::cout << _pheromoneMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void ACO::constructAntSolutions() {
    vector<double> probabilities;

        for(int edges = 0; edges < _N; edges++){
            for(Ant &ant : _ants){
                if(ant.tabuList.size() != _N){ //если еще не все города были пройдены
                    for(int city = 0; city < _N; city++)
                        probabilities.push_back(calculateProbability(ant.tabuList.back(), city, ant));
                    ant.tabuList.push_back(makeChoise(probabilities));
                } else {
                    ant.tabuList.push_back(ant.tabuList[0]);
                }
                probabilities.clear();
            }

        }
        for(Ant ant: _ants){
            double length = 0;
            for(int city = 0; city < _N; city++)
                length += _distanceMatrix[ant.tabuList[city]][ant.tabuList[city + 1]];
            if(length < _length){
                _length = length;
                _path.clear();
                _path.insert(_path.begin(), ant.tabuList.begin(), ant.tabuList.end());
            }
        }

        updatePheromones();
        resetTabuList(_ants);

}

double ACO::calculateProbability(int from, int to, Ant ant) {
    if(from == to || !(std::find(begin(ant.tabuList), end(ant.tabuList), to) == end(ant.tabuList)))// если в один и тот же город или в город, который уже посещал
        return 0;
    double numerator = pow((double)_pheromoneMatrix[from][to], _alpha) * pow((double)(1 / _distanceMatrix[from][to]), _beta);
    double denominator = 0;
    for(int city = 0; city < _N; city++){
        if(std::find(begin(ant.tabuList), end(ant.tabuList), city) == end(ant.tabuList)) { // в данный город муровей еще не заходил
            if(from != city)
                denominator += pow((double)_pheromoneMatrix[from][city], _alpha) * pow((double)(1 / _distanceMatrix[from][city]), _beta);
        }
    }
    return numerator / denominator;
}

void ACO::updatePheromones() {
    for(int i = 0; i< _N; i++){
        for(int j = 0; j < _N; j++)
            _pheromoneMatrix[i][j] *= _e;
    }
    for(int ant = 0; ant < _N; ant++){
        int start, end;
        for(int edge = 0; edge < _N-1; edge++){
            start = _ants[ant].tabuList[edge];
            end = _ants[ant].tabuList[edge + 1];
            _pheromoneMatrix[start][end] += _Q / _pheromoneMatrix[start][end];
        }
    }
}

int ACO::makeChoise(const vector<double> probabilities) {
    double sum = 0;
    for(double probabilitie : probabilities)
        sum += probabilitie;

    std::random_device device;
    std::uniform_real_distribution<double> a(0,1);
    double choise = a(device);

    double len = 0;
    for(int i = 0; i < probabilities.size(); i++) {
        len += probabilities[i];
        if (choise <= len)
            return i;
    }


    return -1;
}

void ACO::resetTabuList(vector<Ant> ants) {
    for(Ant ant: ants)
        ant.tabuList.clear();
}

void ACO::optimize(int cycles) {
    //showMatrices();
    for(int i = 0; i < cycles; i++){
        constructAntSolutions();
    }

}

double ACO::getLength() {
    return _length;
}

vector<int> ACO::getPath() {
    return _path;
}

void ACO::showPath() {
    for(int i = 0; i < _path.size(); i++){
        std::cout << _path[i];
        if (i != _path.size() - 1)
            std::cout << " -> ";
    }
    std::cout << std::endl;

}

void ACO::setDistanceMatrix(vector<vector<double>> distanceMatrix) {
    _distanceMatrix = distanceMatrix;
}

vector<vector<double>> ACO::getDistanceMatrix() {
    return _distanceMatrix;
}

vector<vector<double>> ACO::getPheromoneMatrix() {
    return _pheromoneMatrix;
}
