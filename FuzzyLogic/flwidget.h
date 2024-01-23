#ifndef FLWIDGET_H
#define FLWIDGET_H

#include <QtCore/qobject.h>

class FlWidget : public QObject
{
    Q_OBJECT
public:
    FlWidget();
    ~FlWidget();

    void initialize();
    QWidget *getWidget() { return m_flWidget; }

private:
    QWidget *m_container = nullptr;
    QWidget *m_flWidget = nullptr;
};

#endif // FLWIDGET_H
