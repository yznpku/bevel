#ifndef SKILLLEVELDISPLAY_HPP
#define SKILLLEVELDISPLAY_HPP

#include <QWidget>

#include "skilltrainingunit.hpp"

class QPaintEvent;
class Character;

class SkillLevelDisplay : public QWidget {
  Q_OBJECT
public:
  explicit SkillLevelDisplay(QWidget *parent = 0);
  void init(int skillId, Character* chr);
  void init(SkillTrainingUnit unit, Character* chr);
  void updateSkillLevel();
protected:
  virtual void paintEvent(QPaintEvent* e);
private:
  int skillId;
  Character* chr;
  int skillLevel;
  int trainingToLevel;
};

#endif // SKILLLEVELDISPLAY_HPP
