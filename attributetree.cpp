#include "attributetree.h"

#include <QHeaderView>
#include "global.hpp"
#include "queries.hpp"

AttributeTree::AttributeTree(QWidget* parent) :
  QTreeWidget(parent)
{
  setColumnCount(2);
  setHeaderItem(new QTreeWidgetItem({tr("Name"), tr("Value")}));
  header()->setStretchLastSection(false);
  header()->setSectionResizeMode(0, QHeaderView::Stretch);
  header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void AttributeTree::init(const AttributeSet& as)
{
  this->as = as;
  QSqlQuery attributeQuery(QSqlDatabase::database("static"));
  attributeQuery.prepare(QString("select DISPLAYNAME, CATEGORYID, PUBLISHED, UNITID "
                                 "from dgmAttributeTypes "
                                 "where ATTRIBUTEID = :id"));
  for (QMapIterator<int, double> i(as.attr); i.hasNext();) {
    i.next();
    int attributeId = i.key();
    attributeQuery.bindValue(":id", attributeId);
    attributeQuery.exec();
    attributeQuery.next();
    QString displayName = attributeQuery.value(0).toString();
    int categoryId = attributeQuery.value(1).toInt();
    if (!categoryId)            // Put attribute without a category
      categoryId = 7;           // into category Miscellaneous
    bool published = attributeQuery.value(2).toBool();
    if (!published)
      continue;
    int unitId = attributeQuery.value(3).toInt();
    if (!itemOfCategory.contains(categoryId)) {
      QSqlQuery categoryNameQuery(QSqlDatabase::database("static"));
      categoryNameQuery.prepare("SELECT categoryName "
                                "FROM dgmAttributeCategories "
                                "WHERE categoryId = :id");
      categoryNameQuery.bindValue(":id", categoryId);
      categoryNameQuery.exec();
      categoryNameQuery.next();
      QString categoryName = categoryNameQuery.value(0).toString();
      itemOfCategory[categoryId] = new QTreeWidgetItem(this, {categoryName, ""});
    }
    QString value = intOrDoubleToString(i.value());
    QSqlQuery* unitDisplayNameQuery = Queries::getUnitDisplayNameQuery();
    unitDisplayNameQuery->bindValue(":id", unitId);
    unitDisplayNameQuery->exec();
    if (unitDisplayNameQuery->next()) {
      QString unitDisplayName = unitDisplayNameQuery->value(0).toString();
      value = value + ' ' + unitDisplayName;
    }
    QTreeWidgetItem* item = new QTreeWidgetItem({displayName, value});
    itemOfCategory[categoryId]->addChild(item);
  }
  expandAll();
}
