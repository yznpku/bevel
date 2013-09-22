#include "characterdetailswidget.hpp"
#include "ui_characterdetailswidget.h"

#include "character.hpp"

CharacterDetailsWidget::CharacterDetailsWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CharacterDetailsWidget) {
  setWindowFlags(Qt::Dialog);
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
  connect(ui->skillTrainingQueueListWidget, SIGNAL(skillChanged(int)),
          ui->characterSkillTree, SLOT(updateSkillLevelDisplay(int)));
}

void CharacterDetailsWidget::initWithCharacter(int characterId) {
  chr = Character::fromId(characterId);
  setWindowTitle(tr("Character Details - %1").arg(chr->as.name));
  ui->characterSkillTree->initWithCharacter(chr);
  ui->skillTrainingQueueListWidget->setCharacter(chr);
}

CharacterDetailsWidget::~CharacterDetailsWidget() {
  delete ui;
}
