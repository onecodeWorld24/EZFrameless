#ifndef EZTITLEBARBASE_H
#define EZTITLEBARBASE_H

#include <QDebug>
#include <QWidget>

class EZTitleBarBase : public QWidget
{
public:
    explicit EZTitleBarBase(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setFixedHeight(30);
    }
    virtual ~EZTitleBarBase() {}

    virtual bool isMovableArea(const QPoint &pt)
    {
        qDebug() << __FUNCTION__ << pt;
        if (rect().contains(mapFromParent(pt)))
            return true;
        else
            return false;
    }
};

#endif // EZTITLEBARBASE_H
