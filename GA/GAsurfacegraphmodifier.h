#ifndef GASURFACEGRAPHMODIFIER_H
#define GASURFACEGRAPHMODIFIER_H

#include <QtDataVisualization/q3dsurface.h>
#include <QtDataVisualization/qsurfacedataproxy.h>
#include <QtDataVisualization/qheightmapsurfacedataproxy.h>
#include <QtDataVisualization/qsurface3dseries.h>
#include <QtDataVisualization/qcustom3ditem.h>
#include <QtDataVisualization/qcustom3dlabel.h>
#include <QtDataVisualization/q3dinputhandler.h>
#include <QtWidgets/qslider.h>
#include <QtWidgets/qlabel.h>
#include <QtCore/qpropertyanimation.h>
#include "GA/genetic.cpp"

class TopographicSeries;
class HighlightSeries;
class CustomInputHandler;

class GASurfaceGraphModifier : public QObject
{
    Q_OBJECT
public:
    explicit GASurfaceGraphModifier(Q3DSurface *surface, QLabel *label, QObject *parent);

    static double firstFunc(double x, double y)
    {
        return pow(x, 2) + 3 * pow(y, 2) + 2 * x * y;
    }

    static double secondFunc(double x, double y)
    {
        return 100 * pow((y - pow(x, 2)), 2) + pow((1 - x), 2);
    }

    static double twelfthFunc(double x, double y)
    {
        return 8 * pow(x, 2) + 4 * x * y + 5 * pow(y, 2);
    }

    //Rosenbrock function
    //Global minimum: f(1,1) = 0
    //Ranges: {-1.5 <= x <= 2} and {-0.5 <= y <= 3}
    static double rosenbrock(double x, double y)
    {
        return pow(1 - x, 2) + 100 * pow(y - pow(x, 2), 2);
    }

    //Himmelblau function
    //Global minimum: f(3,2) = 0
    //Global minimum: f(-2.805118, 3.131312) = 0
    //Global minimum: f(-3.779310, -3.283186) = 0
    //Global minimum: f(3.584428, -1.848126) = 0
    //Ranges = {-5 <= x,y <= 5}
    static double himmelblau(double x, double y)
    {
        return pow(pow(x,2) + y - 11, 2) + pow((x + pow(y, 2) - 7), 2);
    }

    //Booth Function
    //Global minimum: f(1,3) = 0
    //Ranges = {-10 <= x,y <= 10}
    static double booth(double x, double y)
    {
        return pow(x + 2*y - 7, 2) + pow(2*x + y - 5, 2);
    }

    void changeGenerationSize(int count)
    {
        this->generationSize = count;
    }

    void changeSampleSize(int count)
    {
        this->sampleSize = count;
    }

    void toggleModeNone()
    {
        m_graph->removeCustomItemAt(positionOne);
        GeneticAlgorithm sln;
        switch (m_funcNumber)
        {
        case 0:
            sln.setOptimizedFunction(himmelblau);
            break;
        case 1:
            sln.setOptimizedFunction(rosenbrock);
            break;
        case 2:
            sln.setOptimizedFunction(booth);
            break;
        case 3:
            sln.setOptimizedFunction(firstFunc);
            break;
        case 4:
            sln.setOptimizedFunction(secondFunc);
            break;
        case 5:
            sln.setOptimizedFunction(twelfthFunc);
            break;
        }
        sln.setRanges(m_graph->axisX()->min(),
                      m_graph->axisX()->max(),
                      m_graph->axisZ()->min(),
                      m_graph->axisZ()->max());
        sln.setGenerationSize(this->generationSize);
        sln.setSampleSize(this->sampleSize);

        m_codeNumber == 0 ? sln.setCodingType(Types::REAL) : sln.setCodingType(Types::GRAYCODE);

        std::random_device device;
        std::uniform_real_distribution<double> unifx(sln.xmin, sln.xmax);
        std::uniform_real_distribution<double> unify(sln.ymin, sln.ymax);

        std::vector<SolutionGray> solutionsGray;
        std::vector<Solution> solutions;

        AnswerGray ansGray;
        Answer ans;
        std::vector<double> vector = {0, 0};

        if (sln.type == Types::GRAYCODE)
        {
            for (int i = 0; i < sln.generationSize; i++)
                solutionsGray.push_back(SolutionGray{
                    0,
                    (unsigned)((unifx(device) - sln.xmin) / (sln.xmax - sln.xmin) * (UINT_MAX)),
                    (unsigned)((unify(device) - sln.ymin) / (sln.ymax - sln.ymin) * (UINT_MAX))
                });

            float formerMin = INT_MAX;
            float min = INT_MAX - 1000;
            SolutionGray decoder;
            while (true)
            {
                ansGray = sln.FindMinimumGray(solutionsGray);
                solutionsGray = ansGray.solution;
                min = ansGray.leastMin;
                vector = {((double)decoder.graydecode((ans.solution[0].x) / UINT_MAX) * (sln.xmax - sln.xmin) + sln.xmin),
                          ((double)decoder.graydecode((ans.solution[0].y) / UINT_MAX) * (sln.ymax - sln.ymin) + sln.ymin)};

                if (abs(formerMin - min) < 1e-6)
                    break;
                else formerMin = min;
            }
        }
        else
        {
            for (int i = 0; i < sln.generationSize; i++)
                solutions.push_back(Solution{
                    0,
                    unifx(device),
                    unify(device)
                });

            int generation = 0;
            float formerMin = INT_MAX;
            float min = INT_MAX - 1000;
            while (true)
            {
                Answer ans = sln.FindMinimum(solutions);
                solutions = ans.solution;
                min = ans.leastMin;
                vector = {ans.solution[0].x, ans.solution[0].y};

                if (abs(formerMin - min) < 1e-6)
                    break;
                else formerMin = min;
            }
        }

        QImage color = QImage(2, 2, QImage::Format_RGB32);
        color.fill(Qt::red);
        switch (m_funcNumber)
        {
        case 0:
            positionOne = QVector3D(vector[0], himmelblau(vector[0], vector[1])/200.0, vector[1]);
            break;
        case 1:
            positionOne = QVector3D(vector[0], rosenbrock(vector[0], vector[1])/20000.0, vector[1]);
            break;
        case 2:
            positionOne = QVector3D(vector[0], booth(vector[0], vector[1])/400.0, vector[1]);
            break;
        case 3:
            positionOne = QVector3D(vector[0], firstFunc(vector[0], vector[1])/100.0, vector[1]);
            break;
        case 4:
            positionOne = QVector3D(vector[0], secondFunc(vector[0], vector[1])/20000.0, vector[1]);
            break;
        case 5:
            positionOne = QVector3D(vector[0], twelfthFunc(vector[0], vector[1])/300.0, vector[1]);
            break;
        }


        QCustom3DItem *item = new QCustom3DItem("C:/Qt/Examples/Qt-6.6.1/datavisualization/graphgallery/data/oilrig.obj", positionOne,
                                                QVector3D(0.025f, 0.025f, 0.025f),
                                                QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 45.0f),
                                                color);
        m_graph->addCustomItem(item);
    }

    void toggleModeItem() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                          | QAbstract3DGraph::SelectionSlice
                                                          | QAbstract3DGraph::SelectionMultiSeries); }
    void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                             | QAbstract3DGraph::SelectionSlice
                                                             | QAbstract3DGraph::SelectionMultiSeries); }

    void setBlackToYellowGradient();
    void setGreenToRedGradient();

    void setAxisMinSliderX(QSlider *slider) { m_axisMinSliderX = slider; }
    void setAxisMaxSliderX(QSlider *slider) { m_axisMaxSliderX = slider; }
    void setAxisMinSliderZ(QSlider *slider) { m_axisMinSliderZ = slider; }
    void setAxisMaxSliderZ(QSlider *slider) { m_axisMaxSliderZ = slider; }

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);

    void changeFunction(int f);
    void changeCoding(int f);

