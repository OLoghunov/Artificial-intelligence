#ifndef GASURFACEGRAPH_H
#define GASURFACEGRAPH_H

#include <QtCore/qobject.h>
#include <QtDataVisualization/q3dsurface.h>

class GASurfaceGraph : public QObject
{
    Q_OBJECT
public:
    GASurfaceGraph();
    ~GASurfaceGraph();

    bool initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize);
    QWidget *surfaceWidget() { return m_surfaceWidget; }

private:
    Q3DSurface *m_surfaceGraph = nullptr;
    QWidget *m_container = nullptr;
    QWidget *m_surfaceWidget = nullptr;
};

#endif

