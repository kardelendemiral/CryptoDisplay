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

// Constructor for the class.
// Takes a list of names or symbols of requested coins.
CoinHandler::CoinHandler(vector<QString> coins_in) : QObject(nullptr)
{

    manager  = new QNetworkAccessManager();
    // Requesting the list of possbile coins with their names, ids and symbols
    QNetworkRequest coinR(QUrl("https://api.coingecko.com/api/v3/coins/list"));
    QNetworkReply *reply = manager->get(coinR);

    // Sends the reply and coin list to the replyCoins function
    connect(reply, &QNetworkReply::finished, this, [=](){replyCoins(reply, coins_in);});
}

// Destructor deletes the Network Manager
CoinHandler::~CoinHandler()
{
    delete manager;
}


// This function handles the reply for the coin list request
void CoinHandler::replyCoins(QNetworkReply *reply, vector<QString> coins){
    // Interpreting the reply as a Json Document then casting it into a Json Array
    QJsonArray doc(QJsonDocument::fromJson(reply->readAll()).array());

    delete reply; //no longer needed

    // Vectors for the data to be retrieved
    vector<QString> new_coins;
    vector<QString> names;

    // Iterating over the Json to find the corresponding ids for the given cois
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


    // Creating the request url for the coin values
    auto ktr = new_coins.begin();
    QString c = *ktr;

    ktr++;
    for(; ktr != new_coins.end(); ktr++)
    {

        c += "," + (*ktr);
    }

    // Requesting the values from the Api
    QNetworkRequest coinR(QUrl("https://api.coingecko.com/api/v3/simple/price?ids="+c+"&vs_currencies=usd,eur,gbp"));

    // Sending the reply and the list of names to replyConversions
    QNetworkReply *reply2 = manager->get(coinR);
    connect(reply2, &QNetworkReply::finished, this, [=](){replyConversions(reply2,names);});
}

// This is for handling the reply for conversion request
void CoinHandler::replyConversions(QNetworkReply *reply, vector<QString> names)
{
    // Interpreting the Reply as a Json Document then casting it into a Json Object
    QJsonObject doc(QJsonDocument::fromJson(reply->readAll()).object());
    delete reply; // no longer needed

    // Iterating over the object to pair the names and the values of coins
    auto ktr = names.begin();
    for(auto itr = doc.constBegin();itr != doc.constEnd(); itr++)
    {
        vector<double> conv;
        QJsonObject convobj = (*itr).toObject();
        for(auto ktr = convobj.constBegin(); ktr != convobj.constEnd(); ktr++)
        {
            conv.push_back((*ktr).toDouble());
        }
        data[*ktr] = conv;
        ktr++;
    }

    // Creating a copy of the results and emitting a signal containing it
    auto data_copy = new map<QString, vector<double>>(data);
    emit ready(data_copy);
}
