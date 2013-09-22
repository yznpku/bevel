#ifndef _INFO_BUTTON_HPP_
#define _INFO_BUTTON_HPP_

#include <QLabel>
#include "attribute_set.hpp"

class QMouseEvent;
class QPixmap;

class InfoButton : public QLabel {
public:
  InfoButton(QWidget* parent = 0);
  void init(int prototypeId);
  void init(const AttributeSet& as);
protected:
  virtual void mousePressEvent(QMouseEvent* e);
private:
  void init();
private:
  bool prototyped;
  int prototypeId;
  AttributeSet as;
  static QPixmap* icon;
};

#endif
