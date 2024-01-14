#include "EZTitlebar.h"

#include <windows.h>
#include <QApplication>
#include <QDebug>
#include <QStyleOption>
//
#include <WinUser.h>

EZTitlebar::EZTitlebar(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new EZTitlebarPrivate)
{
    setObjectName("sysTitlebar");
    setAttribute(Qt::WA_StyledBackground);
    initLayout();
    initConnect();
    setTitleAlignment(Qt::AlignCenter);
    setSystemButtonStyle();
}

EZTitlebar::~EZTitlebar()
{
    delete d_ptr;
}

void EZTitlebar::setTitleText(const QString &text)
{
    Q_D(EZTitlebar);
    d->m_titleLabel->setText(text);
}

QString EZTitlebar::getTitleText()
{
    Q_D(EZTitlebar);
    return d->m_titleLabel->text();
}

void EZTitlebar::setSystemLogo(const QString &iconPath)
{
    Q_D(EZTitlebar);
    d->m_logoLabel->setStyleSheet(QString("#%1 { border-image: url(%2); }").arg(d->m_logoLabel->objectName(), iconPath));
}

void EZTitlebar::setTitleAlignment(Qt::Alignment alignment)
{
    Q_D(EZTitlebar);
    if (alignment == Qt::AlignLeft) {
        d->m_titleLabel->setAlignment(Qt::AlignLeft);
    } else {
        d->m_titleLabel->setAlignment(Qt::AlignCenter);
    }
}

void EZTitlebar::setTitleButtonFlag(SysButton btnFlag)
{
    Q_D(EZTitlebar);
    auto toggleButton = [&](SysButton flag, QPushButton *button) {
        if (btnFlag & flag)
            button->show();
        else
            button->hide();
    };

    toggleButton(SysButton::kHelp, d->m_helpBtn);
    toggleButton(SysButton::kMinimize, d->m_minimizeBtn);
    toggleButton(SysButton::kMaximize, d->m_maximizeBtn);
    toggleButton(SysButton::kClose, d->m_closeBtn);
}

void EZTitlebar::setSystemButtonStyle(SysButtonStyle style,
                                      const QFont   &font,
                                      QChar          closeIcon,
                                      QChar          minIcon,
                                      QChar          maxIcon,
                                      QChar          restoreIcon,
                                      QChar          helpIcon)
{
    Q_D(EZTitlebar);
    d->m_sysButtonStyle = style;
    if (SysButtonStyle::kSystemStyle == style) {
        d->m_closeIcon.setValue(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));
        d->m_minimizeIcon.setValue(QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton));
        d->m_maximizeIcon.setValue(QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton));
        d->m_restoreIcon.setValue(QApplication::style()->standardIcon(QStyle::SP_TitleBarNormalButton));
        d->m_helpIcon.setValue(QApplication::style()->standardIcon(QStyle::SP_TitleBarContextHelpButton));
    } else if (SysButtonStyle::kCustomIconFontStyle == style) {
        d->m_closeBtn->setFont(font);
        d->m_minimizeBtn->setFont(font);
        d->m_maximizeBtn->setFont(font);
        d->m_helpBtn->setFont(font);
        d->m_closeIcon.setValue(closeIcon);
        d->m_minimizeIcon.setValue(minIcon);
        d->m_maximizeIcon.setValue(maxIcon);
        d->m_restoreIcon.setValue(restoreIcon);
        d->m_helpIcon.setValue(helpIcon);
    }
    updateSysButtonIcon();
}

void EZTitlebar::setMaximizeBtnStatus(bool isMaximize)
{
    Q_D(EZTitlebar);
    d->m_isMaximized = isMaximize;
    if (d->m_isMaximized) {
        if (d->m_sysButtonStyle == SysButtonStyle::kSystemStyle)
            d->m_maximizeBtn->setIcon(d->m_restoreIcon.value<QIcon>());
        else if (d->m_sysButtonStyle == SysButtonStyle::kCustomIconFontStyle)
            d->m_maximizeBtn->setText(d->m_restoreIcon.toChar());
    } else {
        if (d->m_sysButtonStyle == SysButtonStyle::kSystemStyle)
            d->m_maximizeBtn->setIcon(d->m_maximizeIcon.value<QIcon>());
        else if (d->m_sysButtonStyle == SysButtonStyle::kCustomIconFontStyle)
            d->m_maximizeBtn->setText(d->m_maximizeIcon.toChar());
    }
}

void EZTitlebar::setCloseEventCallback(const CallbackFun &func)
{
    Q_D(EZTitlebar);
    d->m_closeEventCallback = func;
}

