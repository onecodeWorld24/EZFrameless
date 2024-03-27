#ifndef FRAMEEVENTFILTER_H
#define FRAMEEVENTFILTER_H

#include <QDebug>
#include <QEvent>
#include <QObject>

class Frameless;

class FrameEventFilter : public QObject {
    Q_OBJECT
public:
    static FrameEventFilter* instance()
    {
        static FrameEventFilter instance;
        return &instance;
    }

    bool eventFilter(QObject* watched, QEvent* event) override;

    void addResizeWidget(Frameless* widget)
    {
        m_resizeWidgets.append(widget);
    }

    void removeResizeWidget(Frameless* widget)
    {
        m_resizeWidgets.removeOne(widget);
    }

private:
    explicit FrameEventFilter(QObject* parent = nullptr);
    QList<Frameless*> m_resizeWidgets;
};

#endif // FRAMEEVENTFILTER_H
