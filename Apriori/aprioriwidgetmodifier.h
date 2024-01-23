#ifndef APRIORIWIDGETMODIFIER_H
#define APRIORIWIDGETMODIFIER_H

#include "QtCore/qobject.h"
#include "Apriori.h"

class AprioriWidgetModifier : public QObject
{
    Q_OBJECT
public:
    void changeSupportLevel(float value);
    void changeConfidenceLevel(float value);
    void changeMinSetSize(int value);
    void analyze();
    void read();

signals:
    void analyzed(QString str);
    void transactionsChanged(QString str);

private:
    float confidenceLevel = 0.65f;
    float supportLevel = 0.25f;
    int minSetSize = 3;
    int currentMaxKey = 1;
    std::map<int, std::set<int>> transactions{};
};


#endif // APRIORIWIDGETMODIFIER_H
