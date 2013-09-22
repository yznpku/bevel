#include "characterskilltreeitemnameandskillpointswidget.hpp"
#include "ui_characterskilltreeitemnameandskillpointswidget.h"

#include "character.hpp"
#include "global.hpp"
#include "queries.hpp"

CharacterSkillTreeItemNameAndSkillPointsWidget::
CharacterSkillTreeItemNameAndSkillPointsWidget(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::CharacterSkillTreeItemNameAndSkillPointsWidget) {
  ui->setupUi(this);
}

void CharacterSkillTreeItemNameAndSkillPointsWidget::init(int skillId, Character* chr) {
  this->skillId = skillId;
  this->chr = chr;
  skillPoints = chr->skills[skillId];
  querySkillName();
  querySkillMultiplier();
  ui->skillNameLabel->setText(skillName);
  ui->skillBookPimapLabel->setPixmap(*getTypePixmap32(skillId));
  updateSkillPoints();
}

CharacterSkillTreeItemNameAndSkillPointsWidget::~CharacterSkillTreeItemNameAndSkillPointsWidget() {
  delete ui;
}

void CharacterSkillTreeItemNameAndSkillPointsWidget::updateSkillPoints() {
  skillPoints = chr->skills[skillId];
  ui->skillPointsLabel->setText(tr("Skill Points: %1/%2")
                                .arg(QLocale(QLocale::English).toString((int)ceil(skillPoints)),
                                     QLocale(QLocale::English).toString((int)ceil(skillMultiplier *
                                                                                  BASE_SKILL_POINTS[5]))));
}

void CharacterSkillTreeItemNameAndSkillPointsWidget::querySkillName() {
  QSqlQuery* skillNameQuery = Queries::getTypeNameQuery();
  skillNameQuery->bindValue(":id", skillId);
  skillNameQuery->exec();
  skillNameQuery->next();
  skillName = skillNameQuery->value(0).toString();
}

void CharacterSkillTreeItemNameAndSkillPointsWidget::querySkillMultiplier() {
  QSqlQuery* skillMultiplierQuery = Queries::getSkillMultiplierQuery();
  skillMultiplierQuery->bindValue(":id", skillId);
  skillMultiplierQuery->exec();
  skillMultiplierQuery->next();
  skillMultiplier = skillMultiplierQuery->value(0).toDouble();
}
