#ifndef MARKETORDERSTABLE_HPP
#define MARKETORDERSTABLE_HPP

#include <QTableWidget>

class QEvent;

class MarketOrdersTable : public QTableWidget
{
  Q_OBJECT
public:
  explicit MarketOrdersTable(QWidget *parent = 0);
protected:
  virtual bool viewportEvent(QEvent* e);
};

#endif // MARKETORDERSTABLE_HPP
