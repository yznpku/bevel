#ifndef _ATTRIBUTE_SET_HPP_
#define _ATTRIBUTE_SET_HPP_

#include <QDataStream>
#include <QMap>
#include <QString>

class AttributeSet {
public:
  AttributeSet(QString name = QString());
  static AttributeSet fromPrototype(int prototypeId);
public:
  QString name;
  QString description;
  int prototypeId;
  QMap<int, double> attr;
};

QDataStream& operator<<(QDataStream& stream, const AttributeSet& as);
QDataStream& operator>>(QDataStream& stream, AttributeSet& as);

#endif
