#include "network.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>

QNetworkAccessManager* Network::marketNetwork = 0;

void Network::initNetwork()
{
  if (!marketNetwork)
    marketNetwork = new QNetworkAccessManager();
  marketNetwork->get(QNetworkRequest(QUrl("")));
}


QNetworkReply* Network::getOrders(int typeId, int timeLimit,
                                  int regionLimit, int systemLimit)
{
  QUrlQuery query;
  query.addQueryItem("typeid", QString::number(typeId));
  query.addQueryItem("sethours", QString::number(timeLimit));
  if (systemLimit != -1)
    query.addQueryItem("usesystem", QString::number(systemLimit));
  else if (regionLimit != -1)
    query.addQueryItem("regionlimit", QString::number(regionLimit));

  QUrl url("http://www.ceve-market.org/api/quicklook");
  url.setQuery(query);
  QNetworkRequest request(url);
  return marketNetwork->get(request);
}


QNetworkReply* Network::getPrice(int typeId, int timeLimit,
                                int regionLimit, int systemLimit)
{
  QUrlQuery query;
  query.addQueryItem("typeid", QString::number(typeId));
  query.addQueryItem("sethours", QString::number(timeLimit));
  if (systemLimit != -1)
    query.addQueryItem("usesystem", QString::number(systemLimit));
  else if (regionLimit != -1)
    query.addQueryItem("regionlimit", QString::number(regionLimit));

  QUrl url("http://www.ceve-market.org/api/marketstat");
  url.setQuery(query);
  QNetworkRequest request(url);
  return marketNetwork->get(request);
}
