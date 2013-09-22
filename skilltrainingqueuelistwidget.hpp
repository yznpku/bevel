#ifndef SKILLTRAININGQUEUELISTWIDGET_HPP
#define SKILLTRAININGQUEUELISTWIDGET_HPP

#include <QListWidget>

#include <QList>
#include <QMap>
#include "skilltrainingunit.hpp"
#include "typevariant.hpp"

class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class QListWidgetItem;
class QMimeData;
class Character;

class SkillTrainingQueueListWidget : public QListWidget {
  Q_OBJECT
public:
  explicit SkillTrainingQueueListWidget(QWidget* parent = 0);
  void setCharacter(Character* chr);
signals:
  void skillChanged(int skillId);
protected:
  virtual void dragEnterEvent(QDragEnterEvent* e);
  virtual QStringList mimeTypes() const;
  virtual QMimeData* mimeData(const QList<QListWidgetItem*> items) const;
  virtual bool dropMimeData(int index, const QMimeData* data, Qt::DropAction action);
  virtual void startDrag(Qt::DropActions supportedActions);
private:
  int insertTrainingUnit(int index, SkillTrainingUnit unit);
  void deleteTrainingUnit(int index);
  int deleteTrainingUnit(SkillTrainingUnit unit);
  void addTypes(int index, QList<TypeVariant> types);
  bool addTrainingUnits(int index, QList<SkillTrainingUnit> units);
private:
  Character* chr;
  QMap<QListWidgetItem*, SkillTrainingUnit> unitOfItem;
  QMap<SkillTrainingUnit, QListWidgetItem*> itemOfUnit;
};

#endif // SKILLTRAININGQUEUELISTWIDGET_HPP
