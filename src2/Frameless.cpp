#include "Frameless.h"
#include <QScreen>
#include <QWindow>

#include <windows.h>
//
#include <WinUser.h>
#include <dwmapi.h>
#include <windowsx.h>

Frameless::Frameless(QWidget* parent)
    : QWidget { parent }
{
    setMouseTracking(true);
    FrameEventFilter::instance()->addResizeWidget(this);
    installEventFilter(FrameEventFilter::instance());

    const int num = 4;
    const MARGINS shadow = { num, num, num, num };
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
}

void Frameless::startResizing(const QPoint& pt)
{
    m_bresizing = true;
    m_pressedPt = pt;
}

void Frameless::moveToResizing(const QPoint& pt)
{
    if (m_bresizing) {
        auto diff = pt - m_pressedPt;
        stretchWidget(diff);
        m_pressedPt = pt;
    }
}

void Frameless::stopResizing(const QPoint& pt)
{
    m_bresizing = false;
}

Frameless::ResizeMode Frameless::getResizeMode() { return m_resizeMode; }

Frameless::ResizeMode Frameless::getResizeMode(const QPoint& pos)
{
    auto pt = mapFromGlobal(pos);
    const int borderSize = 8; // 拉伸边框的宽度
    ResizeMode mode = NoResize;
    bool bResizeWidth = minimumWidth() != maximumWidth();
    bool bResizeHeight = minimumHeight() != maximumHeight();

    if (bResizeWidth) {
        if (pt.x() >= 0 && pt.x() <= borderSize)
            mode = LeftResize;
        if (pt.x() >= width() - borderSize && pt.x() <= width())
            mode = RightResize;
    }
    if (bResizeHeight) {
        if (pt.y() >= 0 && pt.y() <= borderSize)
            mode = TopResize;
        if (pt.y() >= height() - borderSize && pt.y() <= height())
            mode = BottomResize;
    }
    if (bResizeWidth && bResizeHeight) {
        if (pt.x() <= borderSize && pt.y() <= borderSize)
            mode = TopLeftResize;
        else if (pt.x() <= borderSize && pt.y() >= height() - borderSize)
            mode = BottomLeftResize;
        else if (pt.x() >= width() - borderSize && pt.y() <= borderSize)
            mode = TopRightResize;
        else if (pt.x() >= width() - borderSize && pt.y() >= height() - borderSize)
            mode = BottomRightResize;
    }

    m_resizeMode = mode;
    return mode;
}

void Frameless::setResizeCursor(ResizeMode mode)
{
    Qt::CursorShape cursorShape;
    switch (mode) {
    case TopResize:
    case BottomResize:
        cursorShape = Qt::SizeVerCursor;
        break;
    case LeftResize:
    case RightResize:
        cursorShape = Qt::SizeHorCursor;
        break;
    case TopLeftResize:
    case BottomRightResize:
        cursorShape = Qt::SizeFDiagCursor;
        break;
    case TopRightResize:
    case BottomLeftResize:
        cursorShape = Qt::SizeBDiagCursor;
        break;
    default:
        cursorShape = Qt::ArrowCursor;
        break;
    }
    setCursor(cursorShape);
}

void Frameless::stretchWidget(const QPoint& diff)
{
    auto rect = geometry();
    switch (m_resizeMode) {
    case TopResize:
        setGeometry(rect.left(), rect.top() + diff.y(), rect.width(), rect.height() - diff.y());
        break;
    case RightResize:
        setGeometry(rect.left(), rect.top(), rect.width() + diff.x(), rect.height());
        break;
    case BottomResize:
        setGeometry(rect.left(), rect.top(), rect.width(), rect.height() + diff.y());
        break;
    case LeftResize:
        setGeometry(rect.left() + diff.x(), rect.top(), rect.width() - diff.x(), rect.height());
        break;
    case TopLeftResize:
        setGeometry(rect.left() + diff.x(), rect.top() + diff.y(), rect.width() - diff.x(), rect.height() - diff.y());
        break;
    case BottomRightResize:
        setGeometry(rect.left(), rect.top(), rect.width() + diff.x(), rect.height() + diff.y());
        break;
    case TopRightResize:
        setGeometry(rect.left(), rect.top() + diff.y(), rect.width() + diff.x(), rect.height() - diff.y());
        break;
    case BottomLeftResize:
        setGeometry(rect.left() + diff.x(), rect.top(), rect.width() - diff.x(), rect.height() + diff.y());
        break;
    default:
        break;
    }
}

bool Frameless::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    MSG* msg = reinterpret_cast<MSG*>(message);
    switch (msg->message) {
    case WM_NCCALCSIZE: {
        const auto clientRect = msg->wParam == FALSE ? reinterpret_cast<LPRECT>(msg->lParam)
                                                     : &(reinterpret_cast<LPNCCALCSIZE_PARAMS>(msg->lParam))->rgrc[0];

        if (::IsZoomed(msg->hwnd)) {
            // To solve the issue where the actual size of the window exceeds the screen size after maximizing.
            const int borderOutside = 8;
            clientRect->left += borderOutside;
            clientRect->right -= borderOutside;
            clientRect->top += borderOutside;
            clientRect->bottom -= borderOutside;
        }

        *result = WVR_REDRAW;
        return true;
    } break;
    default:
        break;
    }
    return QWidget::nativeEvent(eventType, message, result);
}
