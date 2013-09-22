#include "characterskilltree.hpp"

#include <QDataStream>
#include <QHeaderView>
#include <QMapIterator>
#include <QMimeData>
#include <QTreeWidgetItem>
#include <QtSql>
#include "characterskilltreeitemnameandskillpointswidget.hpp"
#include "characterskilltreeitemskilllevelwidget.hpp"
#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>
#include "queries.hpp"
#include "global.hpp"
#include "skillleveldisplay.hpp"
#include "typevariant.hpp"

CharacterSkillTree::CharacterSkillTree(QWidget* parent)
  : QTreeWidget(parent) {
  this->setDragEnabled(true);
  this->setAcceptDrops(true);
  setColumnCount(2);
  setHeaderItem(new QTreeWidgetItem(QStringList({tr("Name"), ""})));
  header()->setStretchLastSection(false);
  header()->setSectionResizeMode(0, QHeaderView::Stretch);
  header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void CharacterSkillTree::initWithCharacter(Character* chr) {
  this->chr = chr;
  QSqlQuery* skillGroupQuery = Queries::getGroupOfTypeQuery();
  QMap<int, int> groupOfSkill;
  QSet<int> groups;
  for (QMapIterator<int, double> i(chr->skills); i.hasNext();) {
    i.next();
    skillGroupQuery->bindValue(":id", i.key());
    skillGroupQuery->exec();
    skillGroupQuery->next();
    int groupId = skillGroupQuery->value(0).toInt();
    groupOfSkill[i.key()] = groupId;
    groups << groupId;
  }

  QSqlQuery* groupNameQuery = Queries::getGroupNameQuery();
  for (QSetIterator<int> i(groups); i.hasNext();) {
    int groupId = i.next();
    groupNameQuery->bindValue(":id", groupId);
    groupNameQuery->exec();
    groupNameQuery->next();
    QString groupName = groupNameQuery->value(0).toString();
    addGroupItem(groupId, groupName);
  }

  for (QMapIterator<int, double> i(chr->skills); i.hasNext();) {
    i.next();
    int skillId = i.key();
    addSkillItem(skillId, itemOfGroup[groupOfSkill[skillId]]);
  }

  this->sortItems(0, Qt::AscendingOrder);

  connect(chr, SIGNAL(skillPointsUpdated(int)),
          this, SLOT(updateSkillPoints(int)));
  connect(chr, SIGNAL(skillDeleted(int)),
          this, SLOT(deleteSkillItem(int)));
  connect(chr, SIGNAL(skillAdded(int)),
          this, SLOT(addSkillItem(int)));
}

void CharacterSkillTree::contextMenuEvent(QContextMenuEvent *e) {
  QTreeWidgetItem* item = itemAt(e->pos());
  if (skillOfItem.contains(item)) {
    QMenu* menu = new QMenu();
    QAction* setSkillPointsAction = menu->addAction(tr("Set Skill Points..."));
    connect(setSkillPointsAction, SIGNAL(triggered(bool)),
            this, SLOT(setSkillPointsActionTriggered()));
    QAction* deleteSkillAction = menu->addAction(tr("Delete"));
    connect(deleteSkillAction, SIGNAL(triggered()),
            this, SLOT(deleteSkillActionTriggered()));
    menu->exec(viewport()->mapToGlobal(e->pos()));
  }
}

void CharacterSkillTree::dragEnterEvent(QDragEnterEvent* e) {
  if (e->mimeData()->hasFormat("eveop/type-variant")) {
    int typeId = TypeVariant::fromMimeData(e->mimeData())[0].toTypeId();
    QSqlQuery* categoryQuery = Queries::getCategoryOfTypeQuery();
    categoryQuery->bindValue(":id", typeId);
    categoryQuery->exec();
    categoryQuery->next();
    int categoryId = categoryQuery->value(0).toInt();
    if (categoryId == 16)
      e->acceptProposedAction();
  } else if (e->mimeData()->hasFormat("eveop/skill-training-unit")) {
    e->acceptProposedAction();
  }
}

void CharacterSkillTree::dragMoveEvent(QDragMoveEvent* e) {
  e->acceptProposedAction();
}

void CharacterSkillTree::dropEvent(QDropEvent* e) {
  if (e->mimeData()->hasFormat("eveop/type-variant")) {
    int typeId = TypeVariant::fromMimeData(e->mimeData())[0].toTypeId();
    e->acceptProposedAction();
    if (!chr->skills.contains(typeId)) {
      chr->addSkill(typeId);
      chr->save();
    }
  } else if (e->mimeData()->hasFormat("eveop/skill-training-unit")) {
    e->acceptProposedAction();
  }
}

QStringList CharacterSkillTree::mimeTypes() const {
  QStringList types;
  types << "eveop/type-variant"
        << "text/plain";
  return types;
}

QMimeData* CharacterSkillTree::mimeData(const QList<QTreeWidgetItem*> items) const {
  QList<TypeVariant> typeVariants;
  for (QListIterator<QTreeWidgetItem*> i(items); i.hasNext();) {
    QTreeWidgetItem* item = i.next();
    if (skillOfItem.contains(item))
      typeVariants << skillOfItem[item];
  }
  if (typeVariants.isEmpty())
    return NULL;
  return listOfTypeVariantToMimeData(typeVariants);
}

void CharacterSkillTree::updateSkillLevelDisplay(int skillId) {
  QTreeWidgetItem* item = itemOfSkill[skillId];
  CharacterSkillTreeItemSkillLevelWidget* widget =
      qobject_cast<CharacterSkillTreeItemSkillLevelWidget*>(itemWidget(item, 1));
  widget->updateSkillLevel();
}

void CharacterSkillTree::setSkillPointsActionTriggered() {
  if (!selectedItems().isEmpty()) {
    QTreeWidgetItem* item = selectedItems()[0];
    if (skillOfItem.contains(item)) {
      int skillId = skillOfItem[item];
      QSqlQuery* skillMultiplierQuery = Queries::getSkillMultiplierQuery();
      skillMultiplierQuery->bindValue(":id", skillId);
      skillMultiplierQuery->exec();
      skillMultiplierQuery->next();
      double skillMultiplier = skillMultiplierQuery->value(0).toDouble();
      double currentSkillPoints = chr->skills[skillId];
      bool ok;
      int newSkillPoints = QInputDialog::getInt(this, tr("Set Skill Points"), tr("Skill Points:"),
                                                (int)ceil(currentSkillPoints), 0,
                                                (int)ceil(BASE_SKILL_POINTS[5] * skillMultiplier),
                                                1, &ok);
      if (ok) {
        chr->setSkillPoints(skillId, newSkillPoints);
        chr->save();
      }
    }
  }
}

void CharacterSkillTree::deleteSkillActionTriggered() {
  if (!selectedItems().isEmpty()) {
    QTreeWidgetItem* item = selectedItems()[0];
    if (skillOfItem.contains(item)) {
      int skillId = skillOfItem[item];
      chr->deleteSkill(skillId);
      chr->save();
    }
  }
}

void CharacterSkillTree::updateSkillPoints(int skillId) {
  CharacterSkillTreeItemNameAndSkillPointsWidget* widget =
      qobject_cast<CharacterSkillTreeItemNameAndSkillPointsWidget*>(itemWidget(itemOfSkill[skillId], 0));
  widget->updateSkillPoints();
}

void CharacterSkillTree::deleteSkillItem(int skillId) {
  QTreeWidgetItem* item = itemOfSkill[skillId];
  QTreeWidgetItem* parent = item->parent();
  itemOfSkill.remove(skillId);
  skillOfItem.remove(item);
  delete item;
  if (!parent->childCount()) {
    int groupId = groupOfItem[parent];
    groupOfItem.remove(parent);
    itemOfGroup.remove(groupId);
    delete parent;
  }
}

void CharacterSkillTree::addSkillItem(int skillId) {
  QSqlQuery* groupOfSkillQuery = Queries::getGroupOfTypeQuery();
  groupOfSkillQuery->bindValue(":id", skillId);
  groupOfSkillQuery->exec();
  groupOfSkillQuery->next();
  int groupId = groupOfSkillQuery->value(0).toInt();

  if (!itemOfGroup.contains(groupId)) {
    QSqlQuery* groupNameQuery = Queries::getGroupNameQuery();
    groupNameQuery->bindValue(":id", groupId);
    groupNameQuery->exec();
    groupNameQuery->next();
    QString groupName = groupNameQuery->value(0).toString();
    addGroupItem(groupId, groupName);
  }

  addSkillItem(skillId, itemOfGroup[groupId]);
}

void CharacterSkillTree::addSkillItem(int skillId, QTreeWidgetItem* parent) {
  CharacterSkillTreeItemNameAndSkillPointsWidget* nameAndSkillWidget =
      new CharacterSkillTreeItemNameAndSkillPointsWidget();
  nameAndSkillWidget->init(skillId, chr);
  CharacterSkillTreeItemSkillLevelWidget* skillLevelWidget =
      new CharacterSkillTreeItemSkillLevelWidget();
  skillLevelWidget->init(skillId, chr);
  QTreeWidgetItem* item =
      new QTreeWidgetItem(parent);
  setItemWidget(item, 0, nameAndSkillWidget);
  setItemWidget(item, 1, skillLevelWidget);
  skillOfItem[item] = skillId;
  itemOfSkill[skillId] = item;
}

void CharacterSkillTree::addGroupItem(int groupId, QString groupName) {
  QTreeWidgetItem* item = new QTreeWidgetItem(this, QStringList(groupName));
  groupOfItem[item] = groupId;
  itemOfGroup[groupId] = item;
}
