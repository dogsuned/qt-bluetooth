#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QObject::connect(&d, SIGNAL(accepted()), &app, SLOT(quit()));

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
