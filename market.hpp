#ifndef MARKET_HPP
#define MARKET_HPP

#include <QObject>
#include <QMap>
#include <QReadWriteLock>
#include <QMutex>
#include <QSet>

class QNetworkReply;

class Market : public QObject
{
  Q_OBJECT
public:
  explicit Market(QObject *parent = 0);
  static void initMarket();
  double getSellPrice(int typeId);
  void requestPrice(int typeId);
  virtual bool event(QEvent* e);
signals:
  void priceUpdated(int typeId);
private slots:
  void priceReplyFinished();
private:
  void processAllRequests();
  void sendRequest(int typeId);
  QStringList parsePriceReply(const QString& xmlString);
private:
  QMap<int, QNetworkReply*> priceReplyOfType;
  QMap<QNetworkReply*, int> typeOfPriceReply;
  QSet<int> requestBuffer;
  QReadWriteLock databaseLock;
  QMutex requestBufferLock;
};

#endif // MARKET_HPP
