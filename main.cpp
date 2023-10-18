#include "clientwidget.h"
#include "serverwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientWidget w;
    w.show();
    w.move(320, 300);
    ServerWidget s;
    s.show();
    s.move(960, 300);
    return a.exec();
}
