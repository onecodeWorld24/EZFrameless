#include "EZFrameless.h"
#include <QDebug>
#include <QIcon>
#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#include <QStyle>
#include <QWindow>

#include <windows.h>
//
#include <WinUser.h>
#include <dwmapi.h>
#include <windowsx.h>

EZFrameless::EZFrameless(QWidget *parent)
    : QWidget{parent}
    , d_ptr(new EZFramelessPrivate(this))
{
    Q_D(EZFrameless);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

    setResizeable(d->m_bResizeable);
    setMinimumSize(400, 500);

    connect(windowHandle(), &QWindow::screenChanged, this, [=] {
        auto hWnd = reinterpret_cast<HWND>(windowHandle()->winId());
        SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    });
}

void EZFrameless::setResizeable(bool resizeable)
{
    Q_D(EZFrameless);
    d->m_bResizeable = resizeable;
    HWND  hwnd = (HWND) this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    if (d->m_bResizeable) {
        ::SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX);
        d->m_titlebar->setTitleButtonFlag(EZTitlebar::kMinMaxClose);
    } else {
        ::SetWindowLong(hwnd, GWL_STYLE, (style & ~WS_MAXIMIZEBOX) | WS_CAPTION);
        d->m_titlebar->setTitleButtonFlag(EZTitlebar::kClose);
    }

    const int     num = 4;
    const MARGINS shadow = {num, num, num, num};
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
}

void EZFrameless::setTitleText(const QString &text)
{
    Q_D(EZFrameless);
    d->m_titlebar->setTitleText(text);
}

QString EZFrameless::getTitleText()
{
    Q_D(EZFrameless);
    return d->m_titlebar->getTitleText();
}

void EZFrameless::setSystemLogo(const QString &iconPath)
{
    Q_D(EZFrameless);
    d->m_titlebar->setSystemLogo(iconPath);
}

void EZFrameless::setTitleAlignment(Qt::Alignment alignment)
{
    Q_D(EZFrameless);
    d->m_titlebar->setTitleAlignment(alignment);
}

void EZFrameless::setTitleButtonFlag(EZTitlebar::SysButton btnFlag)
{
    Q_D(EZFrameless);
    d->m_titlebar->setTitleButtonFlag(btnFlag);
}

void EZFrameless::setSystemButtonStyle(EZTitlebar::SysButtonStyle style,
                                       const QFont               &font,
                                       QChar                      closeIcon,
                                       QChar                      minIcon,
                                       QChar                      maxIcon,
                                       QChar                      restoreIcon,
                                       QChar                      helpIcon)
{
    Q_D(EZFrameless);
    d->m_titlebar->setSystemButtonStyle(style, font, closeIcon, minIcon, maxIcon, restoreIcon, helpIcon);
}

bool EZFrameless::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_D(EZFrameless);
    MSG *msg = reinterpret_cast<MSG *>(message);
    switch (msg->message) {
    // To manage the layout and size of the non-client area of a window (i.e., the window's borders, title bar, etc.).
    case WM_NCCALCSIZE: {
        const auto clientRect = msg->wParam == FALSE ? reinterpret_cast<LPRECT>(msg->lParam)
                                                     : &(reinterpret_cast<LPNCCALCSIZE_PARAMS>(msg->lParam))->rgrc[0];
        d->m_widgetRect = QRect(0, 0, clientRect->right - clientRect->left, clientRect->bottom - clientRect->top);
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
            d->m_titlebar->setMaximizeBtnStatus(true);
        } else
            d->m_titlebar->setMaximizeBtnStatus(false);

        // Customize the title bar to resize dynamically with changes in the window size.
        //  The reason for using move(0, 1) is that when the mouse hovers over the buttons on the title bar,
        //  the upper boundary of the button's hover state exceeds the boundary of the client area by 1px.
        d->m_titlebar->move(0, 1);
        d->m_titlebar->setFixedSize(clientRect->right - clientRect->left, 28);
        *result = WVR_REDRAW;
        return true;
    } break;
    case WM_NCHITTEST: {
        *result = 0;
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        POINT nativeLocalPos{x, y};
        ::ScreenToClient(msg->hwnd, &nativeLocalPos);
        QPoint pt(nativeLocalPos.x, nativeLocalPos.y);

        const int borderWidth = 5;

        if (d->m_bResizeable) {
            bool bResizeWidth = minimumWidth() != maximumWidth();
            bool bResizeHeight = minimumHeight() != maximumHeight();

            if (bResizeWidth) {
                // left border
                if (pt.x() >= d->m_widgetRect.left() && pt.x() < d->m_widgetRect.left() + borderWidth)
                    *result = HTLEFT;
                // right border
                if (pt.x() < d->m_widgetRect.right() && pt.x() >= d->m_widgetRect.right() - borderWidth)
                    *result = HTRIGHT;
            }
            if (bResizeHeight) {
                // top border
                if (pt.y() >= d->m_widgetRect.top() && pt.y() < d->m_widgetRect.top() + borderWidth)
                    *result = HTTOP;
                // bottom border
                if (pt.y() < d->m_widgetRect.bottom() && pt.y() >= d->m_widgetRect.bottom() - borderWidth)
                    *result = HTBOTTOM;
            }
            if (bResizeWidth && bResizeHeight) {
                // left top corner
                if (pt.x() >= d->m_widgetRect.left() && pt.x() < d->m_widgetRect.left() + borderWidth
                    && pt.y() >= d->m_widgetRect.top() && pt.y() < d->m_widgetRect.top() + borderWidth) {
                    *result = HTTOPLEFT;
                }
                // right top corner
                if (pt.x() < d->m_widgetRect.right() && pt.x() >= d->m_widgetRect.right() - borderWidth
                    && pt.y() >= d->m_widgetRect.top() && pt.y() < d->m_widgetRect.top() + borderWidth) {
                    *result = HTTOPRIGHT;
                }
                // right bottom corner
                if (pt.x() < d->m_widgetRect.right() && pt.x() >= d->m_widgetRect.right() - borderWidth
                    && pt.y() < d->m_widgetRect.bottom() && pt.y() >= d->m_widgetRect.bottom() - borderWidth) {
                    *result = HTBOTTOMRIGHT;
                }
                // left bottom corner
                if (pt.x() >= d->m_widgetRect.left() && pt.x() < d->m_widgetRect.left() + borderWidth
                    && pt.y() < d->m_widgetRect.bottom() && pt.y() >= d->m_widgetRect.bottom() - borderWidth) {
                    *result = HTBOTTOMLEFT;
                }
            }
        }
        if (0 != *result)
            return true;
        int ret = d->m_titlebar->getInsideSystemBtns(pt);
        if (ret != HTCLIENT) {
            *result = ret;
            return true;
        }
    } break;
    default:
        break;
    }
    return QWidget::nativeEvent(eventType, message, result);
}
