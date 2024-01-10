#include "EZFrameless.h"
#include <QDebug>
#include <QIcon>
#include <QPainter>
#include <QPainterPath>
#include <QStyle>

#include <windows.h>
//
#include <WinUser.h>
#include <dwmapi.h>
#include <windowsx.h>

EZFrameless::EZFrameless(QWidget *parent)
    : QWidget{parent}
    , m_titlebar(new EZTitlebar(this))
    , m_bResizeable(false)
{
    setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

    setResizeable(m_bResizeable);
    setMinimumSize(400, 500);
}

void EZFrameless::setResizeable(bool resizeable)
{
    m_bResizeable = resizeable;
    HWND  hwnd = (HWND) this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    if (m_bResizeable) {
        ::SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX);
        m_titlebar->setTitleButtonFlag(EZTitlebar::kMinMaxClose);
    } else {
        ::SetWindowLong(hwnd, GWL_STYLE, style & ~WS_MAXIMIZEBOX | WS_CAPTION);
        m_titlebar->setTitleButtonFlag(EZTitlebar::kClose);
    }

    const int     num = 4;
    const MARGINS shadow = {num, num, num, num};
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
}

void EZFrameless::setTitleText(const QString &text)
{
    m_titlebar->setTitleText(text);
}

QString EZFrameless::getTitleText()
{
    return m_titlebar->getTitleText();
}

bool EZFrameless::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = reinterpret_cast<MSG *>(message);
    switch (msg->message) {
    // To manage the layout and size of the non-client area of a window (i.e., the window's borders, title bar, etc.).
    case WM_NCCALCSIZE: {
        const auto clientRect = msg->wParam == FALSE ? reinterpret_cast<LPRECT>(msg->lParam)
                                                     : &(reinterpret_cast<LPNCCALCSIZE_PARAMS>(msg->lParam))->rgrc[0];
        // To resolve the issue of abnormal refresh of the right border when horizontally stretching the window.
        if (clientRect->top != 0)
            clientRect->top -= 1;

        if (::IsZoomed(msg->hwnd)) {
            // To solve the issue where the actual size of the window exceeds the screen size after maximizing.
            const int borderOutside = 8;
            clientRect->left += borderOutside;
            clientRect->right -= borderOutside;
            clientRect->top += borderOutside;
            clientRect->bottom -= borderOutside;
            m_titlebar->setMaximizeBtnStatus(true);
        } else
            m_titlebar->setMaximizeBtnStatus(false);

        // Customize the title bar to resize dynamically with changes in the window size.
        //  The reason for using move(0, 1) is that when the mouse hovers over the buttons on the title bar,
        //  the upper boundary of the button's hover state exceeds the boundary of the client area by 1px.
        m_titlebar->move(0, 1);
        m_titlebar->setFixedSize(clientRect->right - clientRect->left, 28);
        *result = WVR_REDRAW;
        return true;
    } break;
    case WM_NCHITTEST: { // 鼠标悬浮消息
        *result = 0;
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        POINT nativeLocalPos{x, y};
        ::ScreenToClient(msg->hwnd, &nativeLocalPos);
        QPoint pt(nativeLocalPos.x, nativeLocalPos.y);

        const int borderWidth = 5;

        if (m_bResizeable) {
            bool bResizeWidth = minimumWidth() != maximumWidth();
            bool bResizeHeight = minimumHeight() != maximumHeight();

            if (bResizeWidth) {
                // left border
                if (pt.x() >= rect().left() && pt.x() < rect().left() + borderWidth)
                    *result = HTLEFT;
                // right border
                if (pt.x() < rect().right() && pt.x() >= rect().right() - borderWidth)
                    *result = HTRIGHT;
            }
            if (bResizeHeight) {
                // top border
                if (pt.y() >= rect().top() && pt.y() < rect().top() + borderWidth)
                    *result = HTTOP;
                // bottom border
                if (pt.y() < rect().bottom() && pt.y() >= rect().bottom() - borderWidth)
                    *result = HTBOTTOM;
            }
            if (bResizeWidth && bResizeHeight) {
                // left top corner
                if (pt.x() >= rect().left() && pt.x() < rect().left() + borderWidth && pt.y() >= rect().top()
                    && pt.y() < rect().top() + borderWidth) {
                    *result = HTTOPLEFT;
                }
                // right top corner
                if (pt.x() < rect().right() && pt.x() >= rect().right() - borderWidth && pt.y() >= rect().top()
                    && pt.y() < rect().top() + borderWidth) {
                    *result = HTTOPRIGHT;
                }
                // right bottom corner
                if (pt.x() < rect().right() && pt.x() >= rect().right() - borderWidth && pt.y() < rect().bottom()
                    && pt.y() >= rect().bottom() - borderWidth) {
                    *result = HTBOTTOMRIGHT;
                }
                // left bottom corner
                if (pt.x() >= rect().left() && pt.x() < rect().left() + borderWidth && pt.y() < rect().bottom()
                    && pt.y() >= rect().bottom() - borderWidth) {
                    *result = HTBOTTOMLEFT;
                }
            }
        }
        if (0 != *result)
            return true;

        QPoint qtScenePos = QPointF(QPointF(pt / this->devicePixelRatio())).toPoint();
        int    ret = m_titlebar->getInsideSystemBtns(pt);
        if (ret == HTCLIENT)
            return QWidget::nativeEvent(eventType, message, result);
        else {
            *result = ret;
            return true;
        }
    } break;
    default:
        return QWidget::nativeEvent(eventType, message, result);
        break;
    }
}
