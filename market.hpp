#ifndef MARKET_HPP
#define MARKET_HPP

#include <QObject>

class Market : public QObject
{
  Q_OBJECT
public:
  explicit Market(QObject *parent = 0);
  double getPrice(int typeId);
  void requestPrice(int typeId);
signals:
  void priceUpdated(int typeId);
public slots:

};

#endif // MARKET_HPP
