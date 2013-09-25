#include "blueprintcalculatorwidget.hpp"
#include "ui_blueprintcalculatorwidget.h"

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
}

BlueprintCalculatorWidget::~BlueprintCalculatorWidget()
{
  delete ui;
}
