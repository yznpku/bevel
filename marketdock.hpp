#ifndef MARKETDOCK_HPP
#define MARKETDOCK_HPP

#include <QDockWidget>

class MarketOrdersWidget;

class MarketDock : public QDockWidget
{
  Q_OBJECT
public:
  explicit MarketDock(QWidget *parent = 0);  
private:
  MarketOrdersWidget* mow;
};

#endif // MARKETDOCK_HPP
