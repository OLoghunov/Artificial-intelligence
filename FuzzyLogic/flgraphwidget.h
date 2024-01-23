#ifndef FLGRAPHWIDGET_H
#define FLGRAPHWIDGET_H

#include <QGraphicsView>
#include <iostream>
#include <random>

#include "FuzzyRobot.h"

class Labyrinth
{
public:
    std::vector<std::vector<char>> field = {};
    int width;
    int height;
    int playerY = 0, playerX = 1;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> needToBeRemoved = {};

    bool isNotReady = true;
    bool isGenerated = false;

    // the coordinates of the roads are stored here
    // arrays of their potential neighbors are stored in pairs with them
    //
    std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> roadCells = {};

    void createStartAndEnd()
    {
        for (int i = 0; i < height; i++)
        {
            if (field[i][1] == '-')
                playerY = i;
        }
        field[playerY][playerX] = 'P';
        for (int i = 0; i < height; i++)
        {
            if (field[i][width - 2] == '-')
            {
                field[i][width - 2] = 'E';
                break;
            }
        }
    }

    void createRoad(std::pair<int, int> curCell, std::pair<int, int> neighbour) {
        field[neighbour.second][neighbour.first] = '-';
        int newY = curCell.second + (neighbour.second - curCell.second)/2;
        int newX = curCell.first + (neighbour.first - curCell.first)/2;
        field[newY][newX] = '-';
    }

    bool checkNeighbourhood(int x, int y)
    {
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                if (x + i >= 0 && y + j >= 0 &&
                    x + i <= width - 1 && y + j <= height - 1 &&
                    field[y + j][x + i] == '-')
                    return false;
        return true;
    }

    std::vector<std::pair<int, int>> findValidNeighbours(std::pair<int, int> curCell)
    {
        std::vector<std::pair<int, int>> c = {};
        if (curCell.first > 1 && field[curCell.second][curCell.first - 2] == '#' && checkNeighbourhood(curCell.first - 2, curCell.second)) // left neighbour
            c.push_back({curCell.first - 2, curCell.second});
        if (curCell.first < width - 2 && field[curCell.second][curCell.first + 2] == '#' && checkNeighbourhood(curCell.first + 2, curCell.second)) // right neighbour
            c.push_back({curCell.first + 2, curCell.second});
        if (curCell.second > 1 && field[curCell.second - 2][curCell.first] == '#' && checkNeighbourhood(curCell.first, curCell.second - 2)) // top neighbour
            c.push_back({curCell.first, curCell.second - 2});
        if (curCell.second < height - 2 && field[curCell.second + 2][curCell.first] == '#' && checkNeighbourhood(curCell.first, curCell.second + 2)) // bottom neighbour
            c.push_back({curCell.first, curCell.second + 2});
        return c;
    }

    void initiateLabyrinth() {
        height = 19;
        width = 35;

        // firstly the field is filled with walls
        //
        for (int i = 0; i < height; i++)
        {
            field.push_back({});
            for (int j = 0; j < width; j++)
                field.back().push_back('#');
        }

        // the point where the generation starts from
        //
        int x = 19;
        int y = 9;
        field[y][x] = '-';
        std::pair<int, int> startCell = {x, y};

        // hash symbols are walls
        // dash symbols are roads
        // question marks are potential neighbours
        roadCells[startCell] = findValidNeighbours(startCell);
        for (std::pair<int, int> neighbour : roadCells[startCell])
            field[neighbour.second][neighbour.first] = '?';

        // for (auto row : field)
        // {
        //     for (auto ch : row)
        //         std::cout << ch << ' ';
        //     std::cout << std::endl;
        // }
    }

    int myrandom (int i) { return std::rand()%i;}

    void build()
    {
        // a random existing road is selected, a random potential neighbor is selected from it
        //
        std::random_device device;
        std::uniform_int_distribution<int> cross(0, 14);

        std::vector<std::pair<int, int>> vec = {};

        for (std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>> pair : roadCells)
            vec.push_back(pair.first);

        std::random_shuffle(vec.begin(), vec.end());

        std::pair<int, int> curCell = vec[0];

        vec = {};

        for (std::pair<int, int> pair : roadCells[curCell])
            vec.push_back(pair);

        std::random_shuffle(vec.begin(), vec.end());

        std::pair<int, int> neighbour = vec[0];



        // a new road is being created
        //
        createRoad(curCell, neighbour);

        // the list of roads is being updated
        //
        roadCells[neighbour] = {};

        // the list of potential neighbors is updated
        // if the road has no potential neighbors, it is removed from the list
        //
        if (!findValidNeighbours(neighbour).empty())
        {
            for (auto pair : findValidNeighbours(neighbour))
            {
                roadCells[neighbour].push_back(pair);
                field[pair.second][pair.first] = '?';
            }

        }
        else
            roadCells.erase(neighbour);

        // a potential neighbor has become the road
        //
        field[neighbour.second][neighbour.first] = '-';

        roadCells[curCell].erase(std::remove(roadCells[curCell].begin(), roadCells[curCell].end(), neighbour), roadCells[curCell].end());
        if (roadCells[curCell].empty())
            roadCells.erase(curCell);

        // after updating the road the neighbors will double-check
        // the neighbor cell of the potential neighbor cannot be a road
        //
        for (std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>> pair : roadCells)
        {
            for (std::pair<int, int> neighbour : pair.second)
            {
                if (!checkNeighbourhood(neighbour.first, neighbour.second))
                {
                    field[neighbour.second][neighbour.first] = '#';
                    needToBeRemoved.push_back({pair.first, neighbour});
                }
            }
        }

        // invalid neighbors are removed
        //
        for (std::pair<std::pair<int, int>, std::pair<int, int>> pair : needToBeRemoved)
        {
            roadCells[pair.first].erase(std::remove(roadCells[pair.first].begin(), roadCells[pair.first].end(), pair.second), roadCells[pair.first].end());
            if (roadCells[pair.first].empty())
                roadCells.erase(pair.first);
        }
        needToBeRemoved.clear();

        if (roadCells.empty())
            isNotReady = false;
        isGenerated = true;
    }

};

class FlGraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    FuzzyRobot fuzzyRobot;
    FlGraphWidget(QWidget *parent = nullptr);

    void itemMoved();
    void setNodeCount(int count);
public slots:
    void zoomIn();
    void zoomOut();
    void generateScene();
    void updateScene();
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void scaleView(qreal scaleFactor);

private:
    QGraphicsScene *m_scene = new QGraphicsScene(this);
    int timerId = 0;

protected:
    int m_nodeCount = 5;
};

#endif // FLGRAPHWIDGET_H
