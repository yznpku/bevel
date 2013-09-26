#include "blueprintcalculatorwidget.hpp"
#include "ui_blueprintcalculatorwidget.h"

#include <QMapIterator>
#include "global.hpp"
#include "queries.hpp"

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
  fillTables();
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
    extraMaterials[materialTypeId] = quantity;
  }
}

void BlueprintCalculatorWidget::fillTables()
{
  for (QMapIterator<int, int> i(basicMaterials); i.hasNext();) {
    i.next();
    QStringList strList = getStringListForMaterial(i.key(), i.value(), true);
    ui->basicMaterialsTable->addTopLevelItem(new QTreeWidgetItem(strList));
  }
  for (QMapIterator<int, int> i(extraMaterials); i.hasNext();) {
    i.next();
    QStringList strList = getStringListForMaterial(i.key(), i.value(), false);
    ui->extraMaterialsTable->addTopLevelItem(new QTreeWidgetItem(strList));
  }
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
  if (withWaste) {
    result << "";
    int currentQuantity = int(quantity * 1.1);
    result << locale.toString(currentQuantity);
  }
  result << "";

  return result;
}
