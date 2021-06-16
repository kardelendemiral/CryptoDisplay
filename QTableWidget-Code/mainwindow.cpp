#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <vector>
#include <map>
#include <fstream>
#include <QFile>
#include <utility>
#include <iostream>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QProcessEnvironment>
#include "coinhandler.h"

using namespace std;
int rc=0;
vector<QString> q;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //TableWidgetDisplay();

    QString fileName = qgetenv("MYCRYPTOCONVERT"); //burda file actım yazılcak coinlerin namelerini vektore koydum
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          q.push_back(line);
          rc++;
       }
       inputFile.close();
    }

    //vector<QString> q = {"eth", "btc"};
    CoinHandler* handler = new CoinHandler(q);
    connect(handler, SIGNAL(ready(std::map<QString,std::vector<double> >*)), this, SLOT(dataReady(std::map<QString,std::vector<double> >*)));

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataReady(map<QString, vector<double> > *data)
{
    TableWidgetDisplay(data);
}

void MainWindow::TableWidgetDisplay(map<QString, vector<double> > *data)
{


    //map<QString, vector<float>> data;
   // data["Bitcoin"] = {32988,27092,23299}; //datayı cekip koycaz, 0-> usd, 1-> eur, 2-> gbp
    //data["Ethereum"] = {2300.58, 2000.09,1900.22};

    QTableWidget *table = new QTableWidget(this);
    table->setRowCount(rc);
    table->setColumnCount(3);

    this->setCentralWidget(table);

    QStringList hlabels;
    QStringList vlabels;

    /*for(int i=0;i< q.size();i++){
        vlabels << q[i];
    }*/
    hlabels << "USD" << "EUR" << "GBP";
    table->setHorizontalHeaderLabels(hlabels);

    map<QString, vector<double>>::iterator it;
    int row = 0;
    for(it = (*data).begin(); it != (*data).end(); it++){

        QTableWidgetItem *item = new QTableWidgetItem;

        QString coinName = it->first;
        vlabels << coinName;

        vector<double> vec = it->second;

        for(int i=0; i<vec.size() ;i++){

            QTableWidgetItem *item = new QTableWidgetItem;
            double val = vec[i];

            item->setText(QString::number(val));
            table->setItem(row,i,item);

        }

        row++;
    }

     delete data;
     table->setVerticalHeaderLabels(vlabels);




}
