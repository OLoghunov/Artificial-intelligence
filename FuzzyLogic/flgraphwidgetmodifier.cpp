#include "flgraphwidgetmodifier.h"

FlGraphWidgetModifier::FlGraphWidgetModifier(FlGraphWidget *graph)
{
    this->m_graph = graph;
}

void FlGraphWidgetModifier::fillScene()
{
    m_graph->generateScene();

    // auto order = m_graph->getOrder();
    // QString str = QString("");
    // for (int i = 0; i < order.size() - 1; i++)
    // {
    //     str.append("(" + QString::number(order[i].x) + ", " + QString::number(order[i].y) + ")\n");
    // }

    // emit pathChanged(str);
    // emit lengthChanged(m_graph->getDistance());
}

void FlGraphWidgetModifier::updateScene()
{
    m_graph->updateScene();
}