int EZTitlebar::getInsideSystemBtns(const QPoint &pt)
{
    Q_D(EZTitlebar);
    auto  originPt = mapTo(this, QPoint(0, 0));
    QRect titleRect = QRect(originPt, size());
    if (titleRect.contains(pt)) {
        if (d->m_helpBtn->isVisible() && d->m_helpBtn->rect().contains(d->m_helpBtn->mapFromParent(pt)))
            return HTCLIENT;
        else if (d->m_minimizeBtn->isVisible() && d->m_minimizeBtn->rect().contains(d->m_minimizeBtn->mapFromParent(pt)))
            return HTCLIENT;
        else if (d->m_maximizeBtn->isVisible() && d->m_maximizeBtn->rect().contains(d->m_maximizeBtn->mapFromParent(pt)))
            return HTCLIENT;
        else if (d->m_closeBtn->isVisible() && d->m_closeBtn->rect().contains(d->m_closeBtn->mapFromParent(pt)))
            return HTCLIENT;
        return HTCAPTION;
    }
    return HTCLIENT;
}

void EZTitlebar::initLayout()
{
    Q_D(EZTitlebar);
    // Create a layout.
    d->m_hLayout = new QHBoxLayout(this);
    d->m_hLayout->setContentsMargins(0, 0, 0, 0);
    d->m_hLayout->setSpacing(0);

    // Title and icon.
    d->m_logoLabel = new QLabel();
    d->m_logoLabel->setObjectName("sysLogoLabel");
    d->m_logoLabel->setFixedSize(d->m_titleHeight - 2, d->m_titleHeight - 2);
    d->m_hLayout->addWidget(d->m_logoLabel);
    d->m_titleLabel = new QLabel();
    d->m_titleLabel->setObjectName("sysTitleLabel");
    d->m_hLayout->addWidget(d->m_titleLabel); // Use a stretch factor to ensure the title and icon are centered.

    d->m_hLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));

    auto createSysButton = [this](QPushButton *&btn, const QString &objName) {
        btn = new QPushButton(this);
        btn->setFixedSize(40, 28);
        btn->setObjectName(objName);
        btn->hide();
    };
    createSysButton(d->m_minimizeBtn, "sysMinimizeBtn");
    createSysButton(d->m_maximizeBtn, "sysMaximizeBtn");
    createSysButton(d->m_closeBtn, "sysCloseBtn");
    createSysButton(d->m_helpBtn, "sysHelpBtn");
    d->m_hLayout->addWidget(d->m_helpBtn);
    d->m_hLayout->addWidget(d->m_minimizeBtn);
    d->m_hLayout->addWidget(d->m_maximizeBtn);
    d->m_hLayout->addWidget(d->m_closeBtn);
}

void EZTitlebar::initConnect()
{
    Q_D(EZTitlebar);
    connect(d->m_closeBtn, &QPushButton::clicked, this, [d, this]() {
        if (!d->m_closeEventCallback)
            parentWidget()->close();
        else
            d->m_closeEventCallback();
    });
    connect(d->m_minimizeBtn, &QPushButton::clicked, this, [this]() { parentWidget()->showMinimized(); });
    connect(d->m_maximizeBtn, &QPushButton::clicked, this, [this]() {
        if (parentWidget()->isMaximized())
            parentWidget()->showNormal();
        else
            parentWidget()->showMaximized();
    });
}

void EZTitlebar::updateSysButtonIcon()
{
    Q_D(EZTitlebar);
    if (SysButtonStyle::kSystemStyle == d->m_sysButtonStyle) {
        if (d->m_closeIcon.canConvert<QIcon>())
            d->m_closeBtn->setIcon(d->m_closeIcon.value<QIcon>());
        if (d->m_minimizeIcon.canConvert<QIcon>())
            d->m_minimizeBtn->setIcon(d->m_minimizeIcon.value<QIcon>());
        if (d->m_isMaximized && d->m_restoreIcon.canConvert<QIcon>())
            d->m_maximizeBtn->setIcon(d->m_restoreIcon.value<QIcon>());
        else if (!d->m_isMaximized && d->m_maximizeIcon.canConvert<QIcon>())
            d->m_maximizeBtn->setIcon(d->m_maximizeIcon.value<QIcon>());
        if (d->m_helpIcon.canConvert<QIcon>())
            d->m_helpBtn->setIcon(d->m_helpIcon.value<QIcon>());
    } else if (SysButtonStyle::kCustomIconFontStyle == d->m_sysButtonStyle) {
        d->m_closeBtn->setIcon(QIcon());
        d->m_minimizeBtn->setIcon(QIcon());
        d->m_maximizeBtn->setIcon(QIcon());
        d->m_helpBtn->setIcon(QIcon());
        d->m_closeBtn->setText(d->m_closeIcon.toChar());
        d->m_minimizeBtn->setText(d->m_minimizeIcon.toChar());
        if (d->m_isMaximized)
            d->m_maximizeBtn->setText(d->m_restoreIcon.toChar());
        else
            d->m_maximizeBtn->setText(d->m_maximizeIcon.toChar());
        d->m_helpBtn->setText(d->m_helpIcon.toChar());
    }
}
