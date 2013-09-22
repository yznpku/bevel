#include "attribute_set.hpp"

#include <QSqlQuery>
#include <QVariant>

AttributeSet::AttributeSet(QString name) {
  this->name = name;
  this->prototypeId = -1;
}

AttributeSet AttributeSet::fromPrototype(int prototypeId) {
  AttributeSet as;
  as.prototypeId = prototypeId;
  QSqlQuery nameQuery(QSqlDatabase::database("static"));
  nameQuery.exec(QString("select TYPENAME, DESCRIPTION "
                         "from invTypes "
                         "where TYPEID = %1").
                 arg(QString::number(prototypeId)));
  nameQuery.next();
  as.name = nameQuery.value(0).toString();
  as.description = nameQuery.value(1).toString();
  QSqlQuery attrQuery(QSqlDatabase::database("static"));
  attrQuery.exec(QString("select ATTRIBUTEID, VALUEINT, VALUEFLOAT "
                         "from dgmTypeAttributes "
                         "where TYPEID = %1")
                 .arg(QString::number(prototypeId)));
  while (attrQuery.next()) {    
    int attributeId = attrQuery.value(0).toInt();
    int valueInt = attrQuery.value(1).toInt();
    double valueFloat = attrQuery.value(2).toDouble();
    double value = valueInt ? (double)valueInt : valueFloat;
    as.attr[attributeId] = value;
  }
  return as;
}


QDataStream& operator<<(QDataStream& stream, const AttributeSet& as) {
  stream << as.name << as.description << as.prototypeId << as.attr;
  return stream;
}


QDataStream& operator>>(QDataStream& stream, AttributeSet& as) {
  stream >> as.name >> as.description >> as.prototypeId >> as.attr;
  return stream;
}
