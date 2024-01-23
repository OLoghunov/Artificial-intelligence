#ifndef ACOGRAPHWIDGET_H
#define ACOGRAPHWIDGET_H

#include <QGraphicsView>
#include "../Annealing/graphwidget.h"

class Node;

class ACOGraphWidget : public GraphWidget
{
    Q_OBJECT

public:
    ACOGraphWidget(QWidget *parent = nullptr);

    void itemMoved();
    std::vector<int> getPath() { return m_path; }
    int getLength() { return m_length; }

public slots:
    void zoomIn();
    void zoomOut();
    void fillScene();
    void setNodeCount(int count);
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void scaleView(qreal scaleFactor);

private:
    QGraphicsScene *m_scene = new QGraphicsScene(this);
    int timerId = 0;
    Node *centerNode;
    std::vector<int> m_path;
    int m_length;
};

#endif // ACOGRAPHWIDGET_H
