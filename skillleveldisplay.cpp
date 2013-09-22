#include "skillleveldisplay.hpp"

#include <QPainter>
#include <QPen>
#include <QSqlQuery>
#include <QVariant>
#include "character.hpp"
#include "global.hpp"
#include "queries.hpp"

SkillLevelDisplay::SkillLevelDisplay(QWidget *parent) :
  QWidget(parent) {
  skillId = 0;
  chr = 0;
  setFixedSize(48, 10);
}

void SkillLevelDisplay::init(int skillId, Character* chr) {
  this->skillId = skillId;
  this->chr = chr;
  trainingToLevel = -1;
  updateSkillLevel();
}

void SkillLevelDisplay::init(SkillTrainingUnit unit, Character* chr) {
  this->skillId = unit.skillId;
  this->chr = chr;
  updateSkillLevel();
  this->trainingToLevel = unit.level;
}

void SkillLevelDisplay::updateSkillLevel() {
  skillLevel = chr->getSkillLevel(skillId);
  trainingToLevel = -1;
  for (int i = chr->skillTrainingQueue.size() - 1; i >= 0; i--)
    if (chr->skillTrainingQueue[i].skillId == skillId) {
      trainingToLevel = chr->skillTrainingQueue[i].level;
      break;
    }
  update();
}

void SkillLevelDisplay::paintEvent(QPaintEvent* e) {
  Q_UNUSED(e);
  if (!chr)
    return;
  QPainter painter(this);
  QColor primary(124, 120, 119);
  QColor secondary(1, 134, 175);
  painter.setPen(QPen(primary, 1));
  painter.setBrush(primary);
  painter.drawLine(0, 0, 47, 0);
  painter.drawLine(47, 0, 47, 9);
  painter.drawLine(47, 9, 0, 9);
  painter.drawLine(0, 9, 0, 0);
  int i;
  for (i = 0; i < skillLevel; i++)
    painter.drawRect(2 + 9 * i, 2, 7, 5);
  painter.setBrush(secondary);
  for (; i < trainingToLevel; i++)
    painter.drawRect(2 + 9 * i, 2, 7, 5);
}
