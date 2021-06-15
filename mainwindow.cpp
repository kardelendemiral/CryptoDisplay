#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <vector>
#include <map>
#include <utility>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TableWidgetDisplay();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TableWidgetDisplay()
{
    map<QString, vector<float>> data;
    data["Bitcoin"] = {3.112,5.112,9.18}; //datayı cekip koycaz, 0-> usd, 1-> eur, 2-> gbp
    int rc = 5; //kac tane coin varsa bu onun sayısı
    QTableWidget *table = new QTableWidget(this);
    table->setRowCount(rc);
    table->setColumnCount(3);

    this->setCentralWidget(table);

    QStringList hlabels;
    hlabels << "USD" << "EUR" << "GBP";
    table->setHorizontalHeaderLabels(hlabels);

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
