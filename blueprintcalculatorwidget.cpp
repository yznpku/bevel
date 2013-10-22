#include "blueprintcalculatorwidget.hpp"
#include "ui_blueprintcalculatorwidget.h"

#include <QMapIterator>
#include "pixmapbuttondelegate.hpp"
#include "itemdetailswidget.hpp"
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
  ui->materialsCostLabel->setBackgroundRole(QPalette::Window);
  ui->materialsCostLabel->setAutoFillBackground(true);
  ui->productPixmap->setFixedSize(64, 64);
  ui->productHorizontalLayout->setAlignment(ui->productGridLayout, Qt::AlignTop);
  ui->manufacturingTab->layout()->setAlignment(ui->manufacturingVerticalLayout, Qt::AlignTop);
  manufacturingRuns = 1;
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
  PixmapButtonDelegate* basicMaterialsTableInfoButtonDelegate =
      new PixmapButtonDelegate(getIconPixmap("38_16_208"));
  ui->basicMaterialsTable->setItemDelegateForColumn(5, basicMaterialsTableInfoButtonDelegate);

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
  PixmapButtonDelegate* extraMaterialsTableInfoButtonDelegate =
      new PixmapButtonDelegate(getIconPixmap("38_16_208"));
  ui->extraMaterialsTable->setItemDelegateForColumn(3, extraMaterialsTableInfoButtonDelegate);

  calculator = new BlueprintCalculator(this);

  connect(basicMaterialsTableInfoButtonDelegate, SIGNAL(clicked(QModelIndex)),
          this, SLOT(basicMaterialInfoButtonClicked(QModelIndex)));
  connect(extraMaterialsTableInfoButtonDelegate, SIGNAL(clicked(QModelIndex)),
          this, SLOT(extraMaterialInfoButtonClicked(QModelIndex)));
  connect(ui->blueprintPixmapLabel, SIGNAL(typeDropped(int)),
          this, SLOT(blueprintDropped(int)));
  connect(market, SIGNAL(priceUpdated(int)),
          this, SLOT(priceUpdated(int)));
  connect(ui->manufacturingRunsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(manufacturingRunsChanged(int)));
  connect(ui->manufacturingRunsSpinBox, SIGNAL(valueChanged(int)),
          calculator, SLOT(setManufacturingRuns(int)));
  connect(calculator, SIGNAL(basicMaterialsCostChanged(double)),
          this, SLOT(updateBasicMaterialsCost(double)));
  connect(calculator, SIGNAL(extraMaterialsCostChanged(double)),
          this, SLOT(updateExtraMaterialsCost(double)));
  connect(calculator, SIGNAL(manufacturingMaterialsCostChanged(double)),
          this, SLOT(updateManufacturingMaterialsCost(double)));
  connect(calculator, SIGNAL(productSellPriceChanged(double)),
          this, SLOT(updateProductSellPrice(double)));
  connect(calculator, SIGNAL(grossProfitChanged(double)),
          this, SLOT(updateGrossProfit(double)));
}

BlueprintCalculatorWidget::~BlueprintCalculatorWidget()
{
  delete ui;
}

void BlueprintCalculatorWidget::blueprintDropped(int blueprintId)
{
  this->blueprintId = blueprintId;
  calculator->init(blueprintId);

  QSqlQuery* typeNameQuery = Queries::getQuery(Queries::TypeNameQuery);
  typeNameQuery->bindValue(":id", blueprintId);
  typeNameQuery->exec();
  typeNameQuery->next();
  QString blueprintName = typeNameQuery->value(0).toString();
  ui->blueprintNameLabel->setText(QString("<h2>%1</h2>").arg(blueprintName));
  ui->blueprintInfoButton->init(blueprintId);

  productId = BlueprintCalculator::queryProductId(blueprintId);

  typeNameQuery->bindValue(":id", productId);
  typeNameQuery->exec();
  typeNameQuery->next();
  QString productName = typeNameQuery->value(0).toString();
  ui->productNameLabel->setText(QString("<h2>%1</h2>").arg(productName));
  ui->productPixmap->setPixmap(*getTypePixmap64(productId));
  ui->productInfoButton->init(productId);

  portionSize = BlueprintCalculator::queryPortionSize(productId);
  ui->productPortionSizeLabel->setText(QString("*%1").arg(QString::number(portionSize)));

  basicMaterials = calculator->getBasicMaterials();
  extraMaterials = calculator->getExtraMaterials();
  requestPrices();
  fillTables();
}

void BlueprintCalculatorWidget::priceUpdated(int typeId)
{
  if (basicMaterials.contains(typeId))
    updateBasicMaterialItem(typeId);
  if (extraMaterials.contains(typeId))
    updateExtraMaterialItem(typeId);
}

