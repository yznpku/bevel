#include "marketorderstable.hpp"

MarketOrdersTable::MarketOrdersTable(QWidget *parent) :
  QTableWidget(parent)
{
}

bool MarketOrdersTable::viewportEvent(QEvent* e)
{
  resizeColumnToContents(1);
  resizeColumnToContents(2);
  resizeColumnToContents(3);
  return QTableWidget::viewportEvent(e);
}
