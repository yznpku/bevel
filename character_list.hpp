#ifndef _CHARACTER_LIST_HPP_
#define _CHARACTER_LIST_HPP_

#include <QListWidget>

class QContextMenuEvent;
class QListWidgetItem;

class CharacterList : public QListWidget { Q_OBJECT
public:
  CharacterList(QWidget* parent = 0);
protected:
  virtual void contextMenuEvent(QContextMenuEvent* e);
private slots:
  void characterViewDetailClicked();
private:
  void loadItems();
private:
  QMap<QListWidgetItem*, int> characterOfItem;
};

#endif
