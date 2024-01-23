#include "surfacegraphmodifier.h"
#include "highlightseries.h"
#include "custominputhandler.h"

#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtCore/qmath.h>

using namespace Qt::StringLiterals;

const int sampleCountX = 150;
const int sampleCountZ = 150;
const int heightMapGridStepX = 6;
const int heightMapGridStepZ = 6;
const float sampleMin = -5.f;
const float sampleMax = 5.f;

const float areaWidth = 8000.f;
const float areaHeight = 8000.f;
const float aspectRatio = 0.1389f;
const float minRange = areaWidth * 0.49f;

SurfaceGraphModifier::SurfaceGraphModifier(Q3DSurface *surface, QLabel *label, QObject *parent) :
    QObject(parent),
    m_graph(surface),
    m_textField(label)
{
    m_graph->scene()->activeCamera()->setZoomLevel(85.f);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricRight);
    m_graph->activeTheme()->setType(Q3DTheme::ThemeStoneMoss);

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    // function
    //
    m_sqrtSinProxy = new QSurfaceDataProxy();
    m_sqrtSinSeries = new QSurface3DSeries(m_sqrtSinProxy);
    fillSqrtSinProxy(0);

    m_titleLabel = new QCustom3DLabel();

    m_highlight = new HighlightSeries();

    m_highlight->setMinHeight(minRange * aspectRatio);
    m_highlight->handleGradientChange(areaWidth * aspectRatio);

    m_customInputHandler = new CustomInputHandler(m_graph);
    m_customInputHandler->setHighlightSeries(m_highlight);
    m_customInputHandler->setAxes(m_graph->axisX(), m_graph->axisY(), m_graph->axisZ());
    m_customInputHandler->setLimits(0.f, areaWidth, minRange);
    m_customInputHandler->setAspectRatio(aspectRatio);
}

void SurfaceGraphModifier::fillSqrtSinProxy(int flag)
{
    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);

    //! [1]
    auto *dataArray = new QSurfaceDataArray;
    dataArray->reserve(sampleCountZ);
    for (int i = 0 ; i < sampleCountZ ; ++i) {
        auto *newRow = new QSurfaceDataRow;
        newRow->reserve(sampleCountX);
        float z = qMin(sampleMax, (i * stepZ + sampleMin));
        for (int j = 0; j < sampleCountX; ++j) {
            float x = qMin(sampleMax, (j * stepX + sampleMin));
            float y;
            if (flag == 0)
            {
                y = pow((x*x + z - 11), 2) + pow((x + z*z - 7), 2);
                y /= 200.0;
            }
            else if (flag == 1)
            {
                y = pow(1 - x, 2) + 100 * pow(z - pow(x, 2), 2);
                y /= 20000.0;
            }
            else if (flag == 2)
            {
                y = pow(x + 2*z - 7, 2) + pow(2*x + z - 5, 2);
                y /= 400.0;
            }
            else if (flag == 3)
            {
                y = pow(x, 2) + 3 * pow(z, 2) + 2 * x * z;
                y /= 140.0;
            }
            else if (flag == 4)
            {
                y = 4 * pow((x - 5), 2) + pow((z - 6), 2);
                y /= 400.0;
            }
            newRow->append(QSurfaceDataItem({x, y, z}));
        }
        dataArray->append(newRow);
    }

    m_sqrtSinProxy->resetArray(dataArray);
}

void SurfaceGraphModifier::enableSqrtSinModel(bool enable)
{
    if (enable) {
        //! [3]
        m_sqrtSinSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        m_sqrtSinSeries->setFlatShadingEnabled(true);

        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");
        m_graph->axisX()->setRange(sampleMin, sampleMax);
        m_graph->axisY()->setRange(0.f, 2.f);
        m_graph->axisZ()->setRange(sampleMin, sampleMax);
        m_graph->axisX()->setLabelAutoRotation(30.f);
        m_graph->axisY()->setLabelAutoRotation(90.f);
        m_graph->axisZ()->setLabelAutoRotation(30.f);

        m_graph->removeSeries(m_heightMapSeriesOne);
        m_graph->removeSeries(m_heightMapSeriesTwo);
        m_graph->removeSeries(m_heightMapSeriesThree);
        m_graph->removeSeries(m_topography);
        m_graph->removeSeries(m_highlight);

        m_graph->addSeries(m_sqrtSinSeries);
        //! [3]

        m_titleLabel->setVisible(false);
        m_graph->axisX()->setTitleVisible(false);
        m_graph->axisY()->setTitleVisible(false);
        m_graph->axisZ()->setTitleVisible(false);

        m_graph->axisX()->setTitle({});
        m_graph->axisY()->setTitle({});
        m_graph->axisZ()->setTitle({});

        m_graph->setActiveInputHandler(m_defaultInputHandler);

        m_rangeMinX = sampleMin;
        m_rangeMinZ = sampleMin;
        m_stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
        m_stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);
        m_axisMinSliderX->setMinimum(0);
        m_axisMinSliderX->setMaximum(sampleCountX - 2);
        m_axisMinSliderX->setValue(0);
        m_axisMaxSliderX->setMinimum(1);
        m_axisMaxSliderX->setMaximum(sampleCountX - 1);
        m_axisMaxSliderX->setValue(sampleCountX - 1);
        m_axisMinSliderZ->setMinimum(0);
        m_axisMinSliderZ->setMaximum(sampleCountZ - 2);
        m_axisMinSliderZ->setValue(0);
        m_axisMaxSliderZ->setMinimum(1);
        m_axisMaxSliderZ->setMaximum(sampleCountZ - 1);
        m_axisMaxSliderZ->setValue(sampleCountZ - 1);
    }
}

