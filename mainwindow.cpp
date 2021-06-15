#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <vector>
#include <map>
#include <fstream>
#include <QFile>
#include <utility>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QProcessEnvironment>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TableWidgetDisplay();

    // QNetworkAccessManager *manager = new QNetworkAccessManager(this) ;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TableWidgetDisplay()
{
    int rc = 0; //coin sayısı
    QString fileName = qgetenv("MYCRYPTOCONVERT"); //burda file actım yazılcak coinlerin namelerini vektore koydum
    vector<QString> coins;
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          coins.push_back(line);
          rc++;
       }
       inputFile.close();
    }

    map<QString, vector<float>> data;
    data["Bitcoin"] = {32988,27092,23299}; //datayı cekip koycaz, 0-> usd, 1-> eur, 2-> gbp
    data["Ethereum"] = {2300.58, 2000.09,1900.22};

    QTableWidget *table = new QTableWidget(this);
    table->setRowCount(rc);
    table->setColumnCount(3);

    this->setCentralWidget(table);

    QStringList hlabels;
    QStringList vlabels;

    for(int i=0;i< coins.size();i++){
        vlabels << coins[i];
    }
    hlabels << "USD" << "EUR" << "GBP";
    table->setHorizontalHeaderLabels(hlabels);
    table->setVerticalHeaderLabels(vlabels);

    map<QString, vector<float>>::iterator it;
    int row = 0;
    for(it = data.begin(); it != data.end(); it++){

        QTableWidgetItem *item = new QTableWidgetItem;

        QString coinName = it->first;

        vector<float> vec = it->second;

        for(int i=0; i<vec.size() ;i++){

            QTableWidgetItem *item = new QTableWidgetItem;
            float val = vec[i];

            item->setText(QString::number(val));
            table->setItem(row,i,item);

        }

        row++;
    }




}
