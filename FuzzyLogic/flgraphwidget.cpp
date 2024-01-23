#include "flgraphwidget.h"
#include "flgraphwidgetmodifier.h"

#include <math.h>

#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QThread>

#include <QKeyEvent>
#include <QRandomGenerator>


FlGraphWidget::FlGraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->setSceneRect(0, 0, 1280, 720);
    setScene(m_scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(1280, 720);
    setWindowTitle(tr("Fuzzy logic"));
}

void FlGraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void FlGraphWidget::generateScene()
{
    m_scene->clear();
    Labyrinth lab;
    lab.initiateLabyrinth();

    while (lab.isNotReady)
    {
        lab.build();
    }
    lab.createStartAndEnd();
    QPen pen = QPen(Qt::black);
    QBrush brush = QBrush(Qt::black);
    pen.setWidth(2);
    int posX = 0, posY = 0, posFinX = 0, posFinY = 0;

    std::vector<std::vector<char>> field = {
        {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
        {'#','-','-','-','-','-','#','-','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','-','-','-','-','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','-','-','-','-','-','-','-','-','#','-','-','-','-','-','-','-','E','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','-','-','-','-','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','-','-','-','-','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','-','#','-','#','#','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','-','-','-','-','#','-','-','-','#','-','-','-','-','-','-','#','-','-','-','-','-','#','#','#','#','#','#','#','#','#','#'},
        {'#','-','-','-','-','-','-','-','#','-','-','-','#','-','-','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','-','-','-','-','#','-','-','-','#','-','-','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','#','#','-','-','-','#','-','-','-','#','-','-','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','#','-','-','-','#','-','-','-','#','-','#','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','#','#','#','-','#','-','-','-','-','-','#','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','-','-','-','-','-','-','-','#','-','-','-','-','-','#','-','-','-','-','#','-','-','-','#','#','-','#','#','#','#','#','#','#','#','#'},
        {'#','-','-','-','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','#','#','#','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','#','P','-','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','#','#','#','-','-','-','-','#','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','#'},
        {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}

    };

    for (int i = 0; i < lab.height; i++)
    {
        for (int j = 0; j < lab.width; j++)
        {
            switch(field[i][j])
            {
            case '-':
                m_scene->addRect(j * 1280.0f/lab.width + (1280.0f - 1280.0f)/2.0f,
                                 i * 720.0f/lab.height,
                                 1280.0f/lab.width,
                                 720.0f/lab.height, pen, brush);
                break;
            case '#':
                m_scene->addRect(j * 1280.0f/lab.width + (1280.0f - 1280.0f)/2.0f,
                                 i * 720.0f/lab.height,
                                 1280.0f/lab.width,
                                 720.0f/lab.height, pen);
                break;
            case 'P':
                posX = i;
                posY = j;
                m_scene->addRect(j * 1280.0f/lab.width + (1280.0f - 1280.0f)/2.0f,
                                 i * 720.0f/lab.height,
                                 1280.0f/lab.width,
                                 720.0f/lab.height, pen, QBrush(Qt::blue));
                break;
            case 'E':
                posFinX = i;
                posFinY = j;
                m_scene->addRect(j * 1280.0f/lab.width + (1280.0f - 1280.0f)/2.0f,
                                 i * 720.0f/lab.height,
                                 1280.0f/lab.width,
                                 720.0f/lab.height, pen, QBrush(Qt::red));
                break;
            }
        }
    }


    fuzzyRobot.setField(field);
    fuzzyRobot.setStartPosition(posY, posX);
    fuzzyRobot.setFinishPosition(posFinY, posFinX);
}

void FlGraphWidget::updateScene()
{
    QPen pen = QPen(Qt::black);
    pen.setWidth(2);
    std::pair<int, int> formerCoords = fuzzyRobot.getCoordinates();
    m_scene->addRect(formerCoords.first * 1280.0f/35.0f + (1280.0f - 1280.0f)/2.0f,
                     formerCoords.second * 720.0f/19.0f,
                     1280.0f/35.0f,
                     720.0f/19.0f, pen, QBrush(Qt::black));
    fuzzyRobot.makeStep();
    std::pair<int, int> coords = fuzzyRobot.getCoordinates();
    m_scene->addRect(coords.first * 1280.0f/35.0f + (1280.0f - 1280.0f)/2.0f,
                     coords.second * 720.0f/19.0f,
                     1280.0f/35.0f,
                     720.0f/19.0f, pen, QBrush(Qt::blue));
    m_scene->update();
}

void FlGraphWidget::setNodeCount(int count)
{
    this->m_nodeCount = count;
}

void FlGraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::white);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}

void FlGraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if ((factor < 0.07) || (factor > 100))
        return;

    scale(scaleFactor, scaleFactor);
}

void FlGraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void FlGraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
