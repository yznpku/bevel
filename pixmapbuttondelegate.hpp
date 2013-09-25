#ifndef PIXMAPBUTTONDELEGATE_HPP
#define PIXMAPBUTTONDELEGATE_HPP

#include <QStyledItemDelegate>

class PixmapButtonDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit PixmapButtonDelegate(const QPixmap& icon, QObject* parent = 0);
  QPoint closeIconPos(const QStyleOptionViewItem& option) const;
  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const;
  QSize sizeHint(const QStyleOptionViewItem& option,
                 const QModelIndex& index) const;
  bool editorEvent(QEvent* event, QAbstractItemModel* model,
                   const QStyleOptionViewItem& option,
                   const QModelIndex& index);
signals:
  void clicked(const QModelIndex&);
private:
  QPixmap icon;
};

#endif // PIXMAPBUTTONDELEGATE_HPP
