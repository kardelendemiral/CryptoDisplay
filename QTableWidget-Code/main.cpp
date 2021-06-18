#include "mainwindow.h"
#include "coinhandler.h"
#include <QApplication>
#include <map>
#include <utility>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w; //create the table and show it
    w.show();
    QApplication::quit();

    return a.exec();
}

