#include "GAsurfacegraph.h"
#include "GAsurfacegraphmodifier.h"

#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qcommandlinkbutton.h>
#include <QtGui/qpainter.h>

using namespace Qt::StringLiterals;

static QPixmap gradientBtoYPB_Pixmap()
{
    QLinearGradient grBtoY(0, 0, 1, 100);
    grBtoY.setColorAt(1.f, Qt::black);
    grBtoY.setColorAt(0.67f, Qt::blue);
    grBtoY.setColorAt(0.33f, Qt::red);
    grBtoY.setColorAt(0.f, Qt::yellow);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grBtoY));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, 24, 100);
    return pm;
}

static QPixmap gradientGtoRPB_Pixmap()
{
    QLinearGradient grGtoR(0, 0, 1, 100);
    grGtoR.setColorAt(1.f, Qt::darkGreen);
    grGtoR.setColorAt(0.5f, Qt::yellow);
    grGtoR.setColorAt(0.2f, Qt::red);
    grGtoR.setColorAt(0.f, Qt::darkRed);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grGtoR));
    pmp.drawRect(0, 0, 24, 100);
    return pm;
}

static QPixmap highlightPixmap()
{
    constexpr int height = 400;
    constexpr int width = 110;
    constexpr int border = 10;
    QLinearGradient gr(0, 0, 1, height - 2 * border);
    gr.setColorAt(1.f, Qt::black);
    gr.setColorAt(0.8f, Qt::darkGreen);
    gr.setColorAt(0.6f, Qt::green);
    gr.setColorAt(0.4f, Qt::yellow);
    gr.setColorAt(0.2f, Qt::red);
    gr.setColorAt(0.f, Qt::darkRed);

    QPixmap pmHighlight(width, height);
    pmHighlight.fill(Qt::transparent);
    QPainter pmpHighlight(&pmHighlight);
    pmpHighlight.setBrush(QBrush(gr));
    pmpHighlight.setPen(Qt::NoPen);
    pmpHighlight.drawRect(border, border, 35, height - 2 * border);
    pmpHighlight.setPen(Qt::black);
    int step = (height - 2 * border) / 5;
    for (int i = 0; i < 6; ++i) {
        int yPos = i * step + border;
        pmpHighlight.drawLine(border, yPos, 55, yPos);
        const int height = 550 - (i * 110);
        pmpHighlight.drawText(60, yPos + 2, QString::number(height) + u" m"_s);
    }
    return pmHighlight;
}

GASurfaceGraph::GASurfaceGraph()
{
    m_surfaceGraph = new Q3DSurface();
}

GASurfaceGraph::~GASurfaceGraph() = default;

