#include "coinhandler.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <iostream>
#include <QTextStream>
#include <QFile>
#include <QDataStream>


using namespace std;

CoinHandler::CoinHandler(vector<QString> coins_in) : QObject(nullptr)
{
    manager  = new QNetworkAccessManager();

    QNetworkRequest coinR(QUrl("https://api.coingecko.com/api/v3/coins/list"));
    QNetworkReply *reply = manager->get(coinR);
    connect(reply, &QNetworkReply::finished, this, [=](){replyCoins(reply, coins_in);});
}

CoinHandler::~CoinHandler()
{
    delete manager;
}



void CoinHandler::replyCoins(QNetworkReply *reply, vector<QString> coins){
    QJsonArray doc(QJsonDocument::fromJson(reply->readAll()).array());
    delete reply;
    vector<QString> new_coins;
    vector<QString> names;

    for(QJsonArray::const_iterator itr = doc.begin();itr != doc.end(); itr++)
    {
        for(vector<QString>::iterator ktr = coins.begin(); ktr != coins.end(); ktr++)
        {
            QJsonObject obj = (*itr).toObject();
            QString id = obj["id"].toString();
            QString symbol = obj["symbol"].toString();
            QString name = obj["name"].toString();
            if((*ktr) == id or (*ktr) == symbol or (*ktr) == name)
            {
                coins.erase(ktr);
                new_coins.push_back(id);
                names.push_back(name);
                break;
            }
        }
    }
    auto ktr = new_coins.begin();
    QString c = *ktr;

    ktr++;
    for(; ktr != new_coins.end(); ktr++)
    {

        c += "," + (*ktr);
    }

    cout << c.toStdString() << endl;

    QNetworkRequest coinR(QUrl("https://api.coingecko.com/api/v3/simple/price?ids="+c+"&vs_currencies=usd,eur,gbp"));
    QNetworkReply *reply2 = manager->get(coinR);
    connect(reply2, &QNetworkReply::finished, this, [=](){replyConversions(reply2,names);});
}

void CoinHandler::replyConversions(QNetworkReply *reply, vector<QString> names)
{
    QJsonObject doc(QJsonDocument::fromJson(reply->readAll()).object());
    delete reply;

    for(auto itr = doc.constBegin();itr != doc.constEnd(); itr++)
    {
        vector<double> conv;
        QJsonObject convobj = (*itr).toObject();
        for(auto ktr = convobj.constBegin(); ktr != convobj.constEnd(); ktr++)
        {
            conv.push_back((*ktr).toDouble());
        }
        data[names.back()] = conv;
        names.pop_back();
    }


    auto data_copy = new map<QString, vector<double>>(data);
    emit ready(data_copy);
}
