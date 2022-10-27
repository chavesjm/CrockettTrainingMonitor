#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString serialPort;

    if(argc >= 2){
        serialPort = argv[1];
    }

    MainWindow w(serialPort);
    w.show();

    return a.exec();
}
