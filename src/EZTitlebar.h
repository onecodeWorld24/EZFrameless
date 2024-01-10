#ifndef EZTITLEBAR_H
#define EZTITLEBAR_H

#include <functional>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
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
    explicit EZTitlebar(QWidget *parent = nullptr);
    ~EZTitlebar();

    void    setTitleText(const QString &text);
    QString getTitleText();

    void setTitleAlignment(Qt::Alignment alignment);
    void setTitleButtonFlag(SysButton btnFlag);
    void setLeftWidget(QWidget *leftWidget);
    void setRightWidget(QWidget *rightWidget);

    void setMinimizeBtnIcon(const QIcon &icon, const QChar &);
    void setMaximizeBtnIcon(const QIcon &icon);
    void setNormalBtnIcon(const QIcon &icon);
    void setCloseBtnIcon(const QIcon &icon);

    void setMaximizeBtnStatus(bool isMaximize);
    void setCloseEventCallback(const CallbackFun &func);

    int getInsideSystemBtns(const QPoint &pt);

protected:
    void initLayout();

    void initConnect();

private:
    EZTitlebarPrivate *d_ptr;
    Q_DECLARE_PRIVATE(EZTitlebar)
};

class EZTitlebarPrivate
{
public:
    QHBoxLayout *m_hLayout{nullptr};
    QLabel      *m_titleLabel{nullptr};
    QWidget     *m_leftWidgetPlaceholder{nullptr};
    QWidget     *m_rightWidgetPlaceholder{nullptr};
    QPushButton *m_helpBtn{nullptr};
    QPushButton *m_minimizeBtn{nullptr};
    QPushButton *m_maximizeBtn{nullptr};
    QPushButton *m_closeBtn{nullptr};
    QIcon        m_closeIcon;
    QIcon        m_minimizeIcon;
    QIcon        m_maximizeIcon;
    QIcon        m_normalIcon;
    // The callback function for the close button event:
    // The close button by default calls the close() function of the parent window object.
    // The event function can be overridden through the callback function.
    CallbackFun m_closeEventCallback{nullptr};
};

#endif // EZTITLEBAR_H
