#ifndef TYPEPIXMAPLABEL_HPP
#define TYPEPIXMAPLABEL_HPP

#include <QLabel>

class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;

class TypePixmapLabel : public QLabel
{
  Q_OBJECT
public:
  explicit TypePixmapLabel(QWidget* parent = 0);
protected:
  virtual void dragEnterEvent(QDragEnterEvent* e);
  virtual void dragMoveEvent(QDragMoveEvent* e);
  virtual void dropEvent(QDropEvent* e);
  virtual int filterTypeId(int typeId) const;
signals:
  void typeDropped(int typeId);
};

#endif // TYPEPIXMAPLABEL_HPP
