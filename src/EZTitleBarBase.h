#ifndef EZTITLEBARBASE_H
#define EZTITLEBARBASE_H

#include <QWidget>

class EZTitleBarBase : public QWidget
{
public:
    explicit EZTitleBarBase(QWidget *parent = nullptr) {}
    virtual ~EZTitleBarBase() {}

    virtual bool isMovableArea(const QPoint &pt) { return true; }
};

#endif // EZTITLEBARBASE_H
