#include "FontManager.h"
#include "IconFont.h"
#include "widget.h"

#include <QApplication>
#include <QFile>

bool loadStyleSheet(QApplication &a)
{
    QFile   file(":/rc/style.qss");
    QString styleSheet = "";
    if (file.open(QFile::ReadOnly)) {
        styleSheet = QLatin1String(file.readAll());

        a.setStyleSheet(styleSheet);
        file.close();
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qutils::FontManager::instance()->addThirdpartyFont(":rc/font/iconfont.ttf", FontStyle::kCustomIconFont);

    if (!loadStyleSheet(a))
        return 0;
    Widget w;
    w.show();

    return a.exec();
}
