#ifndef SWARMPARTICLESOPT_SPO_H
#define SWARMPARTICLESOPT_SPO_H
#include <vector>
#include <random>

class SPO {

    double bestSwarmPosition[2]{};
    double alpha;
    double beta;
    double maxX;
    double minX;
    double maxY;
    double minY;
    int maxIteration;
    int particleCount;
    int iteration;
    bool needInit;
    bool endOptimization;

    double (*function)(double, double);

    std::random_device device;

    enum Coordinate {
        X = (int)0,
        Y = 1
    };

    struct Particle {

        double position[2] {};
        double speed[2] {};
        double _bestPosition[2];

        Particle(int xPos, int yPos, int xSpeed, int ySpeed);
    };

    std::vector<Particle> particles;

    void updateSpeed(Particle &p);
    void initParameters();

public:
    SPO(double (*func)(double x, double y) = nullptr, double minX = 0, double maxX = 0, double minY = 0, double maxY = 0, int particles = 20, int maxIteration= 0);

    std::vector<Particle> getParticles();
    void setOptimizedFunction(double (*function)(double x, double y));
    void setRanges(double minX, double maxX, double minY, double maxY);
    void setMaxIteration(int count);
    void setParticleCount(int count);
    void reset();

    std::vector<double> optimize();
    std::vector<double> optimizeByStep();
};


#endif //SWARMPARTICLESOPT_SPO_H
