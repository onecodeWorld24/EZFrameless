#ifndef WIDGET_H
#define WIDGET_H

#include "EZFrameless.h"

#include <QWidget>

class Widget : public EZFrameless
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
};
#endif // WIDGET_H
