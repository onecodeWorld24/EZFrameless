#ifndef EZTITLEBAR_H
#define EZTITLEBAR_H

#include <functional>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVariant>
#include <QWidget>

/**
 * @brief: Frameless title bar.
 * @note: The overall layout of the title bar is as follows:
 *  --------------------------------------------------------------------------
 *  | left-side widget | title and icon | right-side widget | system buttons |
 *  | ---------------- | -------------- | ----------------- | -------------- |
 *
 *  left-side widget: Placeholder for the window on the left side of the title bar.
 *  right-side widget: Placeholder for the window on the right side of the title bar.
 *  system buttons: The system buttons mainly include Help, Minimize, Maximize, and Close.
 */

using CallbackFun = std::function<void(void)>;

class EZTitlebarPrivate;

class EZTitlebar : public QWidget
{
    Q_OBJECT
public:
    enum SysButton {
        kNone = 0,
        kHelp = 1 << 0,
        kMinimize = 1 << 1,
        kMaximize = 1 << 2,
        kClose = 1 << 3,
        kMinimizeClose = kMinimize | kClose,
        kMaximizeClose = kMaximize | kClose,
        kMinMaxClose = kMinimize | kMaximize | kClose,
        kAll = kHelp | kMinMaxClose
    };
    enum SysButtonStyle { kSystemStyle = 0, kCustomIconFontStyle };

    explicit EZTitlebar(QWidget *parent = nullptr);
    ~EZTitlebar();

    void    setTitleText(const QString &text);
    QString getTitleText();

    void setSystemLogo(const QString &iconPath);

    void setTitleAlignment(Qt::Alignment alignment);
    void setTitleButtonFlag(SysButton btnFlag);

    void setSystemButtonStyle(SysButtonStyle style = SysButtonStyle::kSystemStyle,
                              const QFont   &font = QFont(),
                              QChar          closeIcon = QChar(),
                              QChar          minIcon = QChar(),
                              QChar          maxIcon = QChar(),
                              QChar          restoreIcon = QChar(),
                              QChar          helpIcon = QChar());

    void setMaximizeBtnStatus(bool isMaximize);
    void setCloseEventCallback(const CallbackFun &func);

    int getInsideSystemBtns(const QPoint &pt);

protected:
    void initLayout();
    void initConnect();

    void updateSysButtonIcon();

private:
    EZTitlebarPrivate *d_ptr;
    Q_DECLARE_PRIVATE(EZTitlebar)
};

class EZTitlebarPrivate
{
public:
    int  m_titleHeight{25};
    bool m_isMaximized{false};

    EZTitlebar::SysButtonStyle m_sysButtonStyle;

    QHBoxLayout *m_hLayout{nullptr};
    QLabel      *m_logoLabel{nullptr};
    QLabel      *m_titleLabel{nullptr};
    QPushButton *m_helpBtn{nullptr};
    QPushButton *m_minimizeBtn{nullptr};
    QPushButton *m_maximizeBtn{nullptr};
    QPushButton *m_closeBtn{nullptr};

    // Button icon
    QVariant m_closeIcon;
    QVariant m_minimizeIcon;
    QVariant m_maximizeIcon;
    QVariant m_restoreIcon;
    QVariant m_helpIcon;

    // The callback function for the close button event:
    // The close button by default calls the close() function of the parent window object.
    // The event function can be overridden through the callback function.
    CallbackFun m_closeEventCallback{nullptr};
};

#endif // EZTITLEBAR_H
