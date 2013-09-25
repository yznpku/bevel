#include "itemdetailswidget.hpp"
#include "ui_itemdetailswidget.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMapIterator>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTabWidget>
#include <QtDebug>
#include <QTextEdit>
#include <QVBoxLayout>

#include "attribute_set.hpp"
#include "global.hpp"
#include "skill_requirement_tree.hpp"

ItemDetailsWidget::ItemDetailsWidget(int typeId, QWidget* parent)
  : QWidget(parent) {
  this->typeId = typeId;
  as = AttributeSet::fromPrototype(typeId);
  init();
}

ItemDetailsWidget::ItemDetailsWidget(const AttributeSet& as, QWidget* parent)
  : QWidget(parent) {
  this->as = as;
  this->typeId = as.prototypeId;
  init();
}

ItemDetailsWidget::~ItemDetailsWidget() {
  delete ui;
}

void ItemDetailsWidget::init() {
  ui = new Ui::ItemDetailsWidget;
  ui->setupUi(this);
  setWindowFlags(Qt::Dialog);
  setWindowTitle(tr("Item Details - %1").arg(as.name));
  ui->typePixmapLabel->setPixmap(*getTypePixmap64(typeId));
  ui->typeNameLabel->setText(QString("<h2>%1</h2>").arg(as.name));
  ui->horizontalLayout->setAlignment(ui->verticalLayout_2, Qt::AlignTop);
  fillTables();
  fillSkillRequirementTree();
  fillTabs();
}

void ItemDetailsWidget::fillTables() {
  QSqlQuery attributeQuery(QSqlDatabase::database("static"));
  attributeQuery.prepare(QString("select ATTRIBUTENAME, CATEGORYID, PUBLISHED "
                                 "from dgmAttributeTypes "
                                 "where ATTRIBUTEID = :id"));
  for (QMapIterator<int, double> i(as.attr); i.hasNext();) {
    i.next();
    int attributeId = i.key();
    attributeQuery.bindValue(":id", attributeId);
    attributeQuery.exec();
    attributeQuery.next();
    QString attributeName = attributeQuery.value(0).toString();
    int categoryId = attributeQuery.value(1).toInt();
    if (!categoryId)            // Put attribute without a category
      categoryId = 7;           // into category Miscellaneous
    bool published = attributeQuery.value(2).toBool();
    if (!published)
      continue;
    if (!tableOfCategory.contains(categoryId)) {
      tableOfCategory[categoryId] = new QTableWidget(0, 2);
      tableOfCategory[categoryId]->setHorizontalHeaderLabels(QStringList({tr("Attribute"), tr("Value")}));
      tableOfCategory[categoryId]->verticalHeader()->hide();
      tableOfCategory[categoryId]->horizontalHeader()
          ->setSectionResizeMode(0, QHeaderView::Stretch);
      tableOfCategory[categoryId]->horizontalHeader()
          ->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    }
    tableOfCategory[categoryId]->insertRow(tableOfCategory[categoryId]->rowCount());
    tableOfCategory[categoryId]->setItem(tableOfCategory[categoryId]->rowCount() - 1,
                                         0, new QTableWidgetItem(decamelize(attributeName)));
    tableOfCategory[categoryId]->setItem(tableOfCategory[categoryId]->rowCount() - 1,
                                         1, new QTableWidgetItem(QString::number(i.value(), 'f', 2)));
  }
}

void ItemDetailsWidget::fillSkillRequirementTree() {
  srt = new SkillRequirementTree(typeId);
}

void ItemDetailsWidget::fillTabs() {
  descriptionTab = new QTextEdit();
  descriptionTab->setText(as.description);
  ui->tabs->addTab(descriptionTab, tr("Description"));
  QSqlQuery categoryQuery(QSqlDatabase::database("static"));
  categoryQuery.prepare(QString("select CATEGORYNAME "
                                "from dgmAttributeCategories "
                                "where CATEGORYID = :id"));
  for (QMapIterator<int, QTableWidget*> i(tableOfCategory); i.hasNext();) {
    i.next();
    categoryQuery.bindValue(":id", i.key());
    categoryQuery.exec();
    categoryQuery.next();
    QString categoryName = categoryQuery.value(0).toString();
    ui->tabs->addTab(i.value(), categoryName);
  }
  ui->tabs->addTab(srt, tr("Skill Requirement"));
}
