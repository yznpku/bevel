#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <QNetworkReply>
#include <QListIterator>

namespace Network {
  extern QNetworkAccessManager* priceNetwork;
  extern QNetworkAccessManager* ordersNetwork;

  void initNetwork();
  QNetworkReply* getOrders(int typeId, int timeLimit, int regionLimit = -1, int systemLimit = -1);
  QNetworkReply* getPrice(int typeId, int timeLimit, int regionLimit = -1, int systemLimit = -1);
}

#endif // NETWORK_HPP
