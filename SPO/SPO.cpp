#include "SPO.h"

SPO::Particle::Particle(int xPos, int yPos, int xSpeed, int ySpeed) {
    position[X] = xPos;
    position[Y] = yPos;
    speed[X] = xSpeed;
    speed[Y] = ySpeed;
}

void SPO::updateSpeed(Particle &p) {
    p.position[X] += p.speed[X];
    p.position[Y] += p.speed[Y];
    p.speed[X] += alpha * (p._bestPosition[X]) + beta * (bestSwarmPosition[X] - p.position[X]);
    p.speed[Y] += alpha * (p._bestPosition[Y]) + beta * (bestSwarmPosition[Y] - p.position[Y]);
}

void SPO::initParameters() {
    std::uniform_real_distribution<double> speed(0,1);
    std::uniform_real_distribution<double> posX(minX, maxX);
    std::uniform_real_distribution<double> posY(minY, maxY);

    bestSwarmPosition[0] = posX(device);
    bestSwarmPosition[1] = posY(device);

    particles.clear();

    //initialize particles
    for(int i = 0; i < particleCount; i++)
    {
        particles.push_back(Particle(posX(device), posY(device), speed(device), speed(device)));
        particles[i]._bestPosition[X] = particles[i].position[X];
        particles[i]._bestPosition[Y] = particles[i].position[Y];
        if(function(particles[i]._bestPosition[X], particles[i]._bestPosition[Y]) < function(bestSwarmPosition[(X)], bestSwarmPosition[Y]))
        {
            bestSwarmPosition[X] = particles[i]._bestPosition[X];
            bestSwarmPosition[Y] = particles[i]._bestPosition[Y];
        }
    }
}

std::vector<double> SPO::optimizeByStep() {
    if(endOptimization){
        return std::vector<double>{bestSwarmPosition[X], bestSwarmPosition[Y]};
    }
    if(needInit){
        initParameters();
        needInit = false;
    }

    std::uniform_real_distribution<double> coefficients(0, 1);

    for(Particle &P: particles){
        alpha = coefficients(device);
        beta = coefficients(device);
        updateSpeed(P);
        P.position[X] = P.position[X] + P.speed[X];
        P.position[Y] = P.position[Y] + P.speed[Y];
        if(function(P.position[X], P.position[Y]) < function(P._bestPosition[(X)], P._bestPosition[Y])){
            P._bestPosition[X] = P.position[X];
            P._bestPosition[Y] = P.position[Y];
            if(function(P.position[X], P.position[Y]) < function(bestSwarmPosition[(X)], bestSwarmPosition[Y])){
                bestSwarmPosition[X] = P.position[X];
                bestSwarmPosition[Y] = P.position[Y];
            }
        }
    }

    iteration++;
    if(iteration == maxIteration){
        needInit = true;
        endOptimization = true;
    }
    return std::vector<double>{bestSwarmPosition[X], bestSwarmPosition[Y]};
}

std::vector<double> SPO::optimize() {

    while(iteration < maxIteration) {
        optimizeByStep();
    }
    needInit = true;
    endOptimization = true;
    return std::vector<double>{bestSwarmPosition[X], bestSwarmPosition[Y]};
}

SPO::SPO(double (*function)(double x, double y), double minX, double maxX, double minY, double maxY, int maxIteration, int particles) {
    this->function = function;
    this->maxIteration = maxIteration;
    this->particleCount = particles;
    this->minX = minX;
    this->maxX = maxX;
    this->minY = minY;
    this->maxY = maxY;
    alpha = 0;
    beta = 0;
    iteration = 0;
    needInit = true;
    endOptimization = false;
}

void SPO::setParticleCount(int count)
{
    particleCount = count;
}

std::vector<SPO::Particle> SPO::getParticles()
{
    return this->particles;
}

void SPO::setOptimizedFunction(double (*function)(double x, double y))
{
    this->function = function;
}

void SPO::setRanges(double minX, double maxX, double minY, double maxY)
{
    this->minX = minX;
    this->maxX = maxX;
    this->minY = minY;
    this->maxY = maxY;
}

void SPO::setMaxIteration(int count) {
    this->maxIteration = count;
}

void SPO::reset() {
    iteration = 0;
    endOptimization = false;
}
