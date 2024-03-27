#include "FrameEventFilter.h"
#include "frameless.h"

FrameEventFilter::FrameEventFilter(QObject* parent)
    : QObject { parent }
{
}

bool FrameEventFilter::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::MouseMove) {
        auto msEvent = static_cast<QMouseEvent*>(event);
        if (msEvent->buttons() & Qt::LeftButton) {
            for (auto frame : m_resizeWidgets) {
                if (frame->isActiveWindow() && frame->getResizeMode() != Frameless::ResizeMode::NoResize) {
                    frame->moveToResizing(msEvent->globalPos());
                    break;
                }
            }
        } else {
            for (auto frame : m_resizeWidgets) {
                frame->setResizeCursor(frame->getResizeMode(static_cast<QMouseEvent*>(event)->globalPos()));
            }
        }
    } else if (event->type() == QEvent::MouseButtonPress) {
        auto msEvent = static_cast<QMouseEvent*>(event);
        if (msEvent->button() == Qt::LeftButton) {
            for (auto frame : m_resizeWidgets) {
                if (frame->getResizeMode(msEvent->globalPos()) != Frameless::ResizeMode::NoResize) {
                    frame->startResizing(msEvent->globalPos());
                    break;
                }
            }
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        auto msEvent = static_cast<QMouseEvent*>(event);
        if (msEvent->button() == Qt::LeftButton) {
            for (auto frame : m_resizeWidgets) {
                if (frame->getResizeMode() != Frameless::ResizeMode::NoResize) {
                    frame->stopResizing(msEvent->globalPos());
                    break;
                }
            }
        }
    }
    return QObject::eventFilter(watched, event);
}
