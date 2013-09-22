#ifndef SKILLTRAININGQUEUELISTITEMWIDGET_HPP
#define SKILLTRAININGQUEUELISTITEMWIDGET_HPP

#include <QWidget>
#include <skilltrainingunit.hpp>

namespace Ui {
  class SkillTrainingQueueListItemWidget;
}

class Character;

class SkillTrainingQueueListItemWidget : public QWidget {
  Q_OBJECT
  
public:
  explicit SkillTrainingQueueListItemWidget(QWidget* parent = 0);
  ~SkillTrainingQueueListItemWidget();
  void init(SkillTrainingUnit unit, Character* chr);
  
private:
  SkillTrainingUnit unit;
  Character* chr;
private:
  Ui::SkillTrainingQueueListItemWidget* ui;
};

#endif // SKILLTRAININGQUEUELISTITEMWIDGET_HPP
