#ifndef COINHANDLER_H
#define COINHANDLER_H



#include <vector>
#include <QObject>
#include <utility>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>

class CoinHandler : public QObject
{
    Q_OBJECT

    QNetworkAccessManager* manager;
    QJsonDocument coversions;
    std::vector<QString> coins;
    std::map<QString, std::vector<double>> data;
public slots:

    void replyCoins(QNetworkReply *reply, std::vector<QString> coins);
    void replyConversions(QNetworkReply *reply, std::vector<QString> names);

signals:
    void ready(std::map<QString, std::vector<double>>* data);

public:
    explicit CoinHandler(std::vector<QString> coins_in);
    ~CoinHandler();

};

#endif // COINHANDLER_H
