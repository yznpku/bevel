#ifndef _CHARACTER_SKILL_TREE_HPP_
#define _CHARACTER_SKILL_TREE_HPP_

#include <QTreeWidget>

#include <QStringList>
#include <QMap>
#include "character.hpp"

class QTreeWidgetItem;
class QContextMenuEvent;
class QDragEnterEvent;
class QDropEvent;
class QDragMoveEvent;
class QMimeData;

class CharacterSkillTree : public QTreeWidget {
  Q_OBJECT
public:
  CharacterSkillTree(QWidget* parent = 0);
  void initWithCharacter(Character* chr);
protected:
  virtual void contextMenuEvent(QContextMenuEvent* e);
  virtual void dragEnterEvent(QDragEnterEvent* e);
  virtual void dragMoveEvent(QDragMoveEvent* e);
  virtual void dropEvent(QDropEvent* e);
  virtual QStringList mimeTypes() const;
  virtual QMimeData* mimeData(const QList<QTreeWidgetItem*> items) const;
public slots:
  void updateSkillLevelDisplay(int skillId);
private slots:
  void setSkillPointsActionTriggered();
  void deleteSkillActionTriggered();
  void updateSkillPoints(int skillId);
  void deleteSkillItem(int skillId);
  void addSkillItem(int skillId);
private:
  void addSkillItem(int skillId, QTreeWidgetItem* parent);
  void addGroupItem(int groupId, QString groupName);
private:
  Character* chr;
  QMap<QTreeWidgetItem*, int> groupOfItem;
  QMap<int, QTreeWidgetItem*> itemOfGroup;
  QMap<QTreeWidgetItem*, int> skillOfItem;
  QMap<int, QTreeWidgetItem*> itemOfSkill;
};


#endif
