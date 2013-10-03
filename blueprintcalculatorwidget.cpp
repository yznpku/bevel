#include "blueprintcalculatorwidget.hpp"
#include "ui_blueprintcalculatorwidget.h"

#include <QMapIterator>
#include "global.hpp"
#include "queries.hpp"
#include "market.hpp"
#include <QtNumeric>

BlueprintCalculatorWidget::BlueprintCalculatorWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::BlueprintCalculatorWidget)
{
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowFlags(Qt::Dialog);
  ui->productPixmap->setFixedSize(64, 64);
  ui->productHorizontalLayout->setAlignment(ui->productGridLayout, Qt::AlignTop);
  ui->manufacturingTab->layout()->setAlignment(ui->manufacturingVerticalLayout, Qt::AlignTop);
  locale = QLocale(QLocale::English);

  ui->basicMaterialsTable->setColumnCount(6);
  ui->basicMaterialsTable->setHeaderItem(new QTreeWidgetItem({tr("Name"),
                                                              tr("Opt.Qty."),
                                                              tr("ME Req."),
                                                              tr("Qty."),
                                                              tr("Cost"),
                                                              ""}));
  ui->basicMaterialsTable->header()->setStretchLastSection(false);
  ui->basicMaterialsTable->header()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->basicMaterialsTable->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->basicMaterialsTable->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->basicMaterialsTable->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->basicMaterialsTable->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->basicMaterialsTable->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

  ui->extraMaterialsTable->setColumnCount(4);
  ui->extraMaterialsTable->setHeaderItem(new QTreeWidgetItem({tr("Name"),
                                                              tr("Qty."),
                                                              tr("Cost"),
                                                              ""}));
  ui->extraMaterialsTable->header()->setStretchLastSection(false);
  ui->extraMaterialsTable->header()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->extraMaterialsTable->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->extraMaterialsTable->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->extraMaterialsTable->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

  connect(ui->blueprintPixmapLabel, SIGNAL(typeDropped(int)),
          this, SLOT(blueprintDropped(int)));
  connect(market, SIGNAL(priceUpdated(int)),
          this, SLOT(priceUpdated(int)));
}

BlueprintCalculatorWidget::~BlueprintCalculatorWidget()
{
  delete ui;
}

void BlueprintCalculatorWidget::blueprintDropped(int blueprintId)
{
  QSqlQuery* typeNameQuery = Queries::getTypeNameQuery();
  typeNameQuery->bindValue(":id", blueprintId);
  typeNameQuery->exec();
  typeNameQuery->next();
  QString blueprintName = typeNameQuery->value(0).toString();
  ui->blueprintNameLabel->setText(QString("<h2>%1</h2>").arg(blueprintName));
  ui->blueprintInfoButton->init(blueprintId);

  QSqlQuery* productQuery = Queries::getProductForBlueprintQuery();
  productQuery->bindValue(":id", blueprintId);
  productQuery->exec();
  productQuery->next();
  int productId = productQuery->value(0).toInt();
  typeNameQuery->bindValue(":id", productId);
  typeNameQuery->exec();
  typeNameQuery->next();
  QString productName = typeNameQuery->value(0).toString();
  ui->productNameLabel->setText(QString("<h2>%1</h2>").arg(productName));
  ui->productPixmap->setPixmap(*getTypePixmap64(productId));
  ui->productInfoButton->init(productId);

  QSqlQuery* portionSizeQuery = Queries::getTypePortionSizeQuery();
  portionSizeQuery->bindValue(":id", productId);
  portionSizeQuery->exec();
  portionSizeQuery->next();
  int portionSize = portionSizeQuery->value(0).toInt();
  ui->productPortionSizeLabel->setText(QString("*%1").arg(QString::number(portionSize)));

  this->blueprintId = blueprintId;
  this->productId = productId;
  this->portionSize = portionSize;

  getMaterials();
  requestPrices();
  fillTables();
}

void BlueprintCalculatorWidget::priceUpdated(int typeId)
{
  updateBasicMaterialItem(typeId);
  updateExtraMaterialItem(typeId);
}

