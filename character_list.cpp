#include "character_list.hpp"

#include <QAction>
#include <QContextMenuEvent>
#include <QListWidgetItem>
#include <QMenu>
#include <QSqlQuery>
#include <QVariant>
#include "characterdetailswidget.hpp"
#include "global.hpp"

CharacterList::CharacterList(QWidget* parent)
  : QListWidget(parent) {
  loadItems();
}

void CharacterList::contextMenuEvent(QContextMenuEvent* e) {
  QListWidgetItem* item = itemAt(e->pos());
  if (characterOfItem.contains(item)) {
    QMenu* menu = new QMenu(this);
    QAction* action = menu->addAction(tr("View Detail"));
    connect(action, SIGNAL(triggered(bool)),
            this, SLOT(characterViewDetailClicked()));
    menu->exec(viewport()->mapToGlobal(e->pos()));
  }
}

void CharacterList::characterViewDetailClicked() {
  if (!selectedItems().isEmpty()) {
    QListWidgetItem* item = selectedItems()[0];
    if (characterOfItem.contains(item)) {
      int characterId = characterOfItem[item];
      CharacterDetailsWidget* cdw = new CharacterDetailsWidget(mainWindow);
      cdw->initWithCharacter(characterId);
      cdw->show();
    }
  }
}

void CharacterList::loadItems() {
  QSqlQuery charactersQuery(QSqlDatabase::database("user"));
  charactersQuery.exec("SELECT characterId, name "
                       "FROM characters");
  while (charactersQuery.next()) {
    int characterId = charactersQuery.value(0).toInt();
    QString name = charactersQuery.value(1).toString();
    QListWidgetItem* item = new QListWidgetItem(name);
    characterOfItem[item] = characterId;
    addItem(item);
  }
}
