#include "marketorderstable.hpp"

#include "busybar.h"

MarketOrdersTable::MarketOrdersTable(QWidget *parent) :
  QTableWidget(parent)
{
  busyBar = new BusyBar(this);
  busyBar->setHidden(true);
}

void MarketOrdersTable::setBusy(bool busy)
{
  busyBar->setHidden(!busy);
}

bool MarketOrdersTable::viewportEvent(QEvent* e)
{
  resizeColumnToContents(1);
  resizeColumnToContents(2);
  resizeColumnToContents(3);
  return QTableWidget::viewportEvent(e);
}

void MarketOrdersTable::resizeEvent(QResizeEvent* e)
{
  busyBar->updatePosition(geometry());
  QTableWidget::resizeEvent(e);
}
