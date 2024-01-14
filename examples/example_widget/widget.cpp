#include "widget.h"
#include "FontManager.h"
#include "IconFont.h"

Widget::Widget(QWidget *parent)
    : EZFrameless(parent)
{
    setResizeable(true);
    setTitleText("frameless_widget");
    setSystemLogo(":/rc/logo.ico");
    setSystemButtonStyle(EZTitlebar::kCustomIconFontStyle,
                         qutils::FontManager::instance()->fontAt(FontStyle::kCustomIconFont),
                         FONT_CLOSE,
                         FONT_MINIMIZE,
                         FONT_MAXIMIZE,
                         FONT_RESTORE,
                         FONT_HELP);
}

Widget::~Widget() {}
