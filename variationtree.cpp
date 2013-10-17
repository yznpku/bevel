#include "variationtree.h"

#include <QHeaderView>
#include "global.hpp"
#include "queries.hpp"
#include "pixmapbuttondelegate.hpp"
#include "itemdetailswidget.hpp"

VariationTree::VariationTree(QWidget *parent) :
  QTreeWidget(parent)
{
  setMouseTracking(true);
  setColumnCount(3);
  setHeaderItem(new QTreeWidgetItem({tr("Name"), tr("Meta Level"), ""}));
  header()->setStretchLastSection(false);
  header()->setSectionResizeMode(0, QHeaderView::Stretch);
  header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

  PixmapButtonDelegate* delegate = new PixmapButtonDelegate(getIconPixmap("38_16_208"));
  setItemDelegateForColumn(2, delegate);
  connect(delegate, SIGNAL(clicked(QModelIndex)),
          this, SLOT(infoButtonClicked(QModelIndex)));
}

void VariationTree::init(int typeId)
{
  parentTypeId = getParentTypeId(typeId);
  QList<int> variations = getVariations(parentTypeId);\
  QMap<int, QTreeWidgetItem*> itemOfMetaGroup;
  for (int i = 0; i < variations.size(); i++) {
    int typeId = variations[i];
    int metaGroupId;
    if (typeId == parentTypeId)
      metaGroupId = 1; // assume parent type is always Tech I
    else {
      QSqlQuery* metaGroupQuery = Queries::getQuery(Queries::MetaGroupOfTypeQuery);
      metaGroupQuery->bindValue(":id", typeId);
      metaGroupQuery->exec();
      metaGroupQuery->next();
      metaGroupId = metaGroupQuery->value(0).toInt();
    }

    if (!itemOfMetaGroup.contains(metaGroupId)) {
      QSqlQuery* metaGroupNameQuery = Queries::getQuery(Queries::MetaGroupNameQuery);
      metaGroupNameQuery->bindValue(":id", metaGroupId);
      metaGroupNameQuery->exec();
      metaGroupNameQuery->next();
      QString groupName = metaGroupNameQuery->value(0).toString();
      QTreeWidgetItem* item = new QTreeWidgetItem(this, {groupName, ""});
      itemOfMetaGroup[metaGroupId] = item;
    }

    QTreeWidgetItem* groupItem = itemOfMetaGroup[metaGroupId];
    QSqlQuery* typeNameQuery = Queries::getQuery(Queries::TypeNameQuery);
    typeNameQuery->bindValue(":id", typeId);
    typeNameQuery->exec();
    typeNameQuery->next();
    QString typeName = typeNameQuery->value(0).toString();
    int metaLevel = 0;
    QSqlQuery* metaLevelQuery = Queries::getQuery(Queries::MetaLevelQuery);
    metaLevelQuery->bindValue(":id", typeId);
    metaLevelQuery->exec();
    if (metaLevelQuery->next()) {                             // some type doesn't even have meta level attribute
      metaLevel = metaLevelQuery->value(0).toInt();
      if (!metaLevel)
        metaLevel = int(metaLevelQuery->value(1).toDouble()); // meta level may be stored in valueFloat column, don't know why.
    }
    QTreeWidgetItem* item = new QTreeWidgetItem(groupItem, {typeName, QString::number(metaLevel), ""});
    typeOfItem[item] = typeId;
  }
  sortByColumn(1, Qt::AscendingOrder);
  expandAll();
}

bool VariationTree::hasVariations(int typeId)
{
  QSqlQuery hasVariationsQuery(QSqlDatabase::database("static"));
  hasVariationsQuery.prepare("SELECT * "
                             "FROM invMetaTypes "
                             "WHERE typeId = :id "
                             "  OR parentTypeId = :id");
  hasVariationsQuery.bindValue(":id", typeId);
  hasVariationsQuery.exec();
  if (hasVariationsQuery.next())
    return true;
  return false;
}

bool VariationTree::isParent(int typeId)
{
  QSqlQuery isParentQuery(QSqlDatabase::database("static"));
  isParentQuery.prepare("SELECT * "
                        "FROM invMetaTypes "
                        "WHERE parentTypeId = :id");
  isParentQuery.bindValue(":id", typeId);
  isParentQuery.exec();
  if (isParentQuery.next())
    return true;
  return false;
}

int VariationTree::getParentTypeId(int typeId)
{
  if (isParent(typeId))
    return typeId;
  else {
    QSqlQuery parentQuery(QSqlDatabase::database("static"));
    parentQuery.prepare("SELECT parentTypeId "
                        "FROM invMetaTypes "
                        "WHERE typeId = :id");
    parentQuery.bindValue(":id", typeId);
    parentQuery.exec();
    parentQuery.next();
    return parentQuery.value(0).toInt();
  }
}

QList<int> VariationTree::getVariations(int parentTypeId)
{
  QSqlQuery childrenQuery(QSqlDatabase::database("static"));
  childrenQuery.prepare("SELECT typeId "
                        "FROM invMetaTypes "
                        "WHERE parentTypeId = :id");
  childrenQuery.bindValue(":id", parentTypeId);
  childrenQuery.exec();

  QList<int> result;
  result << parentTypeId;
  while (childrenQuery.next())
    result << childrenQuery.value(0).toInt();
  return result;
}

void VariationTree::infoButtonClicked(const QModelIndex& index)
{
  QTreeWidgetItem* item = itemFromIndex(index);
  if (!typeOfItem.contains(item))
    return;
  int typeId = typeOfItem[item];
  ItemDetailsWidget* idw = new ItemDetailsWidget(typeId, mainWindow);
  idw->show();
}
