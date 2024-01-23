#ifndef SURFACEGRAPHMODIFIER_H
#define SURFACEGRAPHMODIFIER_H

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
#include "SPO/SPO.h"

class TopographicSeries;
class HighlightSeries;
class CustomInputHandler;

class SurfaceGraphModifier : public QObject
{
    Q_OBJECT
public:
    explicit SurfaceGraphModifier(Q3DSurface *surface, QLabel *label, QObject *parent);

    static double firstFunc(double x, double y)
    {
        return pow(x, 2) + 3 * pow(y, 2) + 2 * x * y;
    }

    static double secondFunc(double x, double y)
    {
        return 4 * pow((x - 5), 2) + pow((y - 6), 2);
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

    void changeParticleCount(int count)
    {
        this->particleCount = count;
    }

    void changeMaxIteration(int count)
    {
        this->maxIteration = count;
    }

    void toggleModeNone()
    {
        m_graph->removeCustomItemAt(positionOne);
        SPO spo;
        spo.reset();
        switch (m_funcNumber)
        {
        case 0:
            spo.setOptimizedFunction(himmelblau);
            break;
        case 1:
            spo.setOptimizedFunction(rosenbrock);
            break;
        case 2:
            spo.setOptimizedFunction(booth);
            break;
        case 3:
            spo.setOptimizedFunction(firstFunc);
            break;
        case 4:
            spo.setOptimizedFunction(secondFunc);
            break;
        }

        spo.setRanges(m_graph->axisX()->min(),
                      m_graph->axisX()->max(),
                      m_graph->axisZ()->min(),
                      m_graph->axisZ()->max());
        spo.setParticleCount(this->particleCount);
        spo.setMaxIteration(this->maxIteration);
        auto vector = spo.optimize();


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
            positionOne = QVector3D(vector[0], firstFunc(vector[0], vector[1])/140.0, vector[1]);
            break;
        case 4:
            positionOne = QVector3D(vector[0], secondFunc(vector[0], vector[1])/400.0, vector[1]);
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
    int particleCount = 100;
    int maxIteration = 1000;

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

#endif // SURFACEGRAPHMODIFIER_H
