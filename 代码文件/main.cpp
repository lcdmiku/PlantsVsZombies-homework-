#include "mainscene.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //setting
    QCoreApplication::setApplicationName("plant Vs zombies");
    MainScene w;
    w.show();
    return a.exec();
}
