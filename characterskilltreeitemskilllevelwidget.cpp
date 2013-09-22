#include "characterskilltreeitemskilllevelwidget.hpp"
#include "ui_characterskilltreeitemskilllevelwidget.h"

CharacterSkillTreeItemSkillLevelWidget::CharacterSkillTreeItemSkillLevelWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CharacterSkillTreeItemSkillLevelWidget) {
  ui->setupUi(this);
}

CharacterSkillTreeItemSkillLevelWidget::~CharacterSkillTreeItemSkillLevelWidget() {
  delete ui;
}

void CharacterSkillTreeItemSkillLevelWidget::init(int skillId, Character* chr) {
  this->skillId = skillId;
  this->chr = chr;
  ui->skillLevelDisplay->init(skillId, chr);
}

void CharacterSkillTreeItemSkillLevelWidget::updateSkillLevel() {
  ui->skillLevelDisplay->updateSkillLevel();
}
