#include "typetree.hpp"

#include <QAction>
#include <QContextMenuEvent>
#include <QResizeEvent>
#include <QDataStream>
#include <QDrag>
#include <QHeaderView>
#include <QLabel>
#include <QListIterator>
#include <QMenu>
#include <QProgressBar>
#include <QPushButton>
#include <QtSql>
#include <QTreeWidgetItem>
#include <QQueue>
#include <QtConcurrent>
#include "global.hpp"
#include "queries.hpp"
#include "infobutton.hpp"
#include "pixmapbuttondelegate.hpp"
#include "itemdetailswidget.hpp"
#include "typevariant.hpp"
#include "market.hpp"

TypeTree::TypeTree(QWidget* parent)
  : QTreeWidget(parent) {
  locale = QLocale(QLocale::English);
  setDragEnabled(true);
  setMouseTracking(true);

  busyBar = new QProgressBar(this);
  busyBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  busyBar->setRange(0, 0);
  busyBar->resize(100, 19);
  busyBar->show();
}

void TypeTree::setColumns(const QList<TypeTree::Column>& columns)
{
  this->columns = columns;
  setColumnCount(columns.size() + 1);
  header()->setStretchLastSection(false);
  QStringList headers;
  headers << tr("Name");
  header()->setSectionResizeMode(0, QHeaderView::Stretch);
  for (int i = 0; i < columns.size(); i++) {
    switch (columns[i]) {
    case InfoButtonColumn:
    {
      headers << "";
      PixmapButtonDelegate* delegate = new PixmapButtonDelegate(getIconPixmap("38_16_208"));
      this->setItemDelegateForColumn(i + 1, delegate);
      connect(delegate, SIGNAL(clicked(QModelIndex)),
              this, SLOT(infoButtonClicked(QModelIndex)));
      break;
    }
    case PriceColumn:
      headers << tr("Price");
      break;
    case UpdatePriceButtonColumn:
    {
      headers << "";
      PixmapButtonDelegate* delegate = new PixmapButtonDelegate(getIconPixmap("73_16_11"));
      this->setItemDelegateForColumn(i + 1, delegate);
      connect(delegate, SIGNAL(clicked(QModelIndex)),
              this, SLOT(updatePriceButtonClicked(QModelIndex)));
      break;
    }
    }
    header()->setSectionResizeMode(i + 1, QHeaderView::ResizeToContents);
  }
  setHeaderItem(new QTreeWidgetItem(headers));
  if (columns.contains(PriceColumn))
    connect(market, SIGNAL(priceUpdated(int)),
            this, SLOT(priceUpdated(int)));
}

void TypeTree::initWithDefaultGroups()
{
  rootItemsFutureWatcher = new QFutureWatcher<QList<QTreeWidgetItem*> >();
  rootItemsFutureWatcher->setFuture(QtConcurrent::run([&]() {
    QList<QTreeWidgetItem*> rootItems;
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
      QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(categoryName));
      itemOfCategory[categoryId] = item;
      rootItems << item;
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
        typeOfItem[item] = typeId;
        itemOfType[typeId] = item;
      }
    }
    return rootItems;
  }));
  connect(rootItemsFutureWatcher, SIGNAL(resultReadyAt(int)),
          this, SLOT(resultReady()));
}

void TypeTree::initWithMarketGroups()
{
  rootItemsFutureWatcher = new QFutureWatcher<QList<QTreeWidgetItem*> >();
  rootItemsFutureWatcher->setFuture(QtConcurrent::run([&]() {
    QList<QTreeWidgetItem*> rootItems;
    QMap<int, QTreeWidgetItem*> itemOfMarketGroup;
    QSqlQuery rootGroupQuery(QSqlDatabase::database("static"));
    rootGroupQuery.exec("SELECT marketGroupId "
                        "FROM invMarketGroups "
                        "WHERE parentGroupId IS NULL");
    QQueue<int> q;
    while (rootGroupQuery.next()) {
      int groupId = rootGroupQuery.value(0).toInt();
      QSqlQuery* groupNameQuery = Queries::getQuery(Queries::MarketGroupNameQuery);
      groupNameQuery->bindValue(":id", groupId);
      groupNameQuery->exec();
      groupNameQuery->next();
      QString groupName = groupNameQuery->value(0).toString();
      QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(groupName));
      rootItems << item;
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
        QSqlQuery* subgroupNameQuery = Queries::getQuery(Queries::MarketGroupNameQuery);
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
        QTreeWidgetItem* item = new QTreeWidgetItem(parentItem, getStringListForType(typeId));
        typeOfItem[item] = typeId;
        itemOfType[typeId] = item;
      }
    }
    return rootItems;
  }));
  connect(rootItemsFutureWatcher, SIGNAL(resultReadyAt(int)),
          this, SLOT(resultReady()));
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

void TypeTree::resizeEvent(QResizeEvent* e)
{
  QRect newGeometry = busyBar->rect();
  newGeometry.moveCenter(QPoint(geometry().width() / 2, geometry().height() / 2));
  busyBar->setGeometry(newGeometry);
  QTreeWidget::resizeEvent(e);
}

QStringList TypeTree::mimeTypes() const {
  QStringList types;
  types << "bevel/type-variant"
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

void TypeTree::updatePriceButtonClicked(const QModelIndex& index)
{
  QTreeWidgetItem* item = itemFromIndex(index);
  if (!typeOfItem.contains(item))
    return;
  int typeId = typeOfItem[item];
  market->requestPrice(typeId);
}

void TypeTree::priceUpdated(int typeId)
{
  if (!itemOfType.contains(typeId))
    return;
  QTreeWidgetItem* item = itemOfType[typeId];
  QStringList strList = getStringListForType(typeId);
  for (int i = 0; i < strList.size(); i++)
    item->setText(i, strList[i]);
}

void TypeTree::resultReady()
{
  QList<QTreeWidgetItem*> rootItems = rootItemsFutureWatcher->future().result();
  addTopLevelItems(rootItems);
  busyBar->hide();
}

QStringList TypeTree::getStringListForType(int typeId)
{
  QStringList result;

  QSqlQuery* typeNameQuery = Queries::getQuery(Queries::TypeNameQuery);
  typeNameQuery->bindValue(":id", typeId);
  typeNameQuery->exec();
  typeNameQuery->next();
  result << typeNameQuery->value(0).toString();

  for (int i = 0; i < columns.size(); i++)
    switch (columns[i]) {
    case InfoButtonColumn:
    case UpdatePriceButtonColumn:
      result << "";
      break;
    case PriceColumn:
    {
      double sellPrice = market->getSellPrice(typeId);
      QString str;
      if (qIsNaN(sellPrice))
        str = tr("N/A");
      else
        str = locale.toString(sellPrice, 'f', 2);
      result << str;
      break;
    }
    }
  return result;
}
