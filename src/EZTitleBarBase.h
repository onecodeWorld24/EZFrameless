#ifndef EZTITLEBARBASE_H
#define EZTITLEBARBASE_H

#include <QApplication>
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
        if (rect().contains(mapFromParent(pt)))
            return true;
        else
            return false;
    }

public slots:
    virtual void onClosedClicked() { qApp->quit(); }
    virtual void onMinimizedClicked()
    {
        if (parentWidget())
            parentWidget()->showMinimized();
    }
    virtual void onMaximizedClicked()
    {
        if (parentWidget())
            if (parentWidget()->isMaximized())
                parentWidget()->showNormal();
            else
                parentWidget()->showMaximized();
    }
    virtual void onSettingClicked() {}

    virtual void onMaximized() {}
    virtual void onNormaled() {}
};

#endif // EZTITLEBARBASE_H
