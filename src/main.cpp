#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QProcess* proc = new QProcess();
    int exitCode = proc->execute("/home/aghasi_m/Aghas/VideoEditor/install.sh");
    QApplication a(argc, argv);
    MainWindow w(exitCode);
    w.show();
    return a.exec();
}
