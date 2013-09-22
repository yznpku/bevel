#ifndef MARKETORDERSTYPEPIXMAPLABEL_HPP
#define MARKETORDERSTYPEPIXMAPLABEL_HPP

#include <QLabel>

class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;

class MarketOrdersTypePixmapLabel : public QLabel
{
  Q_OBJECT
public:
  explicit MarketOrdersTypePixmapLabel(QWidget *parent = 0);
protected:
  virtual void dragEnterEvent(QDragEnterEvent* e);
  virtual void dragMoveEvent(QDragMoveEvent* e);
  virtual void dropEvent(QDropEvent* e);
signals:
  void typeDropped(int typeId);
};

#endif // MARKETORDERSTYPEPIXMAPLABEL_HPP
