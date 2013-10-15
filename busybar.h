#ifndef BUSYBAR_H
#define BUSYBAR_H

#include <QProgressBar>

class BusyBar : public QProgressBar
{
  Q_OBJECT
public:
  explicit BusyBar(QWidget *parent = 0);
  void updatePosition(const QRect& newGeometry);
};

#endif // BUSYBAR_H
