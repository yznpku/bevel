#ifndef CHARACTERSKILLTREEITEMSKILLLEVELWIDGET_HPP
#define CHARACTERSKILLTREEITEMSKILLLEVELWIDGET_HPP

#include <QWidget>

namespace Ui {
  class CharacterSkillTreeItemSkillLevelWidget;
}

class Character;

class CharacterSkillTreeItemSkillLevelWidget : public QWidget {
  Q_OBJECT
public:
  explicit CharacterSkillTreeItemSkillLevelWidget(QWidget *parent = 0);
  ~CharacterSkillTreeItemSkillLevelWidget();
  void init(int skillId, Character* chr);
public slots:
  void updateSkillLevel();
private:
  int skillId;
  Character* chr;
private:
  Ui::CharacterSkillTreeItemSkillLevelWidget *ui;
};

#endif // CHARACTERSKILLTREEITEMSKILLLEVELWIDGET_HPP
