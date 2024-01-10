#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#if 1
    QApplication a(argc, argv);
    Widget       w;
    w.show();
#else
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();
#endif
    return a.exec();
}
