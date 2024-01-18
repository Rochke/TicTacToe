#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("TicTacToe");
    w.setWindowIcon(QIcon(":/Resources/favicon.ico"));
    return a.exec();
}
