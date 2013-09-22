#ifndef CHARACTERDETAILSWIDGET_HPP
#define CHARACTERDETAILSWIDGET_HPP

#include <QWidget>

class Character;

namespace Ui {
  class CharacterDetailsWidget;
}

class CharacterDetailsWidget : public QWidget
{
  Q_OBJECT
  
public:
  explicit CharacterDetailsWidget(QWidget *parent = 0);
  void initWithCharacter(int characterId);
  ~CharacterDetailsWidget();
  
private:
  Character* chr;
private:
  Ui::CharacterDetailsWidget *ui;
};

#endif // CHARACTERDETAILSWIDGET_HPP
