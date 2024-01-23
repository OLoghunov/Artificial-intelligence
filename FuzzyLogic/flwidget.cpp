#include "flwidget.h"
#include "flgraphwidget.h"
#include "flgraphwidgetmodifier.h"

#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qpushbutton.h>

#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>

using namespace Qt::StringLiterals;

FlWidget::FlWidget()
{
    FlWidget::initialize();
}

FlWidget::~FlWidget() = default;

void FlWidget::initialize()
{
    // create surface for graph
    //
    m_flWidget = new QWidget;
    auto *hLayout = new QHBoxLayout(m_flWidget);
    auto container = new FlGraphWidget();
    hLayout->addWidget(container);

    // create control boxes
    //
    auto *vLayout = new QVBoxLayout();
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    auto *selectionGroupBox = new QGroupBox(u"Fuzzy logic settings"_s);

    auto *optimizeButton = new QPushButton(m_flWidget);
    optimizeButton->setText(u"Generate labyrinth"_s);

    auto *stepButton = new QPushButton(m_flWidget);
    stepButton->setText(u"Make step"_s);

    auto *selectionVBox = new QVBoxLayout;

    selectionVBox->addWidget(optimizeButton);
    selectionVBox->addWidget(stepButton);

    selectionGroupBox->setLayout(selectionVBox);

    vLayout->addWidget(selectionGroupBox);


    FlGraphWidgetModifier *modifier = new FlGraphWidgetModifier(container);

    QObject::connect(optimizeButton, &QPushButton::pressed, modifier, &FlGraphWidgetModifier::fillScene);

    QObject::connect(stepButton, &QPushButton::pressed, modifier, &FlGraphWidgetModifier::updateScene);
}
