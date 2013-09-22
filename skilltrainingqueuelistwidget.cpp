#include "skilltrainingqueuelistwidget.hpp"

#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QListWidgetItem>
#include <QListIterator>
#include <QSqlQuery>
#include <QDebug>
#include "character.hpp"
#include "global.hpp"
#include "skilltrainingqueuelistitemwidget.hpp"
#include "queries.hpp"

SkillTrainingQueueListWidget::SkillTrainingQueueListWidget(QWidget* parent) :
  QListWidget(parent) {
  this->viewport()->setAcceptDrops(true);
  this->setDropIndicatorShown(true);
  this->setDragEnabled(true);
}

void SkillTrainingQueueListWidget::setCharacter(Character* chr) {
  this->chr = chr;
}

void SkillTrainingQueueListWidget::dragEnterEvent(QDragEnterEvent* e) {
  bool willAccept = false;
  if (e->mimeData()->hasFormat("eveop/skill-training-unit"))
    willAccept = true;
  if (e->mimeData()->hasFormat("eveop/type-variant")) {
    QList<TypeVariant> tvl = TypeVariant::fromMimeData(e->mimeData());
    QSqlQuery* categoryQuery = Queries::getCategoryOfTypeQuery();
    categoryQuery->bindValue(":id", tvl[0].toTypeId());
    categoryQuery->exec();
    categoryQuery->next();
    int categoryId = categoryQuery->value(0).toInt();
    if (categoryId == 16)
      willAccept = true;
  }
  if (willAccept)
    QListWidget::dragEnterEvent(e);
}

QStringList SkillTrainingQueueListWidget::mimeTypes() const {
  QStringList types;
  types << "eveop/type-variant"
        << "eveop/skill-training-unit";
  return types;
}

QMimeData* SkillTrainingQueueListWidget::mimeData(const QList<QListWidgetItem*> items) const {
  QList<SkillTrainingUnit> units;
  for (QListIterator<QListWidgetItem*> i(items); i.hasNext();) {
    QListWidgetItem* item = i.next();
    units << unitOfItem[item];
  }
  if (units.isEmpty())
    return 0;
  return listOfSkillTrainingUnitToMimeData(units);
}

bool SkillTrainingQueueListWidget::dropMimeData(int index, const QMimeData* data, Qt::DropAction action) {
  Q_UNUSED(action);
  clearSelection();
  if (data->hasFormat("eveop/type-variant")) {
    QList<TypeVariant> tvl = TypeVariant::fromMimeData(data);
    addTypes(index, tvl);
  } else if (data->hasFormat("eveop/skill-training-unit")) {
    QList<SkillTrainingUnit> stul = SkillTrainingUnit::fromMimeData(data);
    return addTrainingUnits(index, stul);
  }
  return true;
}

void SkillTrainingQueueListWidget::startDrag(Qt::DropActions supportedActions) {
  if (selectedItems().isEmpty()) {
    QAbstractItemView::startDrag(supportedActions);
    return;
  }
  QListWidgetItem* item = selectedItems()[0];
  QDrag* drag = new QDrag(this);
  drag->setMimeData(mimeData(QList<QListWidgetItem*>({item})));
  drag->setPixmap(*getTypePixmap64(unitOfItem[item].skillId));
  if (drag->exec(supportedActions, Qt::CopyAction)) {
    if (unitOfItem.contains(item))
      deleteTrainingUnit(unitOfItem[item]);
  }
}

// Insert 'unit' before 'index', removing duplicates in the queue.
// The index where 'unit' is finally inserted is returned.
// Note: this method assumes 'unit' has exactly one level higher than
//       current skill level or another unit before 'index'.
int SkillTrainingQueueListWidget::insertTrainingUnit(int index, SkillTrainingUnit unit) {
  int occurenceIndex = deleteTrainingUnit(unit);
  if (occurenceIndex != -1 && occurenceIndex < index)
    --index;
  chr->skillTrainingQueue.insert(index, unit);

  SkillTrainingQueueListItemWidget* itemWidget = new SkillTrainingQueueListItemWidget();
  itemWidget->init(unit, chr);
  QListWidgetItem* item = new QListWidgetItem();
  item->setSizeHint(itemWidget->sizeHint());
  this->insertItem(index, item);
  this->setItemWidget(item, itemWidget);
  unitOfItem[item] = unit;
  itemOfUnit[unit] = item;

  emit skillChanged(unit.skillId);
  return index;
}

void SkillTrainingQueueListWidget::deleteTrainingUnit(int index) {
  SkillTrainingUnit unit = chr->skillTrainingQueue[index];
  QListWidgetItem* item = itemOfUnit[unit];
  delete item;
  itemOfUnit.remove(unit);
  unitOfItem.remove(item);
  chr->skillTrainingQueue.removeAt(index);
  emit skillChanged(unit.skillId);
}

void SkillTrainingQueueListWidget::addTypes(int index, QList<TypeVariant> types) {
  QSqlQuery* categoryQuery = Queries::getCategoryOfTypeQuery();
  for (QListIterator<TypeVariant> i(types); i.hasNext();) { // for each type in the list
    int skillId = i.next().toTypeId();
    categoryQuery->bindValue(":id", skillId);
    categoryQuery->exec();
    categoryQuery->next();
    int categoryId = categoryQuery->value(0).toInt();
    if (categoryId == 16) {    // make sure the type is a skill
      int currentLevel = chr->getSkillLevel(skillId) + 1;
      if (currentLevel == 0)  // the character doesn't have this skill
        continue;
      for (int j = index - 1; j >= 0; j--) {
        SkillTrainingUnit unit = unitOfItem[item(j)];
        if (unit.skillId == skillId) {
          currentLevel = unit.level + 1;
          break;
        }
      }
      if (currentLevel == 6)  // can't train this skill any further
        continue;
      insertTrainingUnit(index, SkillTrainingUnit(skillId, currentLevel));
      ++index;
    }
  }
}

bool SkillTrainingQueueListWidget::addTrainingUnits(int index, QList<SkillTrainingUnit> units) {
  if (units.isEmpty())
    return false;
  SkillTrainingUnit unit = units[0];
  int previousLevel = chr->getSkillLevel(unit.skillId);
  for (int previousIndex = index - 1; previousIndex >= 0; --previousIndex)
    if (chr->skillTrainingQueue[previousIndex].skillId == unit.skillId) {
      previousLevel = chr->skillTrainingQueue[previousIndex].level;
      break;
    }
  if (previousLevel == unit.level || previousLevel == unit.level - 1) {
    index = insertTrainingUnit(index, unit) + 1;
    return true;
  }
  return false;
}

// Deletes the SkillTrainingUnit equal to 'unit', and returns the index of the unit deleted.
// Returns -1 if 'unit' does not exist.
int SkillTrainingQueueListWidget::deleteTrainingUnit(SkillTrainingUnit unit) {
  int index = chr->skillTrainingQueue.indexOf(unit);
  if (index != -1)
    deleteTrainingUnit(index);
  return index;
}
