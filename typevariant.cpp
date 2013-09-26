#include "typevariant.hpp"

#include <QMimeData>

TypeVariant::TypeVariant() {
  // Empty and invalid object
}

QList<TypeVariant> TypeVariant::fromMimeData(const QMimeData* mimeData) {
  QList<TypeVariant> tvl;
  QByteArray data = mimeData->data("bevel/type-variant");
  QDataStream stream(data);
  stream >> tvl;
  return tvl;
}

TypeVariant::TypeVariant(int typeId) {
  this->typeId = typeId;
  this->as = NULL;
}

TypeVariant::TypeVariant(const AttributeSet& as) {
  this->as = new AttributeSet(as);
}

TypeVariant::~TypeVariant() {
  if (as)
    delete as;
}


int TypeVariant::toTypeId() const {
  if (as)
    return as->prototypeId;
  else
    return typeId;
}

AttributeSet TypeVariant::toAttributeSet() const {
  if (as)
    return *as;
  else
    return AttributeSet::fromPrototype(typeId);
}

QDataStream& operator<<(QDataStream& stream, const TypeVariant& tv) {
  if (tv.as)
    stream << true << *tv.as;
  else
    stream << false << tv.typeId;
  return stream;
}

QDataStream& operator>>(QDataStream& stream, TypeVariant& tv) {
  bool custom;
  stream >> custom;
  if (custom) {
    tv.as = new AttributeSet();
    stream >> *tv.as;
  } else {
    stream >> tv.typeId;
    tv.as = NULL;
  }
  return stream;
}