public Q_SLOTS:
    void enableSqrtSinModel(bool enable);
    void enableHeightMapModel(bool enable);
    void enableTopographyModel(bool enable);

    void toggleItemOne(bool show);
    void toggleItemTwo(bool show);
    void toggleItemThree(bool show);
    void toggleSeeThrough(bool seethrough);
    void toggleOilHighlight(bool highlight);
    void toggleShadows(bool shadows);
    void toggleSurfaceTexture(bool enable);

private:
    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
    void fillSqrtSinProxy(int flag);
    void handleElementSelected(QAbstract3DGraph::ElementType type);
    void resetSelection();

private:
    QVector3D positionOne = QVector3D(10.0, 10.0, 10.0);

    int m_funcNumber = 0;
    int m_codeNumber = 0;

    Q3DSurface *m_graph = nullptr;
    QSurfaceDataProxy *m_sqrtSinProxy = nullptr;
    QSurface3DSeries *m_sqrtSinSeries = nullptr;
    QHeightMapSurfaceDataProxy *m_heightMapProxyOne = nullptr;
    QHeightMapSurfaceDataProxy *m_heightMapProxyTwo = nullptr;
    QHeightMapSurfaceDataProxy *m_heightMapProxyThree = nullptr;
    QSurface3DSeries *m_heightMapSeriesOne = nullptr;
    QSurface3DSeries *m_heightMapSeriesTwo = nullptr;
    QSurface3DSeries *m_heightMapSeriesThree = nullptr;

    QSlider *m_axisMinSliderX = nullptr;
    QSlider *m_axisMaxSliderX = nullptr;
    QSlider *m_axisMinSliderZ = nullptr;
    QSlider *m_axisMaxSliderZ = nullptr;
    float m_rangeMinX = 0.f;
    float m_rangeMinZ = 0.f;
    float m_stepX = 0.f;
    float m_stepZ = 0.f;
    int m_heightMapWidth = 0;
    int m_heightMapHeight = 0;
    int generationSize = 1000;
    int sampleSize = 100;

    QLabel *m_textField = nullptr;
    QPropertyAnimation *m_selectionAnimation = nullptr;
    QCustom3DLabel *m_titleLabel = nullptr;
    QCustom3DItem *m_previouslyAnimatedItem = nullptr;
    QVector3D m_previousScaling = {};

    TopographicSeries *m_topography = nullptr;
    HighlightSeries *m_highlight = nullptr;
    int m_highlightWidth = 0;
    int m_highlightHeight = 0;

    CustomInputHandler *m_customInputHandler = nullptr;
    Q3DInputHandler *m_defaultInputHandler = new Q3DInputHandler();
};

#endif // GASURFACEGRAPHMODIFIER_H
