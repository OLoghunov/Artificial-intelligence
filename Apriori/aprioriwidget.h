#ifndef APRIORIWIDGET_H
#define APRIORIWIDGET_H

#include <QtCore/qobject.h>

class AprioriWidget : public QObject
{
    Q_OBJECT
public:
    AprioriWidget();
    ~AprioriWidget();

    void initialize();
    QWidget *getWidget() { return m_aprioriWidget; }

private:
    QWidget *m_container = nullptr;
    QWidget *m_aprioriWidget = nullptr;
};

#endif // APRIORIWIDGET_H