bool GASurfaceGraph::initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize)
{
    if (!m_surfaceGraph->hasContext())
        return false;

    m_surfaceWidget = new QWidget;
    auto *hLayout = new QHBoxLayout(m_surfaceWidget);
    m_container = QWidget::createWindowContainer(m_surfaceGraph, m_surfaceWidget);
    m_container->setMinimumSize(minimumGraphSize);
    m_container->setMaximumSize(maximumGraphSize);
    m_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_container->setFocusPolicy(Qt::StrongFocus);
    hLayout->addWidget(m_container, 1);

    auto *vLayout = new QVBoxLayout();
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    // Create control widgets
    auto *modelGroupBox = new QGroupBox(u"Model"_s);

    auto *sqrtSinModelRB = new QRadioButton(m_surfaceWidget);
    sqrtSinModelRB->setText(u"Sqrt && Sin"_s);
    sqrtSinModelRB->setChecked(false);

    auto *modelVBox = new QVBoxLayout;
    modelVBox->addWidget(sqrtSinModelRB);
    modelGroupBox->setLayout(modelVBox);

    auto *selectionGroupBox = new QGroupBox(u"GA settings"_s);

    auto *itemStyleList = new QComboBox(m_surfaceWidget);
    itemStyleList->addItem(u"Himmelblau"_s, QAbstract3DSeries::MeshSphere);
    itemStyleList->addItem(u"Rosenbrock"_s, QAbstract3DSeries::MeshCube);
    itemStyleList->addItem(u"Booth"_s, QAbstract3DSeries::MeshMinimal);
    itemStyleList->addItem(u"First func"_s, QAbstract3DSeries::MeshSphere);
    itemStyleList->addItem(u"Second func"_s, QAbstract3DSeries::MeshCube);
    itemStyleList->addItem(u"Twelfth func"_s, QAbstract3DSeries::MeshMinimal);
    itemStyleList->setCurrentIndex(0);

    auto *itemCodeList = new QComboBox(m_surfaceWidget);
    itemCodeList->addItem(u"Real"_s, QAbstract3DSeries::MeshSphere);
    itemCodeList->addItem(u"Gray Code"_s, QAbstract3DSeries::MeshCube);
    itemCodeList->setCurrentIndex(0);

    auto *generationSize = new QSlider(Qt::Horizontal);
    generationSize->setMinimum(500);
    generationSize->setMaximum(1000);
    generationSize->setTickInterval(1);
    generationSize->setEnabled(true);

    auto *sampleSize = new QSlider(Qt::Horizontal);
    sampleSize->setMinimum(100);
    sampleSize->setMaximum(400);
    sampleSize->setTickInterval(1);
    sampleSize->setEnabled(true);

    auto *optimizeButton = new QPushButton(m_surfaceWidget);
    optimizeButton->setText(u"Start optimization"_s);

    auto *modeItemRB = new QRadioButton(m_surfaceWidget);
    modeItemRB->setText(u"Item"_s);
    modeItemRB->setChecked(false);

    auto *modeSliceRowRB = new QRadioButton(m_surfaceWidget);
    modeSliceRowRB->setText(u"Row Slice"_s);
    modeSliceRowRB->setChecked(false);

    auto *modeSliceColumnRB = new QRadioButton(m_surfaceWidget);
    modeSliceColumnRB->setText(u"Column Slice"_s);
    modeSliceColumnRB->setChecked(false);

    auto *selectionVBox = new QVBoxLayout;
    vLayout->addWidget(new QLabel(u"Select function"_s));
    vLayout->addWidget(itemStyleList);

    vLayout->addWidget(new QLabel(u"Select coding type"_s));
    vLayout->addWidget(itemCodeList);

    QLabel *partCountLabel = new QLabel("Generation size:\n500                   1000");
    selectionVBox->addWidget(partCountLabel);
    selectionVBox->addWidget(generationSize);

    QLabel *maxIterationLabel = new QLabel("Sample size:\n100                    400");
    selectionVBox->addWidget(maxIterationLabel);
    selectionVBox->addWidget(sampleSize);

    selectionVBox->addWidget(optimizeButton);
    selectionVBox->addWidget(modeItemRB);
    selectionVBox->addWidget(modeSliceRowRB);
    selectionVBox->addWidget(modeSliceColumnRB);
    selectionGroupBox->setLayout(selectionVBox);

    auto *axisGroupBox = new QGroupBox(u"Axis ranges"_s);

    auto *axisMinSliderX = new QSlider(Qt::Horizontal);
    axisMinSliderX->setMinimum(0);
    axisMinSliderX->setTickInterval(1);
    axisMinSliderX->setEnabled(true);
    auto *axisMaxSliderX = new QSlider(Qt::Horizontal);
    axisMaxSliderX->setMinimum(1);
    axisMaxSliderX->setTickInterval(1);
    axisMaxSliderX->setEnabled(true);
    auto *axisMinSliderZ = new QSlider(Qt::Horizontal);
    axisMinSliderZ->setMinimum(0);
    axisMinSliderZ->setTickInterval(1);
    axisMinSliderZ->setEnabled(true);
    auto *axisMaxSliderZ = new QSlider(Qt::Horizontal);
    axisMaxSliderZ->setMinimum(1);
    axisMaxSliderZ->setTickInterval(1);
    axisMaxSliderZ->setEnabled(true);

    auto *axisVBox = new QVBoxLayout;
    axisVBox->addWidget(new QLabel(u"Column range"_s));
    axisVBox->addWidget(axisMinSliderX);
    axisVBox->addWidget(axisMaxSliderX);
    axisVBox->addWidget(new QLabel(u"Row range"_s));
    axisVBox->addWidget(axisMinSliderZ);
    axisVBox->addWidget(axisMaxSliderZ);
    axisGroupBox->setLayout(axisVBox);

    // Mode-dependent controls
    // sqrt-sin
    auto *colorGroupBox = new QGroupBox(u"Custom gradient"_s);

    QPixmap pixmap = gradientBtoYPB_Pixmap();
    auto *gradientBtoYPB = new QPushButton(m_surfaceWidget);
    gradientBtoYPB->setIcon(QIcon(pixmap));
    gradientBtoYPB->setIconSize(pixmap.size());

    pixmap = gradientGtoRPB_Pixmap();
    auto *gradientGtoRPB = new QPushButton(m_surfaceWidget);
    gradientGtoRPB->setIcon(QIcon(pixmap));
    gradientGtoRPB->setIconSize(pixmap.size());
    auto *colorHBox = new QHBoxLayout;
    colorHBox->addWidget(gradientBtoYPB);
    colorHBox->addWidget(gradientGtoRPB);
    colorGroupBox->setLayout(colorHBox);

    auto *labelSelectedItem = new QLabel(u"Nothing"_s);
    labelSelectedItem->setVisible(false);

    // Populate vertical layout
    // Common
    //vLayout->addWidget(modelGroupBox);
    vLayout->addWidget(selectionGroupBox);
    vLayout->addWidget(axisGroupBox);

    // Sqrt Sin
    vLayout->addWidget(colorGroupBox);

    vLayout->addWidget(labelSelectedItem);


    // Create the controller
    auto *modifier = new GASurfaceGraphModifier(m_surfaceGraph, labelSelectedItem, this);

    // Connect widget controls to controller
    QObject::connect(sqrtSinModelRB, &QRadioButton::toggled,
                     modifier, &GASurfaceGraphModifier::enableSqrtSinModel);

    QObject::connect(generationSize, &QSlider::valueChanged,
                     modifier, &GASurfaceGraphModifier::changeGenerationSize);

    QObject::connect(sampleSize, &QSlider::valueChanged,
                     modifier, &GASurfaceGraphModifier::changeSampleSize);

    QObject::connect(optimizeButton, &QPushButton::clicked,
                     modifier, &GASurfaceGraphModifier::toggleModeNone);

    QObject::connect(modeItemRB,  &QRadioButton::toggled,
                     modifier, &GASurfaceGraphModifier::toggleModeItem);
    QObject::connect(modeSliceRowRB,  &QRadioButton::toggled,
                     modifier, &GASurfaceGraphModifier::toggleModeSliceRow);
    QObject::connect(modeSliceColumnRB,  &QRadioButton::toggled,
                     modifier, &GASurfaceGraphModifier::toggleModeSliceColumn);

    QObject::connect(axisMinSliderX, &QSlider::valueChanged,
                     modifier, &GASurfaceGraphModifier::adjustXMin);
    QObject::connect(axisMaxSliderX, &QSlider::valueChanged,
                     modifier, &GASurfaceGraphModifier::adjustXMax);
    QObject::connect(axisMinSliderZ, &QSlider::valueChanged,
                     modifier, &GASurfaceGraphModifier::adjustZMin);
    QObject::connect(axisMaxSliderZ, &QSlider::valueChanged,
                     modifier, &GASurfaceGraphModifier::adjustZMax);


    QObject::connect(itemStyleList, &QComboBox::currentIndexChanged,
                     modifier, &GASurfaceGraphModifier::changeFunction);

    QObject::connect(itemCodeList, &QComboBox::currentIndexChanged,
                     modifier, &GASurfaceGraphModifier::changeCoding);

    // Mode dependent connections
    QObject::connect(gradientBtoYPB, &QPushButton::pressed,
                     modifier, &GASurfaceGraphModifier::setBlackToYellowGradient);
    QObject::connect(gradientGtoRPB, &QPushButton::pressed,
                     modifier, &GASurfaceGraphModifier::setGreenToRedGradient);


    // Connections to disable features depending on mode
    QObject::connect(sqrtSinModelRB, &QRadioButton::toggled,
                     colorGroupBox, &QGroupBox::setVisible);


    modifier->setAxisMinSliderX(axisMinSliderX);
    modifier->setAxisMaxSliderX(axisMaxSliderX);
    modifier->setAxisMinSliderZ(axisMinSliderZ);
    modifier->setAxisMaxSliderZ(axisMaxSliderZ);

    sqrtSinModelRB->setChecked(true);
    modeItemRB->setChecked(true);

    return true;
}
