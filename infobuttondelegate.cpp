#include "infobuttondelegate.hpp"

#include <QPainter>
#include <QMouseEvent>

InfoButtonDelegate::InfoButtonDelegate(const QPixmap& icon, QObject *parent)
  : QStyledItemDelegate(parent)
{
  this->icon = icon;
}

QPoint InfoButtonDelegate::closeIconPos(const QStyleOptionViewItem& option) const
{
  return QPoint(option.rect.right() - icon.width() - 2,
                option.rect.center().y() - icon.height() / 2);
}

void InfoButtonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QStyledItemDelegate::paint(painter, option, index);
  if (!index.model()->hasChildren(index) && (option.state & QStyle::State_MouseOver))
    painter->drawPixmap(closeIconPos(option), icon);
}

QSize InfoButtonDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QSize size = QStyledItemDelegate::sizeHint(option, index);
  if (!index.parent().isValid()) {
    size.rwidth() += icon.width() + 4;
    size.setHeight(qMax(size.height(),
                        icon.height() + 4));
  }
  return size;
}

bool InfoButtonDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
  if(!index.model()->hasChildren(index) &&
     event->type() == QEvent::MouseButtonRelease) {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    QRect buttonRect = icon.rect().translated(closeIconPos(option));
    if(buttonRect.contains(mouseEvent->pos()))
      emit clicked(index);
  }
  return false;
}
