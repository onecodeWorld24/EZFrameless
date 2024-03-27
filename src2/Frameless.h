#ifndef FRAMELESS_H
#define FRAMELESS_H

#include "FrameEventFilter.h"
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>

class Frameless : public QWidget {
    Q_OBJECT
public:
    enum ResizeMode {
        NoResize,
        TopResize,
        BottomResize,
        LeftResize,
        RightResize,
        TopLeftResize,
        TopRightResize,
        BottomLeftResize,
        BottomRightResize
    };
    explicit Frameless(QWidget* parent = nullptr);

    void setResizeFlag(bool bResize) { m_bresizeFlag = bResize; }

    void startResizing(const QPoint& pt);
    void moveToResizing(const QPoint& pt);
    void stopResizing(const QPoint& pt);

    ResizeMode getResizeMode();
    ResizeMode getResizeMode(const QPoint& pos);
    void setResizeCursor(ResizeMode mode);

    void stretchWidget(const QPoint& diff);

    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

private:
    ResizeMode m_resizeMode = NoResize;

    bool m_bresizing = false;
    bool m_bresizeFlag = false;
    QPoint m_pressedPt;
};

#endif // FRAMELESS_H
