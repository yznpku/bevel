#ifndef _SKILL_REQUIREMENT_TREE_HPP_
#define _SKILL_REQUIREMENT_TREE_HPP_

#include <QTreeWidget>
#include "attribute_set.hpp"

class QTreeWidgetItem;

class SkillRequirementTree : public QTreeWidget {
public:
  SkillRequirementTree(int typeId);
private:
  void addSkillRequirementFor(QTreeWidgetItem* item);
  AttributeSet as;
  QMap<QTreeWidgetItem*, int> typeOfItem;
};

#endif
