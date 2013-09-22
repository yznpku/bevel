#include "infobutton.hpp"

#include <QMouseEvent>
#include <QPixmap>
#include <QApplication>
#include "global.hpp"
#include "itemdetailswidget.hpp"

QPixmap* InfoButton::icon;

InfoButton::InfoButton(QWidget* parent)
  : QLabel(parent)
{

}

void InfoButton::init(int prototypeId)
{
  prototyped = true;
  this->prototypeId = prototypeId;
  init();
}

void InfoButton::init(const AttributeSet& as)
{
  prototyped = false;
  this->as = as;
  init();
}

void InfoButton::init() {
  if (!icon)
    icon = new QPixmap(qApp->applicationDirPath() + "/Icons/items/38_16_208.png");
  setPixmap(*icon);
}

void InfoButton::mousePressEvent(QMouseEvent* e) {
  Q_UNUSED(e);
  ItemDetailsWidget* idw;
  if(prototyped)
    idw = new ItemDetailsWidget(prototypeId, mainWindow);
  else
    idw = new ItemDetailsWidget(as, mainWindow);
  idw->show();
}


