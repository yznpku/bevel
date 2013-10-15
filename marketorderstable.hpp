#ifndef MARKETORDERSTABLE_HPP
#define MARKETORDERSTABLE_HPP

#include <QTableWidget>

class QEvent;
class QResizeEvent;
class BusyBar;

class MarketOrdersTable : public QTableWidget
{
  Q_OBJECT
public:
  explicit MarketOrdersTable(QWidget *parent = 0);
  void setBusy(bool busy);
protected:
  virtual bool viewportEvent(QEvent* e);
  virtual void resizeEvent(QResizeEvent* e);
private:
  BusyBar* busyBar;
};

#endif // MARKETORDERSTABLE_HPP
