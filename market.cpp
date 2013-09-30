#include "market.hpp"

#include <QThread>
#include <QSetIterator>
#include <QtXmlPatterns>
#include "queries.hpp"
#include "network.hpp"
#include "settings.hpp"
#include "requestpriceevent.hpp"

Market* market;

Market::Market(QObject *parent) :
  QObject(parent)
{
}

void Market::initMarket()
{
  market = new Market();
  QThread* marketThread = new QThread();
  marketThread->start();
  market->moveToThread(marketThread);
  Network::marketNetwork->moveToThread(marketThread);
}

double Market::getSellPrice(int typeId)
{
  databaseLock.lockForRead();

  QSqlQuery* marketPriceQuery = Queries::getMarketPriceQuery();
  marketPriceQuery->bindValue(":id", typeId);
  marketPriceQuery->exec();

  if (!marketPriceQuery->next()) {
    databaseLock.unlock();
    return qQNaN();
  }

  double price = marketPriceQuery->value(0).toDouble();

  databaseLock.unlock();
  return price;
}

void Market::requestPrice(int typeId)
{
  requestBufferLock.lock();
  requestBuffer.insert(typeId);
  RequestPriceEvent* event = new RequestPriceEvent();
  QCoreApplication::postEvent(this, event);
  requestBufferLock.unlock();
}

bool Market::event(QEvent* e)
{
  if (e->type() == RequestPriceEvent::RequestPriceEventType) {
    QCoreApplication::removePostedEvents(this, RequestPriceEvent::RequestPriceEventType);
    processAllRequests();
    return true;
  }
  return false;
}

void Market::sendRequest(int typeId)
{
  if (priceReplyOfType.contains(typeId)) { // abandon the ongoing request for the type
    QNetworkReply* reply = priceReplyOfType[typeId];
    typeOfPriceReply.remove(reply);
    priceReplyOfType.remove(typeId);
    reply->deleteLater();
  }

  QNetworkReply* reply = Network::getPrice(typeId, Settings::getMarketOrdersTimeLimitSetting(),
                                           -1, 30000142);
  typeOfPriceReply[reply] = typeId;
  priceReplyOfType[typeId] = reply;
  connect(reply, SIGNAL(finished()),
          this, SLOT(priceReplyFinished()));
}

void Market::priceReplyFinished()
{
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
  int typeId = typeOfPriceReply[reply];
  QString xmlString = reply->readAll();
  typeOfPriceReply.remove(reply);
  priceReplyOfType.remove(typeId);
  reply->deleteLater();

  QStringList splitted = parsePriceReply(xmlString);
  double sellPrice = splitted[0].toDouble();
  double buyPrice = splitted[1].toDouble();
  double averagePrice = splitted[2].toDouble();
  qlonglong updateTime = QDateTime::currentMSecsSinceEpoch();
  // TODO: set price to NaN if equal to 0.00

  databaseLock.lockForWrite();

  QSqlQuery* updateQuery = Queries::getUpdateMarketPriceQuery();
  updateQuery->bindValue(":id", typeId);
  updateQuery->bindValue(":sellPrice", sellPrice);
  updateQuery->bindValue(":buyPrice", buyPrice);
  updateQuery->bindValue(":averagePrice", averagePrice);
  updateQuery->bindValue(":updateTime", updateTime);
  updateQuery->exec();

  if (updateQuery->numRowsAffected() < 1) {
    QSqlQuery* insertQuery = Queries::getInsertMarketPriceQuery();
    insertQuery->bindValue(":id", typeId);
    insertQuery->bindValue(":sellPrice", sellPrice);
    insertQuery->bindValue(":buyPrice", buyPrice);
    insertQuery->bindValue(":averagePrice", averagePrice);
    insertQuery->bindValue(":updateTime", updateTime);
    insertQuery->exec();
  }

  databaseLock.unlock();
  emit priceUpdated(typeId);
}

void Market::processAllRequests()
{
  requestBufferLock.lock();
  for (QSetIterator<int> i(requestBuffer); i.hasNext();) {
    int typeId = i.next();
    sendRequest(typeId);
  }
  requestBuffer.clear();
  requestBufferLock.unlock();
}

QStringList Market::parsePriceReply(const QString& xmlString)
{
  QXmlQuery query;
  query.setFocus(xmlString);
  query.setQuery("fn:concat(//sell/min/text(), \"/\", "
                 "          //buy/max/text(), \"/\", "
                 "          //all/avg/text()"
                 ")");
  QStringList strList;
  query.evaluateTo(&strList);
  QStringList splitted = strList[0].split("/");
  return splitted;
}