void BlueprintCalculatorWidget::manufacturingRunsChanged(int manufacturingRuns)
{
  this->manufacturingRuns = manufacturingRuns;
  updateManufacturingMaterialItems();
}

void BlueprintCalculatorWidget::basicMaterialInfoButtonClicked(const QModelIndex& index)
{
  Q_UNUSED(index)
  QTreeWidgetItem* item = ui->basicMaterialsTable->selectedItems()[0];
  if (!materialOfItem.contains(item))
    return;
  int typeId = materialOfItem[item];
  ItemDetailsWidget* widget = new ItemDetailsWidget(typeId, mainWindow);
  widget->show();
}

void BlueprintCalculatorWidget::extraMaterialInfoButtonClicked(const QModelIndex& index)
{
  Q_UNUSED(index)
  QTreeWidgetItem* item = ui->extraMaterialsTable->selectedItems()[0];
  if (!materialOfItem.contains(item))
    return;
  int typeId = materialOfItem[item];
  ItemDetailsWidget* widget = new ItemDetailsWidget(typeId, mainWindow);
  widget->show();
}

void BlueprintCalculatorWidget::updateBasicMaterialsCost(double cost)
{
  QString str = qIsNaN(cost) ? tr("N/A") : locale.toString(cost, 'f', 2);
  str += " ISK";
  ui->basicMaterialsCostLabel->setText(str);
}

void BlueprintCalculatorWidget::updateExtraMaterialsCost(double cost)
{
  QString str = qIsNaN(cost) ? tr("N/A") : locale.toString(cost, 'f', 2);
  str += " ISK";
  ui->extraMaterialsCostLabel->setText(str);
}

void BlueprintCalculatorWidget::updateManufacturingMaterialsCost(double cost)
{
  QString str = qIsNaN(cost) ? tr("N/A") : locale.toString(cost, 'f', 2);
  str += " ISK";
  ui->materialsCostLabel->setText(str);
}

void BlueprintCalculatorWidget::updateProductSellPrice(double sellPrice)
{
  QString str = qIsNaN(sellPrice) ? tr("N/A") : locale.toString(sellPrice, 'f', 2);
  str += " ISK";
  ui->sellPriceLabel->setText(str);
}

void BlueprintCalculatorWidget::updateGrossProfit(double grossProfit)
{
  QString str = qIsNaN(grossProfit) ? tr("N/A") : locale.toString(grossProfit, 'f', 2);
  str += " ISK";
  ui->profitLabel->setText(str);
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
  market->requestPrice(productId);
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
    materialOfItem[item] = i.key();
    updateBasicMaterialItem(i.key());
  }
  for (QMapIterator<int, int> i(extraMaterials); i.hasNext();) {
    i.next();
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setIcon(0, QIcon(*getTypePixmap32(i.key())));
    ui->extraMaterialsTable->addTopLevelItem(item);
    itemOfExtraMaterial[i.key()] = item;
    materialOfItem[item] = i.key();
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

void BlueprintCalculatorWidget::updateManufacturingMaterialItems()
{
  for (QMapIterator<int, int> i(basicMaterials); i.hasNext();) {
    i.next();
    int materialTypeId = i.key();
    updateBasicMaterialItem(materialTypeId);
  }
  for (QMapIterator<int, int> i(extraMaterials); i.hasNext();) {
    i.next();
    int materialTypeId = i.key();
    updateExtraMaterialItem(materialTypeId);
  }
}

QStringList BlueprintCalculatorWidget::getStringListForMaterial(int materialTypeId, int quantity, bool withWaste)
{
  QStringList result;

  QSqlQuery* typeNameQuery = Queries::getQuery(Queries::TypeNameQuery);
  typeNameQuery->bindValue(":id", materialTypeId);
  typeNameQuery->exec();
  typeNameQuery->next();
  result << typeNameQuery->value(0).toString();
  result << locale.toString(quantity * manufacturingRuns);
  int actualQuantity = quantity * manufacturingRuns;
  if (withWaste) {
    result << locale.toString(BlueprintCalculator::getMeRequiredForOptimalMaterial(quantity));
    actualQuantity = BlueprintCalculator::getQuantityWithWaste(quantity, 0) * manufacturingRuns;
    result << locale.toString(actualQuantity);
  }
  double sellPrice = market->getSellPrice(materialTypeId) * actualQuantity;
  result << (qIsNaN(sellPrice) ? tr("N/A") : locale.toString(sellPrice, 'f', 2));

  return result;
}

