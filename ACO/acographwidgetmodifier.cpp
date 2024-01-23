#include "acographwidgetmodifier.h"

ACOGraphWidgetModifier::ACOGraphWidgetModifier(ACOGraphWidget *graph)
{
    this->m_graph = graph;
}

void ACOGraphWidgetModifier::fillScene()
{
    m_graph->fillScene();

    QString str = "";
    std::vector<int> path = m_graph->getPath();
    for (auto town : path)
        str.append(QString::number(town + 1) + " ");

    emit pathChanged(str);
    emit lengthChanged(m_graph->getLength());
}

void ACOGraphWidgetModifier::setNodeCount(int count)
{
    m_graph->setNodeCount(count);
}
