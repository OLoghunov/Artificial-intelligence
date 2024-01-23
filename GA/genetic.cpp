#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <algorithm>

enum class Types
{
    REAL, GRAYCODE
};

class Solution
{
public:
    double ans, x, y;

    void Fitness(double (*func)(double, double))
    {
        ans = func(x, y);
    }
};

class SolutionGray
{
public:
    unsigned int graydecode(unsigned int gray)
    {
        unsigned int bin;
        for (bin = 0; gray; gray >>= 1) {
            bin ^= gray;
        }
        return bin;
    }

    double ans;

    unsigned x, y;

    void Fitness(double (*func)(double, double), double xmin, double xmax, double ymin, double ymax)
    {
        double realx = ((double)graydecode(x) / UINT_MAX) * (xmax - xmin) + xmin;
        double realy = ((double)graydecode(y) / UINT_MAX) * (ymax - ymin) + ymin;

        ans = func(realx, realy);
    }
};

struct Answer
{
    std::vector<Solution> solution;
    double leastMin;
};

struct AnswerGray
{
    std::vector<SolutionGray> solution;
    double leastMin;
};

class GeneticAlgorithm
{
    double (*function)(double, double);
public:
    double xmin = -100, xmax = 100, ymin = -100, ymax = 100;
    double generationSize = 1000;
    double sampleSize = 100;
    Types type = Types::REAL;

    Answer FindMinimum(std::vector<Solution> solutions)
    {
        // Initialization
        //
        std::random_device device;

        // Run out fitness function
        //
        for (auto& s : solutions) { s.Fitness(function); }

        // Sort our solutions by rank
        //
        std::sort(solutions.begin(), solutions.end(),
                  [](const auto& lhs, const auto& rhs) {
                      return lhs.ans < rhs.ans;
                  });

        double leastAns = solutions[0].ans;

        // Take out top 100 solutions
        //
        std::vector<Solution> sample;
        std::copy(
            solutions.begin(),
            solutions.begin() + sampleSize,
            std::back_inserter(sample));
        solutions.clear();

        // Mutate our top solutions by %
        //
        std::uniform_real_distribution<float> m(0.99, 1.01);
        std::for_each(sample.begin(),
                      sample.end(),
                      [&](auto& s) {
                          s.x *= m(device);
                          s.y *= m(device);
                      });

        // Cross over
        //
        std::uniform_int_distribution<int> cross(0, sampleSize - 1);
        for (int i = 0; i < generationSize; i++)
        {
            solutions.push_back(Solution{
                0,
                (sample[cross(device)].x + sample[cross(device)].x) / 2,
                (sample[cross(device)].y + sample[cross(device)].y) / 2
            });
        }
        return Answer{ solutions, leastAns };
    }

    AnswerGray FindMinimumGray(std::vector<SolutionGray> solutions)
    {
        // Initialization
        //
        std::random_device device;

        const int NUM = 1000;

        // Run out fitness function
        //
        for (auto& s : solutions) { s.Fitness(function, xmin, xmax, ymin, ymax); }

        // Sort our solutions by ans
        //
        std::sort(solutions.begin(), solutions.end(),
                  [](const auto& lhs, const auto& rhs) {
                      return lhs.ans < rhs.ans;
                  });

        double leastAns = solutions[0].ans;

        // Take out top 100 solutions
        //
        const int SAMPLE_SIZE = 100;
        std::vector<SolutionGray> sample;
        std::copy(
            solutions.begin(),
            solutions.begin() + SAMPLE_SIZE,
            std::back_inserter(sample));
        solutions.clear();

        // Mutate our top solutions by %
        //
        std::uniform_int_distribution<int> m(0, 31);
        std::for_each(sample.begin(),
                      sample.end(),
                      [&](auto& s) {
                          s.x = (unsigned)((int)s.x ^ (1 << m(device)));
                          s.y = (unsigned)((int)s.y ^ (1 << m(device)));
                      });

        // Cross over
        //
        unsigned mask = (~0) << m(device);
        std::uniform_int_distribution<int> cross(0, SAMPLE_SIZE - 1);
        for (int i = 0; i < NUM; i++)
        {
            solutions.push_back(SolutionGray{
                0,
                (unsigned)(sample[cross(device)].x & mask | sample[cross(device)].x & ~mask),
                (unsigned)(sample[cross(device)].y & mask | sample[cross(device)].y & ~mask)
            });
        }
        return AnswerGray{ solutions, leastAns };
    }

    void setOptimizedFunction(double (*function)(double x, double y))
    {
        this->function = function;
    }

    void setRanges(double xmin, double xmax, double ymin, double ymax)
    {
        this->xmin = xmin;
        this->xmax = xmax;
        this->ymin = ymin;
        this->ymax = ymax;
    }

    void setGenerationSize(double s)
    {
        this->generationSize = s;
    }

    void setSampleSize(double s)
    {
        this->sampleSize = s;
    }

    void setCodingType(Types type)
    {
        this->type = type;
    }
};
