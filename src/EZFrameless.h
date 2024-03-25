#ifndef SIMPLEFRAMELESS_H
#define SIMPLEFRAMELESS_H

#include "EZTitleBarBase.h"
#include "FrameEventFilter.h"

#include <QWidget>

class EZFramelessPrivate;

class EZFrameless : public QWidget
{
    Q_OBJECT
public:
    explicit EZFrameless(QWidget *parent = nullptr);

    void setResizeable(bool resizeable = true);

    void setTitlebarWidget(EZTitleBarBase *widget);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    void changeEvent(QEvent *event) override;

private:
    EZFramelessPrivate *d_ptr;
    Q_DECLARE_PRIVATE(EZFrameless)
};

class EZFramelessPrivate
{
public:
    EZFramelessPrivate(QWidget *parent)
        : m_titlebar(new EZTitleBarBase(parent))
        , m_eventFilter(new FrameEventFilter())
    {}

    EZTitleBarBase   *m_titlebar;
    bool              m_bResizeable{true};
    QMargins          m_margins;
    QMargins          m_frames;
    QRect             m_widgetRect;
    FrameEventFilter *m_eventFilter;
};

#endif // SIMPLEFRAMELESS_H
