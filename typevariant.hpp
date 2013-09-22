#ifndef TYPEVARIANT_HPP
#define TYPEVARIANT_HPP

#include <QDataStream>
#include <QList>
#include "attribute_set.hpp"

class QMimeData;

class TypeVariant {
public:
  static QList<TypeVariant> fromMimeData(const QMimeData* mimeData);
  TypeVariant();
  TypeVariant(int typeId);
  TypeVariant(const AttributeSet& as);
  ~TypeVariant();
public:
  int toTypeId() const;
  AttributeSet toAttributeSet() const;
private:
  int typeId;
  AttributeSet* as;
private:
  friend QDataStream& operator<<(QDataStream& stream, const TypeVariant& tv);
  friend QDataStream& operator>>(QDataStream& stream, TypeVariant& tv);
};

QDataStream& operator<<(QDataStream& stream, const TypeVariant& tv);
QDataStream& operator>>(QDataStream& stream, TypeVariant& tv);

#endif // TYPEVARIANT_HPP
