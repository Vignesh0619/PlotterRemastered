
#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("TonboLogo.png"));
    w.setWindowTitle("TEMS");
    w.show();
    return a.exec();
}
