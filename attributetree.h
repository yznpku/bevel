#ifndef ATTRIBUTETREE_H
#define ATTRIBUTETREE_H

#include <QTreeWidget>
#include <QMap>
#include "attribute_set.hpp"

class AttributeTree : public QTreeWidget
{
  Q_OBJECT
public:
  AttributeTree(QWidget* parent = 0);
  void init(const AttributeSet& as);
private:
  AttributeSet as;
  QMap<int, QTreeWidgetItem*> itemOfCategory;
  QString valueToString(int attributeId, int unitId);
};

#endif // ATTRIBUTETREE_H
