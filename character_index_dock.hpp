#ifndef _CHARACTER_INDEX_DOCK_HPP_
#define _CHARACTER_INDEX_DOCK_HPP_

#include <QDockWidget>

class CharacterList;

class CharacterIndexDock : public QDockWidget {
public:
  CharacterIndexDock(QWidget* parent = 0);
private:
  CharacterList* cl;
};

#endif
