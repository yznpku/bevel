#include "typepixmaplabel.hpp"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include "typevariant.hpp"
#include "global.hpp"

TypePixmapLabel::TypePixmapLabel(QWidget *parent) :
  QLabel(parent)
{
  setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  setFixedSize(80, 80);
  setAlignment(Qt::AlignmentFlag::AlignCenter);
  setAcceptDrops(true);
}

void TypePixmapLabel::dragEnterEvent(QDragEnterEvent* e)
{
  if (e->mimeData()->hasFormat("eveop/type-variant"))
    e->acceptProposedAction();
}

void TypePixmapLabel::dragMoveEvent(QDragMoveEvent* e)
{
  e->acceptProposedAction();
}

void TypePixmapLabel::dropEvent(QDropEvent* e)
{
  if (e->mimeData()->hasFormat("eveop/type-variant")) {
    int typeId = TypeVariant::fromMimeData(e->mimeData())[0].toTypeId();
    setText(QString());
    setPixmap(*getTypePixmap64(typeId));
    emit typeDropped(typeId);
  }
}
