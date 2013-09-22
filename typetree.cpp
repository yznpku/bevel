#include "typetree.hpp"

#include <QAction>
#include <QContextMenuEvent>
#include <QDataStream>
#include <QHeaderView>
#include <QLabel>
#include <QListIterator>
#include <QMenu>
#include <QPushButton>
#include <QtSql>
#include <QTreeWidgetItem>
#include <QQueue>
#include "global.hpp"
#include "queries.hpp"
#include "infobutton.hpp"
#include "infobuttondelegate.hpp"
#include "itemdetailswidget.hpp"
#include "typevariant.hpp"
#include <QDrag>

TypeTree::TypeTree(QWidget* parent)
  : QTreeWidget(parent) {
  setColumnCount(2);
  setHeaderItem(new QTreeWidgetItem(QStringList({tr("Name"), ""})));
  header()->setStretchLastSection(false);
  header()->setSectionResizeMode(0, QHeaderView::Stretch);
  header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  this->setUniformRowHeights(true);
  setDragEnabled(true);
}

void TypeTree::initWithDefaultGroups()
{
  QMap<int, QTreeWidgetItem*> itemOfCategory;
  QMap<int, QTreeWidgetItem*> itemOfGroup;
  QSqlQuery categoryList(QSqlDatabase::database("static"));
  categoryList.exec("select CATEGORYID, CATEGORYNAME "
                    "from invCategories "
                    "where PUBLISHED = 1 "
                    "order by CATEGORYNAME");
  while (categoryList.next()) {
    int categoryId = categoryList.value(0).toInt();
    QString categoryName = categoryList.value(1).toString();
    QTreeWidgetItem* item = new QTreeWidgetItem(this, QStringList(categoryName));
    itemOfCategory[categoryId] = item;
  }

  QSqlQuery groupList(QSqlDatabase::database("static"));
  groupList.exec("select GROUPID, CATEGORYID, GROUPNAME "
                 "from invGroups "
                 "where PUBLISHED = 1 "
                 "order by GROUPNAME");
  while (groupList.next()) {
    int groupId = groupList.value(0).toInt();
    int categoryId = groupList.value(1).toInt();
    QString groupName = groupList.value(2).toString();
    if (itemOfCategory.contains(categoryId)) {
      QTreeWidgetItem* item = new QTreeWidgetItem(itemOfCategory[categoryId],
                                                  QStringList(groupName));
      itemOfGroup[groupId] = item;
    }
  }

  QSqlQuery typeList(QSqlDatabase::database("static"));
  typeList.exec("select TYPEID, GROUPID, TYPENAME "
                "from invTypes "
                "where PUBLISHED = 1 "
                "order by TYPENAME");
  while (typeList.next()) {
    int typeId = typeList.value(0).toInt();
    int groupId = typeList.value(1).toInt();
    QString typeName = typeList.value(2).toString();
    if (itemOfGroup.contains(groupId)) {
      QTreeWidgetItem* item = new QTreeWidgetItem(itemOfGroup[groupId],
                                                  QStringList(typeName));
      InfoButton* infoButton = new InfoButton();
      infoButton->init(typeId);
      setItemWidget(item, 1, infoButton);
      typeOfItem[item] = typeId;
      itemOfType[typeId] = item;
    }
  }
}

