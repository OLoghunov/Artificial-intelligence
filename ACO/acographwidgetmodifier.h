#ifndef ACOGRAPHWIDGETMODIFIER_H
#define ACOGRAPHWIDGETMODIFIER_H

#include "QtCore/qobject.h"
#include "acographwidget.h"

class ACOGraphWidgetModifier : public QObject
{
    Q_OBJECT
public:
    explicit ACOGraphWidgetModifier(ACOGraphWidget *graph);
    void setNodeCount(int count);

    void fillScene();

signals:
    void pathChanged(QString str);
    void lengthChanged(int length);

private:
    ACOGraphWidget *m_graph;
};

#endif // ACOGRAPHWIDGETMODIFIER_H
