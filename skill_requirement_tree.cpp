#include "skill_requirement_tree.hpp"

#include <QHeaderView>
#include <QTreeWidgetItem>

const int requiredSkillIdAttributeId[] =    {182, 183, 184};
const int requiredSkillLevelAttributeId[] = {277, 278, 279};
const QString skillLevelSuffix[] = {"", "I", "II", "III", "IV", "V"};

SkillRequirementTree::SkillRequirementTree(int typeId) {
  as = AttributeSet::fromPrototype(typeId);
  setColumnCount(2);
  setHeaderItem(new QTreeWidgetItem(QStringList({tr("Name"), ""})));
  header()->setStretchLastSection(false);
  header()->setSectionResizeMode(0, QHeaderView::Stretch);
  header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  QTreeWidgetItem* root = new QTreeWidgetItem(this, QStringList({as.name, ""}));
  typeOfItem[root] = typeId;
  addSkillRequirementFor(root);
  expandAll();
}

void SkillRequirementTree::addSkillRequirementFor(QTreeWidgetItem* item) {
  int typeId = typeOfItem[item];
  AttributeSet itemAs = AttributeSet::fromPrototype(typeId);
  for (int i = 0; i < 3; i++) {
    if (itemAs.attr.contains(requiredSkillIdAttributeId[i])) {
      int requiredSkillId = itemAs.attr[requiredSkillIdAttributeId[i]];
      int requiredSkillLevel = itemAs.attr[requiredSkillLevelAttributeId[i]];
      AttributeSet requiredSkillAs = AttributeSet::fromPrototype(requiredSkillId);
      QString skillNameWithSuffix = requiredSkillAs.name + ' '
        + skillLevelSuffix[requiredSkillLevel];
      QTreeWidgetItem* child = new QTreeWidgetItem(item,
                                                   QStringList({skillNameWithSuffix, ""}));
      typeOfItem[child] = requiredSkillId;
      addSkillRequirementFor(child);
    }
  }
}