void TypeTree::initWithMarketGroups()
{
  InfoButtonDelegate* delegate = new InfoButtonDelegate(getIconPixmap("38_16_208"));
  this->setItemDelegateForColumn(1, delegate);
  connect(delegate, SIGNAL(clicked(QModelIndex)),
          this, SLOT(infoButtonClicked(QModelIndex)));
  QMap<int, QTreeWidgetItem*> itemOfMarketGroup;
  QSqlQuery rootGroupQuery(QSqlDatabase::database("static"));
  rootGroupQuery.exec("SELECT marketGroupId "
                      "FROM invMarketGroups "
                      "WHERE parentGroupId IS NULL");
  QQueue<int> q;
  while (rootGroupQuery.next()) {
    int groupId = rootGroupQuery.value(0).toInt();
    QSqlQuery* groupNameQuery = Queries::getMarketGroupNameQuery();
    groupNameQuery->bindValue(":id", groupId);
    groupNameQuery->exec();
    groupNameQuery->next();
    QString groupName = groupNameQuery->value(0).toString();
    QTreeWidgetItem* item = new QTreeWidgetItem(this, QStringList(groupName));
    itemOfMarketGroup[groupId] = item;
    q.enqueue(groupId);
  }

  QSqlQuery subgroupsQuery(QSqlDatabase::database("static"));
  subgroupsQuery.prepare("SELECT marketGroupId "
                         "FROM invMarketGroups "
                         "WHERE parentGroupId = :id");
  QSqlQuery typesOfGroupQuery(QSqlDatabase::database("static"));
  typesOfGroupQuery.prepare("SELECT typeId, typeName "
                            "FROM invTypes "
                            "WHERE marketGroupId = :id "
                            "  AND published = 1"
                            "  AND groupId IS NOT NULL");
  while (!q.isEmpty()) {
    int marketGroupId = q.dequeue();
    QTreeWidgetItem* parentItem = itemOfMarketGroup[marketGroupId];
    subgroupsQuery.bindValue(":id", marketGroupId);
    subgroupsQuery.exec();
    while (subgroupsQuery.next()) {
      int subgroupId = subgroupsQuery.value(0).toInt();
      QSqlQuery* subgroupNameQuery = Queries::getMarketGroupNameQuery();
      subgroupNameQuery->bindValue(":id", subgroupId);
      subgroupNameQuery->exec();
      subgroupNameQuery->next();
      QString subgroupName = subgroupNameQuery->value(0).toString();
      QTreeWidgetItem* item = new QTreeWidgetItem(parentItem, QStringList(subgroupName));
      itemOfMarketGroup[subgroupId] = item;
      q.enqueue(subgroupId);
    }
    typesOfGroupQuery.bindValue(":id", marketGroupId);
    typesOfGroupQuery.exec();
    while (typesOfGroupQuery.next()) {
      int typeId = typesOfGroupQuery.value(0).toInt();
      QString typeName = typesOfGroupQuery.value(1).toString();
      QTreeWidgetItem* item = new QTreeWidgetItem(parentItem, QStringList(typeName));
      typeOfItem[item] = typeId;
      itemOfType[typeId] = item;
    }
  }
}

void TypeTree::contextMenuEvent(QContextMenuEvent* e) {
  QTreeWidgetItem* item = itemAt(e->pos());
  if (typeOfItem.contains(item)) {
    QMenu* menu = new QMenu(this);
    QAction* action = menu->addAction(tr("View Details"));
    connect(action, SIGNAL(triggered(bool)),
            this, SLOT(typeViewDetailsClicked()));
    menu->exec(viewport()->mapToGlobal(e->pos()));
  }
}

QStringList TypeTree::mimeTypes() const {
  QStringList types;
  types << "eveop/type-variant"
        << "text/plain";
  return types;
}

QMimeData* TypeTree::mimeData(const QList<QTreeWidgetItem*> items) const {
  QList<TypeVariant> typeVariants;
  for (QListIterator<QTreeWidgetItem*> i(items); i.hasNext();) {
    QTreeWidgetItem* item = i.next();
    if (typeOfItem.contains(item))
      typeVariants << typeOfItem[item];
  }
  if (typeVariants.isEmpty())
    return 0;
  return listOfTypeVariantToMimeData(typeVariants);
}

void TypeTree::startDrag(Qt::DropActions supportedActions) {
  // TODO: list support
  if (selectedItems().isEmpty() || !typeOfItem.contains(selectedItems()[0])) {
    QAbstractItemView::startDrag(supportedActions);
    return;
  }
  QDrag* drag = new QDrag(this);
  drag->setMimeData(mimeData(selectedItems()));
  drag->setPixmap(*getTypePixmap64(typeOfItem[selectedItems()[0]]));
  drag->exec(supportedActions, Qt::CopyAction);
}

void TypeTree::typeViewDetailsClicked() {
  if (!selectedItems().isEmpty()) {
    QTreeWidgetItem* item = selectedItems()[0];
    if (typeOfItem.contains(item)) {
      ItemDetailsWidget* idw = new ItemDetailsWidget(typeOfItem[item],
                                                     mainWindow);
      idw->show();
    }
  }
}

void TypeTree::infoButtonClicked(const QModelIndex& index)
{
  QTreeWidgetItem* item = itemFromIndex(index);
  if (!typeOfItem.contains(item))
    return;
  int typeId = typeOfItem[item];
  ItemDetailsWidget* widget = new ItemDetailsWidget(typeId, mainWindow);
  widget->show();
}
