#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mm;
    mm.setWindowTitle("Sea Battle");
    mm.show();

    return a.exec();

}
