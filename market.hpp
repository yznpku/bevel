#ifndef MARKET_HPP
#define MARKET_HPP

#include <QObject>
#include <QMap>

class QNetworkReply;

class Market : public QObject
{
  Q_OBJECT
public:
  explicit Market(QObject *parent = 0);
  double getSellPrice(int typeId);
  void requestPrice(int typeId);
signals:
  void priceUpdated(int typeId);
private slots:
  void priceReplyFinished();
private:
  QStringList parsePriceReply(const QString& xmlString);
private:
  QMap<int, QNetworkReply*> priceReplyOfType;
  QMap<QNetworkReply*, int> typeOfPriceReply;
};

#endif // MARKET_HPP
