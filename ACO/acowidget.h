#ifndef ACOWIDGET_H
#define ACOWIDGET_H

#include <QtCore/qobject.h>

class ACOWidget : public QObject
{
    Q_OBJECT
public:
    ACOWidget();
    ~ACOWidget();

    void initialize();
    QWidget *getWidget() { return m_acoWidget; }

private:
    QWidget *m_container = nullptr;
    QWidget *m_acoWidget = nullptr;
};

#endif // ACOWIDGET_H
