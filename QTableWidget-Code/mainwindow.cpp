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
int rc=0; //row count of the table
vector<QString> q; //a vector to keep the names or symbols of the coins to be displayed in the table

MainWindow::MainWindow(QWidget *parent) : //a subclass of QWidget since we'll use its tools but also need additional functionality
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QString fileName = qgetenv("MYCRYPTOCONVERT"); 
    QFile inputFile(fileName); //open the file where the coin names (or symbols) to be displayed are written
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          q.push_back(line); //put the coin names (or symbols) to a vector
          rc++;
       }
       inputFile.close();
    }
        
    CoinHandler* handler = new CoinHandler(q); // a coin handler is created to retrieve the data
    connect(handler, SIGNAL(ready(std::map<QString,std::vector<double> >*)), this, SLOT(dataReady(std::map<QString,std::vector<double> >*))); //when the coin handler finishes its job, "dataReady" function is called with the map that keeps the coin currency data

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataReady(map<QString, vector<double> > *data) //this function just sends the data to the function which will create the table
{
    TableWidgetDisplay(data);
}

void MainWindow::TableWidgetDisplay(map<QString, vector<double> > *data)
{

    QTableWidget *table = new QTableWidget(this); //the main table to be displayed 
    table->setRowCount(rc); //row count is calculated when reading the input file in the constructor. he tables has to have this many rows
    table->setColumnCount(3); //the table has a constant number of columns which are for USD, EUR and GBP

    this->setCentralWidget(table); //this widget has its central widget as this table

    QStringList hlabels;
    QStringList vlabels;

    
    hlabels << "USD" << "EUR" << "GBP"; //horizontal labels
    table->setHorizontalHeaderLabels(hlabels); //horizontal labels are set

    map<QString, vector<double>>::iterator it; // an iterator to iterate the map and put the data in the table
    int row = 0;
    for(it = (*data).begin(); it != (*data).end(); it++){ // main loop to constuct the table

        QString coinName = it->first; //the map has its keys as the names of the coins
        vlabels << coinName; // the coin name is added to the vertical labels 

        vector<double> vec = it->second; //this vector keeps 3 double numbers which are the value of the current coin with respect to usd, eur and gbp

        for(int i=0; i<vec.size() ;i++){ // iterate the vector

            QTableWidgetItem *item = new QTableWidgetItem; //create an item to put in a certain location in the table

            int j;

            if(i==0) //this part is just to put these values to the corresponding row in a desired order. the order coming from the coin handler is not suitable for the horizontal labels
                j=2;
            if(i==1)
                j=0;
            if(i==2)
                j=1;
            double val = vec[j]; // the value to put in the current part of the table

            item->setText(QString::number(val)); // set the text of the item object as the value
            table->setItem(row,i,item); //put the item to the table's appropriate location

        }

        row++;
    }

     delete data; 
     table->setVerticalHeaderLabels(vlabels); //lastly, insert the vertical labels to the table




}
