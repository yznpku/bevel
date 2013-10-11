#include "itemdetailswidget.hpp"
#include "ui_itemdetailswidget.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMapIterator>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>

#include "attribute_set.hpp"
#include "global.hpp"
#include "attributetree.h"
#include "skillrequirementtree.hpp"

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
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(tr("Item Details - %1").arg(as.name));
  ui->typePixmapLabel->setPixmap(*getTypePixmap64(typeId));
  ui->typeNameLabel->setText(QString("<h2>%1</h2>").arg(as.name));
  ui->horizontalLayout->setAlignment(ui->verticalLayout_2, Qt::AlignTop);
  initDescriptionTab();
  initAttributeTree();
  initSkillRequirementTree();
  fillTabs();
}

void ItemDetailsWidget::initDescriptionTab()
{
  descriptionTab = new QTextEdit();
  descriptionTab->setText(as.description);
}

void ItemDetailsWidget::initAttributeTree()
{
  at = new AttributeTree();
  at->init(as);
}

void ItemDetailsWidget::initSkillRequirementTree() {
  srt = new SkillRequirementTree(typeId);
}

void ItemDetailsWidget::fillTabs() {
  ui->tabs->addTab(descriptionTab, tr("Description"));
  ui->tabs->addTab(at, tr("Attributes"));
  ui->tabs->addTab(srt, tr("Skill Requirement"));
}
