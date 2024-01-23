#include "aprioriwidget.h"
#include "aprioriwidgetmodifier.h"

#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qpushbutton.h>

#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>

using namespace Qt::StringLiterals;

AprioriWidget::AprioriWidget()
{
    AprioriWidget::initialize();
}

AprioriWidget::~AprioriWidget() = default;

void AprioriWidget::initialize()
{
    m_aprioriWidget = new QWidget;
    auto *mainVLayout = new QVBoxLayout(m_aprioriWidget);
    auto *hLayout = new QHBoxLayout();

    mainVLayout->addLayout(hLayout);

    // create control boxes
    //
    auto *vLayout = new QVBoxLayout();
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    auto *supportLabel = new QLabel(u"Support level (%)"_s);

    auto *supportLevel = new QSlider(Qt::Horizontal);
    supportLevel->setFixedWidth(480);
    supportLevel->setValue(25);
    supportLevel->setMinimum(0);
    supportLevel->setMaximum(100);
    supportLevel->setTickInterval(1);
    supportLevel->setEnabled(true);

    auto *confidenceLabel = new QLabel(u"Confidence level (%)"_s);

    auto *confidenceLevel = new QSlider(Qt::Horizontal);
    confidenceLevel->setFixedWidth(480);
    confidenceLevel->setValue(65);
    confidenceLevel->setMinimum(0);
    confidenceLevel->setMaximum(100);
    confidenceLevel->setTickInterval(1);
    confidenceLevel->setEnabled(true);

    auto *sizeLabel = new QLabel(u"Minimum set size (2-5)"_s);

    auto *setSize = new QSlider(Qt::Horizontal);
    setSize->setFixedWidth(480);
    setSize->setValue(3);
    setSize->setMinimum(2);
    setSize->setMaximum(5);
    setSize->setTickInterval(1);
    setSize->setEnabled(true);

    auto* readButton = new QPushButton(m_aprioriWidget);
    readButton->setText(u"Read data"_s);

    auto *mapLabel = new QLabel("");
    mapLabel->setFixedHeight(300);

    auto *optimizeButton = new QPushButton(m_aprioriWidget);

    optimizeButton->setText(u"Analyze"_s);

    auto *transactionsGroupBox = new QGroupBox(u"Transactions list"_s);
    transactionsGroupBox->setFixedWidth(500);

    auto *settingsGroupBox = new QGroupBox(u"Apriori settings"_s);

    auto *resultGroupBox = new QGroupBox(u"Optimization results"_s);

    auto *resultLabel = new QLabel("");

    auto *transactionsVBox = new QVBoxLayout;

    auto *resultSelectionVBox = new QVBoxLayout;

    transactionsVBox->addWidget(mapLabel);
    transactionsVBox->addWidget(supportLabel);
    transactionsVBox->addWidget(supportLevel);
    transactionsVBox->addWidget(confidenceLabel);
    transactionsVBox->addWidget(confidenceLevel);
    transactionsVBox->addWidget(sizeLabel);
    transactionsVBox->addWidget(setSize);
    transactionsVBox->addWidget(readButton);
    resultSelectionVBox->addWidget(resultLabel);
    resultSelectionVBox->addWidget(optimizeButton);

    transactionsGroupBox->setLayout(transactionsVBox);
    resultGroupBox->setLayout(resultSelectionVBox);

    hLayout->addWidget(transactionsGroupBox);
    hLayout->addWidget(resultGroupBox);



    AprioriWidgetModifier *modifier = new AprioriWidgetModifier();

    QObject::connect(supportLevel, &QSlider::valueChanged, modifier, &AprioriWidgetModifier::changeSupportLevel);

    QObject::connect(confidenceLevel, &QSlider::valueChanged, modifier, &AprioriWidgetModifier::changeConfidenceLevel);

    QObject::connect(setSize, &QSlider::valueChanged, modifier, &AprioriWidgetModifier::changeMinSetSize);

    QObject::connect(modifier, SIGNAL(analyzed(QString)), resultLabel, SLOT(setText(QString)));

    QObject::connect(modifier, SIGNAL(transactionsChanged(QString)), mapLabel, SLOT(setText(QString)));

    QObject::connect(optimizeButton, &QPushButton::pressed, modifier, &AprioriWidgetModifier::analyze);

    QObject::connect(readButton, &QPushButton::pressed, modifier, &AprioriWidgetModifier::read);
}
