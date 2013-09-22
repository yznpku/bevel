#ifndef CHARACTERSKILLTREEITEMNAMEANDSKILLPOINTSWIDGET_HPP
#define CHARACTERSKILLTREEITEMNAMEANDSKILLPOINTSWIDGET_HPP

#include <QWidget>
#include <QSqlQuery>

class Character;

namespace Ui {
  class CharacterSkillTreeItemNameAndSkillPointsWidget;
}

class CharacterSkillTreeItemNameAndSkillPointsWidget : public QWidget {
  Q_OBJECT
  
public:
  CharacterSkillTreeItemNameAndSkillPointsWidget(QWidget* parent = 0);
  void init(int skillId, Character* chr);
  ~CharacterSkillTreeItemNameAndSkillPointsWidget();
public:
  void updateSkillPoints();
private:
  void querySkillName();
  void querySkillMultiplier();
private:
  Character* chr;
  int skillId;
  QString skillName;
  double skillMultiplier;
  double skillPoints;
private:
  Ui::CharacterSkillTreeItemNameAndSkillPointsWidget *ui;
};

#endif // CHARACTERSKILLTREEITEMNAMEANDSKILLPOINTSWIDGET_HPP
