#include "skilltrainingqueuelistitemwidget.hpp"
#include "ui_skilltrainingqueuelistitemwidget.h"

SkillTrainingQueueListItemWidget::SkillTrainingQueueListItemWidget(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::SkillTrainingQueueListItemWidget) {
  ui->setupUi(this);
}

SkillTrainingQueueListItemWidget::~SkillTrainingQueueListItemWidget() {
  delete ui;
}

void SkillTrainingQueueListItemWidget::init(SkillTrainingUnit unit, Character* chr) {
  this->unit = unit;
  this->chr = chr;
  ui->nameAndSkillPointsWidget->init(unit.skillId, chr);
  ui->skillLevelDisplay->init(unit, chr);
}
