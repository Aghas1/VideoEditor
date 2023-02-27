#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QProcess* proc = new QProcess();
    std::string exePath = argv[0];
    int index = exePath.find_last_of('/');
    exePath.erase(index);
    exePath += "/install.sh";
    int exitCode = proc->execute(exePath.c_str());
    QApplication a(argc, argv);
    MainWindow w(exitCode);
    w.show();
    return a.exec();
}
