#include "busybar.h"

BusyBar::BusyBar(QWidget *parent) :
  QProgressBar(parent)
{
  setRange(0, 0);
  resize(100, 19);
}

void BusyBar::updatePosition(const QRect& parentGeometry)
{
  QRect newGeometry = rect();
  newGeometry.moveCenter(QPoint(parentGeometry.width() / 2, parentGeometry.height() / 2));
  setGeometry(newGeometry);
}
