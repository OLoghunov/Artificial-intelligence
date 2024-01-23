#ifndef FLGRAPHWIDGETMODIFIER_H
#define FLGRAPHWIDGETMODIFIER_H

#include "QtCore/qobject.h"
#include "flgraphwidget.h"

class FlGraphWidgetModifier : public QObject
{
    Q_OBJECT
public:
    explicit FlGraphWidgetModifier(FlGraphWidget *graph);
    void fillScene();
    void updateScene();

signals:
    void pathChanged(QString str);
    void lengthChanged(int length);

private:
    FlGraphWidget *m_graph;
};

#endif // FLGRAPHWIDGETMODIFIER_H
