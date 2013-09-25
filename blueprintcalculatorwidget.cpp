#include "blueprintcalculatorwidget.hpp"
#include "ui_blueprintcalculatorwidget.h"

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
}
