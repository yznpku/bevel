#include "marketorderstypepixmaplabel.hpp"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include "typevariant.hpp"
#include "global.hpp"

MarketOrdersTypePixmapLabel::MarketOrdersTypePixmapLabel(QWidget *parent) :
  QLabel(parent)
{
  setAcceptDrops(true);
}

void MarketOrdersTypePixmapLabel::dragEnterEvent(QDragEnterEvent* e)
{
  if (e->mimeData()->hasFormat("eveop/type-variant"))
    e->acceptProposedAction();
}

void MarketOrdersTypePixmapLabel::dragMoveEvent(QDragMoveEvent* e)
{
  e->acceptProposedAction();
}

void MarketOrdersTypePixmapLabel::dropEvent(QDropEvent* e)
{
  if (e->mimeData()->hasFormat("eveop/type-variant")) {
    int typeId = TypeVariant::fromMimeData(e->mimeData())[0].toTypeId();
    setText(QString());
    setPixmap(*getTypePixmap64(typeId));
    emit typeDropped(typeId);
  }
}
