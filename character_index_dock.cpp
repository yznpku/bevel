#include "character_index_dock.hpp"

#include "character_list.hpp"

CharacterIndexDock::CharacterIndexDock(QWidget* parent)
  : QDockWidget(tr("Character Index"), parent) {
  cl = new CharacterList(this);
  setWidget(cl);
}
