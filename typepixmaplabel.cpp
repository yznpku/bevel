#include "typepixmaplabel.hpp"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include "typevariant.hpp"
#include "global.hpp"

TypePixmapLabel::TypePixmapLabel(QWidget* parent)
  : QLabel(parent)
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
    int filteredTypeId = filterTypeId(typeId);
    if (filteredTypeId == -1)
      return;
    setText(QString());
    setPixmap(*getTypePixmap64(filteredTypeId));
    emit typeDropped(filteredTypeId);
  }
}

// This method receives a param 'typeId' indicating the type dropped here.
// We can transform the Id and return it in the subclass, or we can just return it as it is.
// Return -1 if the typeId is not to be accepted.
int TypePixmapLabel::filterTypeId(int typeId) const
{
  return typeId;
}
