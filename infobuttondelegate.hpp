#ifndef INFOBUTTONDELEGATE_HPP
#define INFOBUTTONDELEGATE_HPP

#include <QStyledItemDelegate>

class InfoButtonDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit InfoButtonDelegate(const QPixmap& icon, QObject* parent = 0);
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

#endif // INFOBUTTONDELEGATE_HPP
