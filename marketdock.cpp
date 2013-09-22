#include "marketdock.hpp"
#include "marketorderswidget.hpp"

MarketDock::MarketDock(QWidget *parent) :
    QDockWidget(tr("Market"), parent)
{
  mow = new MarketOrdersWidget();
  setWidget(mow);
}
