#include "acowidget.h"
#include "acographwidget.h"
#include "acographwidgetmodifier.h"

#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qpushbutton.h>

#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>

using namespace Qt::StringLiterals;

ACOWidget::ACOWidget()
{
    ACOWidget::initialize();
}

ACOWidget::~ACOWidget() = default;

void ACOWidget::initialize()
{
    // create surface for graph
    //
    m_acoWidget = new QWidget;
    auto *hLayout = new QHBoxLayout(m_acoWidget);
    auto container = new ACOGraphWidget();
    hLayout->addWidget(container);

    // create control boxes
    //
    auto *vLayout = new QVBoxLayout();
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    auto *selectionGroupBox = new QGroupBox(u"Ant colony settings"_s);

    auto *nodeLabel = new QLabel("Node number:\n5\t\t\t\t     50");

    auto *nodeCount = new QSlider(Qt::Horizontal);
    nodeCount->setMinimum(5);
    nodeCount->setMaximum(50);
    nodeCount->setTickInterval(1);
    nodeCount->setEnabled(true);

    auto *optimizeButton = new QPushButton(m_acoWidget);
    optimizeButton->setText(u"Start optimization"_s);

    auto *selectionVBox = new QVBoxLayout;

    selectionVBox->addWidget(nodeLabel);
    selectionVBox->addWidget(nodeCount);
    selectionVBox->addWidget(optimizeButton);

    selectionGroupBox->setLayout(selectionVBox);

    vLayout->addWidget(selectionGroupBox);


    auto *resultGroupBox = new QGroupBox(u"Optimization results"_s);

    auto *nodepathLabel = new QLabel("Path:");

    auto *pathLabel = new QLabel("");

    auto *nodelenLabel = new QLabel("Length:");

    auto *lenLabel = new QLabel("");

    auto *resultSelectionVBox = new QVBoxLayout;

    resultSelectionVBox->addWidget(nodepathLabel);
    resultSelectionVBox->addWidget(pathLabel);
    resultSelectionVBox->addWidget(nodelenLabel);
    resultSelectionVBox->addWidget(lenLabel);

    resultGroupBox->setLayout(resultSelectionVBox);

    vLayout->addWidget(resultGroupBox);



    ACOGraphWidgetModifier *modifier = new ACOGraphWidgetModifier(container);

    QObject::connect(nodeCount, &QSlider::valueChanged, modifier, &ACOGraphWidgetModifier::setNodeCount);

    QObject::connect(modifier, SIGNAL(pathChanged(QString)), pathLabel, SLOT(setText(QString)));

    QObject::connect(modifier, SIGNAL(lengthChanged(int)), lenLabel, SLOT(setNum(int)));

    QObject::connect(optimizeButton, &QPushButton::pressed, modifier, &ACOGraphWidgetModifier::fillScene);
}
