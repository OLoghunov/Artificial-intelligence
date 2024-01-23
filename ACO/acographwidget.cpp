#include "acographwidget.h"
#include "../Annealing/edge.h"
#include "../Annealing/node.h"
#include "../Annealing/SimulatedAnnealing.h"
#include "ACO.h"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>

//! [0]
ACOGraphWidget::ACOGraphWidget(QWidget *parent)
    : GraphWidget(parent)
{
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->setSceneRect(0, 0, 1599, 899);
    setScene(m_scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(1280, 720);
    setWindowTitle(tr("ACO"));
    //fillScene();
    //! [0]
}
//! [1]

//! [2]
void ACOGraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}
//! [2]

void ACOGraphWidget::setNodeCount(int count)
{
    this->m_nodeCount = count;
}

void ACOGraphWidget::fillScene()
{
    m_scene->clear();

    std::vector<Node*> nodes;
    for (int i = 0; i < m_nodeCount; i++)
    {
        nodes.push_back(new Node(this));
        m_scene->addItem(nodes.back());
        for (int j = 0; j < nodes.size() - 1; j++)
            m_scene->addItem(new Edge(nodes[i], nodes[j]));
        nodes.back()->setPos(sin(M_PI/m_nodeCount*2*i) * 350 + 800.0, cos(M_PI/m_nodeCount*2*i) * 350 + 450.0);
        QGraphicsTextItem *text = m_scene->addText(QString::number(i + 1));
        text->adjustSize();
        text->setPos(sin(M_PI/m_nodeCount*2*i) * 390 + 800.0, cos(M_PI/m_nodeCount*2*i) * 390 + 450.0);
    }

    vector<vector<double>> myDistanceMatrix =
        vector<vector<double>>{vector<double>{0, 2, 5, 4, 1, 2, 1, 7, 8, 5},
                               vector<double>{1, 0, 2, 8, 6, 3, 2, 1, 4, 6},
                               vector<double>{4, 3, 0, 1, 5, 1, 3, 8, 2, 6},
                               vector<double>{2, 6, 10, 0, 4, 1, 4, 3, 1, 4},
                               vector<double>{1, 7, 1, 11, 0, 2, 5, 5, 4, 1},
                               vector<double>{1, 7, 1, 11, 2, 0, 6, 1, 3, 9},
                               vector<double>{1, 9, 1, 1, 2, 4, 0, 2, 7, 3},
                               vector<double>{7, 2, 4, 6, 6, 7, 8, 0, 2, 5},
                               vector<double>{6, 6, 9, 8, 1, 3, 4, 5, 0, 9},
                               vector<double>{5, 9, 9, 9, 2, 5, 6, 9, 4, 0}};

    if (m_nodeCount > myDistanceMatrix.size())
        m_nodeCount = myDistanceMatrix.size();

    ACO aco = ACO(m_nodeCount, 1.0, 2.0, 0.5, 100.0, 0.01);

    aco.setDistanceMatrix(myDistanceMatrix);
    aco.optimize(5);

    auto phMatrix = aco.getPheromoneMatrix();

    QPen pen = QPen(Qt::red);

    for (int i = 0; i < phMatrix.size(); i++)
    {
        for (int j = 0; j < phMatrix[i].size(); j++)
        {
            pen.setWidth(phMatrix[i][j]/200);
            m_scene->addLine(nodes[i]->x(), nodes[i]->y(), nodes[j]->x(), nodes[j]->y(), pen);
        }
    }

    this->m_path = aco.getPath();
    this->m_length = aco.getLength();
}

//! [3]
void ACOGraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void ACOGraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Fill
    QRectF sceneRect = this->sceneRect();
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::white);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // // Text
    // QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
    //                 sceneRect.width() - 4, sceneRect.height() - 4);
    // QString message(tr("Annealing optimization"));

    // QFont font = painter->font();
    // font.setBold(true);
    // font.setPointSize(14);
    // painter->setFont(font);
    // painter->setPen(Qt::lightGray);
    // painter->drawText(textRect.translated(2, 2), message);
    // painter->setPen(Qt::black);
    // painter->drawText(textRect, message);
}

void ACOGraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if ((factor < 0.07) || (factor > 100))
        return;

    scale(scaleFactor, scaleFactor);
}

void ACOGraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void ACOGraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
