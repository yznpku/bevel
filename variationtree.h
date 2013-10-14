#ifndef VARIATIONTREE_H
#define VARIATIONTREE_H

#include <QTreeWidget>

#include <QMap>
#include <QList>

class VariationTree : public QTreeWidget
{
  Q_OBJECT
public:
  VariationTree(QWidget *parent = 0);
  void init(int typeId);
public:
  static bool hasVariations(int typeId);
  static bool isParent(int typeId);
  static int getParentTypeId(int typeId);
  static QList<int> getVariations(int parentTypeId);
private:
  int parentTypeId;
  QMap<QTreeWidgetItem*, int> typeOfItem;
};

#endif // VARIATIONTREE_H
