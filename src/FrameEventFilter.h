#ifndef FRAMEEVENTFILTER_H
#define FRAMEEVENTFILTER_H

#include <QAbstractNativeEventFilter>
#include <QWidget>

class FrameEventFilter : public QAbstractNativeEventFilter
{
public:
    enum EventType { ET_MouseSize = 0 };
    explicit FrameEventFilter();

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

    void setMainWindow(QWidget *widget) { m_mainWindow = widget; }

private:
    QWidget *m_mainWindow;
};

#endif // FRAMEEVENTFILTER_H
