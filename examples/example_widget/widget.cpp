#include "widget.h"

Widget::Widget(QWidget *parent)
    : EZFrameless(parent)
{
    setResizeable(true);

    QWidget *leftWidget = new QWidget();
    leftWidget->setObjectName("w1");
    leftWidget->setMinimumSize(20, 20);
    QWidget *rightWidget = new QWidget();
    rightWidget->setObjectName("w2");
    rightWidget->setMinimumWidth(20);
    setLeftWidget(leftWidget);
    setRightWidget(rightWidget);

    setStyleSheet("#titlebar { background-color: black; }"
                  "QWidget#w1 { background-color: red; }"
                  "QWidget#w2 { background-color: yellow; }");
}

Widget::~Widget() {}