void SurfaceGraphModifier::adjustXMin(int min)
{
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraphModifier::adjustXMax(int max)
{
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraphModifier::adjustZMin(int min)
{
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void SurfaceGraphModifier::adjustZMax(int max)
{
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}

void SurfaceGraphModifier::changeFunction(int f)
{
    m_graph->removeCustomItemAt(positionOne);
    m_funcNumber = f;
    fillSqrtSinProxy(f);
}

void SurfaceGraphModifier::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void SurfaceGraphModifier::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void SurfaceGraphModifier::setBlackToYellowGradient()
{
    //! [8]
    QLinearGradient gr;
    gr.setColorAt(0.f, Qt::black);
    gr.setColorAt(0.33f, Qt::blue);
    gr.setColorAt(0.67f, Qt::red);
    gr.setColorAt(1.f, Qt::yellow);

    m_sqrtSinSeries->setBaseGradient(gr);
    m_sqrtSinSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    //! [8]
}

void SurfaceGraphModifier::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.f, Qt::darkGreen);
    gr.setColorAt(0.5f, Qt::yellow);
    gr.setColorAt(0.8f, Qt::red);
    gr.setColorAt(1.f, Qt::darkRed);

    m_sqrtSinSeries->setBaseGradient(gr);
    m_sqrtSinSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void SurfaceGraphModifier::handleElementSelected(QAbstract3DGraph::ElementType type)
{
    resetSelection();
    if (type == QAbstract3DGraph::ElementCustomItem) {
        QCustom3DItem *item = m_graph->selectedCustomItem();
        QString text;
        if (qobject_cast<QCustom3DLabel *>(item) != 0) {
            text.append("Custom label: ");
        } else {
            QStringList split = item->meshFile().split("/");
            text.append(split.last());
            text.append(": ");
        }
        int index = m_graph->selectedCustomItemIndex();
        text.append(QString::number(index));
        m_textField->setText(text);
        m_previouslyAnimatedItem = item;
        m_previousScaling = item->scaling();
        m_selectionAnimation->setTargetObject(item);
        m_selectionAnimation->setStartValue(item->scaling());
        m_selectionAnimation->setEndValue(item->scaling() * 1.5f);
        m_selectionAnimation->start();
    } else if (type == QAbstract3DGraph::ElementSeries) {
        QString text = "Surface (";
        QSurface3DSeries *series = m_graph->selectedSeries();
        if (series) {
            QPoint point = series->selectedPoint();
            QString posStr;
            posStr.setNum(point.x());
            text.append(posStr);
            text.append(", ");
            posStr.setNum(point.y());
            text.append(posStr);
        }
        text.append(")");
        m_textField->setText(text);
    } else if (type > QAbstract3DGraph::ElementSeries
               && type < QAbstract3DGraph::ElementCustomItem) {
        int index = m_graph->selectedLabelIndex();
        QString text;
        if (type == QAbstract3DGraph::ElementAxisXLabel)
            text.append("Axis X label: ");
        else if (type == QAbstract3DGraph::ElementAxisYLabel)
            text.append("Axis Y label: ");
        else
            text.append("Axis Z label: ");
        text.append(QString::number(index));
        m_textField->setText(text);
    } else {
        m_textField->setText("Nothing");
    }
}

void SurfaceGraphModifier::resetSelection()
{
    m_selectionAnimation->stop();
    if (m_previouslyAnimatedItem)
        m_previouslyAnimatedItem->setScaling(m_previousScaling);
    m_previouslyAnimatedItem = nullptr;
}
