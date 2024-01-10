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
    setObjectName("titlebar");
    setAttribute(Qt::WA_StyledBackground);
    initLayout();
    initConnect();
    setTitleAlignment(Qt::AlignCenter);
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

void EZTitlebar::setLeftWidget(QWidget *leftWidget)
{
    Q_D(EZTitlebar);
    auto oldWidget = d->m_hLayout->itemAt(0)->widget();
    if (oldWidget) {
        d->m_hLayout->removeWidget(oldWidget);
        oldWidget->hide();
        oldWidget->deleteLater();
    }
    d->m_hLayout->insertWidget(0, leftWidget);
}

void EZTitlebar::setRightWidget(QWidget *rightWidget)
{
    Q_D(EZTitlebar);
    auto oldWidget = d->m_hLayout->itemAt(2)->widget();
    if (oldWidget) {
        d->m_hLayout->removeWidget(oldWidget);
        oldWidget->hide();
        oldWidget->deleteLater();
    }
    d->m_hLayout->insertWidget(2, rightWidget);
}

void EZTitlebar::setMaximizeBtnStatus(bool isMaximize)
{
    Q_D(EZTitlebar);
    if (isMaximize) {
        d->m_maximizeBtn->setIcon(d->m_normalIcon);
    } else
        d->m_maximizeBtn->setIcon(d->m_maximizeIcon);
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
    auto layout = d->m_hLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);

    // Placeholder for the left-side widget.
    d->m_leftWidgetPlaceholder = new QWidget();
    layout->addWidget(d->m_leftWidgetPlaceholder);

    // Title and icon.
    d->m_titleLabel = new QLabel();
    layout->addWidget(d->m_titleLabel, 1); // Use a stretch factor to ensure the title and icon are centered.

    // Placeholder for the right-side widget.
    d->m_rightWidgetPlaceholder = new QWidget();
    layout->addWidget(d->m_rightWidgetPlaceholder);

    // System buttons.
    d->m_closeIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton);
    d->m_minimizeIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton);
    d->m_maximizeIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton);
    d->m_normalIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarNormalButton);

    auto createSysButton = [this](QPushButton *&btn, const QString &objName, const QIcon &icon = QIcon()) {
        btn = new QPushButton(icon, "", this);
        btn->setFixedSize(40, 28);
        btn->setObjectName(objName);
        btn->hide();
    };
    createSysButton(d->m_minimizeBtn, "SysMinimizeBtn", d->m_minimizeIcon);
    createSysButton(d->m_maximizeBtn, "SysMaximizeBtn", d->m_maximizeIcon);
    createSysButton(d->m_closeBtn, "SysCloseBtn", d->m_closeIcon);
    createSysButton(d->m_helpBtn, "SysHelpBtn");
    layout->addWidget(d->m_helpBtn);
    layout->addWidget(d->m_minimizeBtn);
    layout->addWidget(d->m_maximizeBtn);
    layout->addWidget(d->m_closeBtn);

    this->setStyleSheet(
        "QPushButton#SysHelpBtn, QPushButton#SysMinimizeBtn, QPushButton#SysMaximizeBtn, QPushButton#SysCloseBtn "
        "{ border: none; background: transparent;} "
        "QPushButton#SysHelpBtn:hover, QPushButton#SysMinimizeBtn:hover, QPushButton#SysMaximizeBtn:hover "
        "{ background-color: #e5e5e5; } "
        "QPushButton#SysCloseBtn:hover { background-color: red; }");
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