void BlueprintCalculatorWidget::getMaterials()
{
  basicMaterials.clear();
  extraMaterials.clear();

  QSqlQuery* basicMaterialsQuery = Queries::getBasicMaterialsQuery();
  basicMaterialsQuery->bindValue(":id", productId);
  basicMaterialsQuery->exec();
  while (basicMaterialsQuery->next()) {
    int materialTypeId = basicMaterialsQuery->value(0).toInt();
    int quantity = basicMaterialsQuery->value(1).toInt();
    basicMaterials[materialTypeId] = quantity;
  }

  QSqlQuery* extraMaterialsQuery = Queries::getExtraMaterialsQuery();
  extraMaterialsQuery->bindValue(":id", blueprintId);
  extraMaterialsQuery->exec();
  while (extraMaterialsQuery->next()) {
    int materialTypeId = extraMaterialsQuery->value(0).toInt();
    int quantity = extraMaterialsQuery->value(1).toInt();
    QSqlQuery* categoryQuery = Queries::getCategoryOfTypeQuery();
    categoryQuery->bindValue(":id", materialTypeId);
    categoryQuery->exec();
    categoryQuery->next();
    if (categoryQuery->value(0).toInt() == 16) // Do not treat skills as materials
      continue;
    extraMaterials[materialTypeId] = quantity;
  }
}

void BlueprintCalculatorWidget::requestPrices()
{
  for (QMapIterator<int, int> i(basicMaterials); i.hasNext();) {
    i.next();
    market->requestPrice(i.key());
  }
  for (QMapIterator<int, int> i(extraMaterials); i.hasNext();) {
    i.next();
    market->requestPrice(i.key());
  }
}

void BlueprintCalculatorWidget::fillTables()
{
  ui->basicMaterialsTable->clear();
  ui->extraMaterialsTable->clear();
  itemOfBasicMaterial.clear();
  itemOfExtraMaterial.clear();

  for (QMapIterator<int, int> i(basicMaterials); i.hasNext();) {
    i.next();
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setIcon(0, QIcon(*getTypePixmap32(i.key())));
    ui->basicMaterialsTable->addTopLevelItem(item);
    itemOfBasicMaterial[i.key()] = item;
    updateBasicMaterialItem(i.key());
  }
  for (QMapIterator<int, int> i(extraMaterials); i.hasNext();) {
    i.next();
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setIcon(0, QIcon(*getTypePixmap32(i.key())));
    ui->extraMaterialsTable->addTopLevelItem(item);
    itemOfExtraMaterial[i.key()] = item;
    updateExtraMaterialItem(i.key());
  }
}

void BlueprintCalculatorWidget::updateBasicMaterialItem(int typeId)
{
  if (!itemOfBasicMaterial.contains(typeId))
    return;
  QTreeWidgetItem* item = itemOfBasicMaterial[typeId];
  QStringList strList = getStringListForMaterial(typeId, basicMaterials[typeId], true);
  for (int i = 0; i < strList.size(); i++)
    item->setText(i, strList[i]);
}

void BlueprintCalculatorWidget::updateExtraMaterialItem(int typeId)
{
  if (!itemOfExtraMaterial.contains(typeId))
    return;
  QTreeWidgetItem* item = itemOfExtraMaterial[typeId];
  QStringList strList = getStringListForMaterial(typeId, extraMaterials[typeId], false);
  for (int i = 0; i < strList.size(); i++)
    item->setText(i, strList[i]);
}

QStringList BlueprintCalculatorWidget::getStringListForMaterial(int materialTypeId, int quantity, bool withWaste)
{
  QStringList result;

  QSqlQuery* typeNameQuery = Queries::getTypeNameQuery();
  typeNameQuery->bindValue(":id", materialTypeId);
  typeNameQuery->exec();
  typeNameQuery->next();
  result << typeNameQuery->value(0).toString();
  result << locale.toString(quantity);
  int actualQuantity = quantity;
  if (withWaste) {
    result << locale.toString(getMeRequiredForOptimalMaterial(quantity));
    actualQuantity = getQuantityWithWaste(quantity, 0);
    result << locale.toString(actualQuantity);
  }
  double sellPrice = market->getSellPrice(materialTypeId) * actualQuantity;
  result << (qIsNaN(sellPrice) ? tr("N/A") : locale.toString(sellPrice));

  return result;
}

int BlueprintCalculatorWidget::getMeRequiredForOptimalMaterial(int materialQuantity) const
{
  return int(0.2 * materialQuantity);
}

int BlueprintCalculatorWidget::getQuantityWithWaste(int quantity, int me) const
{
  double wasteFactor;
  if (me < 0)
    wasteFactor = 0.1 * (1 - me);
  else
    wasteFactor = 0.1 / (1 + me);
  return qRound(quantity * (1 + wasteFactor));
}
