#ifndef SIMPLEFRAMELESS_H
#define SIMPLEFRAMELESS_H

#include "EZTitlebar.h"

#include <QWidget>

class EZFramelessPrivate;

class EZFrameless : public QWidget
{
    Q_OBJECT
public:
    explicit EZFrameless(QWidget *parent = nullptr);

    void setResizeable(bool resizeable = true);

    void    setTitleText(const QString &text);
    QString getTitleText();

    void setSystemLogo(const QString &iconPath);

    void setTitleAlignment(Qt::Alignment alignment);
    void setTitleButtonFlag(EZTitlebar::SysButton btnFlag);

    void setSystemButtonStyle(EZTitlebar::SysButtonStyle style = EZTitlebar::SysButtonStyle::kSystemStyle,
                              const QFont               &font = QFont(),
                              QChar                      closeIcon = QChar(),
                              QChar                      minIcon = QChar(),
                              QChar                      maxIcon = QChar(),
                              QChar                      restoreIcon = QChar(),
                              QChar                      helpIcon = QChar());

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
    EZFramelessPrivate *d_ptr;
    Q_DECLARE_PRIVATE(EZFrameless)
};

class EZFramelessPrivate
{
public:
    EZFramelessPrivate(QWidget *parent)
        : m_titlebar(new EZTitlebar(parent))
    {}

    EZTitlebar *m_titlebar;
    bool        m_bResizeable{true};
    QMargins    m_margins;
    QMargins    m_frames;
    QRect       m_widgetRect;
};

#endif // SIMPLEFRAMELESS_H
