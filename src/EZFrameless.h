#ifndef SIMPLEFRAMELESS_H
#define SIMPLEFRAMELESS_H

#include "EZTitlebar.h"

#include <QWidget>

class EZFrameless : public QWidget
{
    Q_OBJECT
public:
    explicit EZFrameless(QWidget *parent = nullptr);

    void setResizeable(bool resizeable = true);

    void    setTitleText(const QString &text);
    QString getTitleText();

    void setTitleAlignment(Qt::Alignment alignment) { m_titlebar->setTitleAlignment(alignment); }
    void setTitleButtonFlag(EZTitlebar::SysButton btnFlag) { m_titlebar->setTitleButtonFlag(btnFlag); }
    void setLeftWidget(QWidget *leftWidget) { m_titlebar->setLeftWidget(leftWidget); }
    void setRightWidget(QWidget *rightWidget) { m_titlebar->setRightWidget(rightWidget); }

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
    EZTitlebar *m_titlebar;
    bool        m_bResizeable;
    QMargins    m_margins;
    QMargins    m_frames;

    QRect m_widgetRect;
};

#endif // SIMPLEFRAMELESS_H
