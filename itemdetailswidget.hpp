#ifndef ITEMDETAILSWIDGET_HPP
#define ITEMDETAILSWIDGET_HPP

#include <QWidget>
#include "attribute_set.hpp"

class QTableWidget;
class QTabWidget;
class QTextEdit;
class AttributeTree;
class SkillRequirementTree;
class QTreeWidgetItem;

namespace Ui {
  class ItemDetailsWidget;
}

class ItemDetailsWidget : public QWidget {
  Q_OBJECT
  
public:
  ItemDetailsWidget(int typeId, QWidget* parent = 0);
  ItemDetailsWidget(const AttributeSet& as, QWidget* parent = 0);
  ~ItemDetailsWidget();
private:
  void init();
  void initDescriptionTab();
  void initAttributeTree();
  void initSkillRequirementTree();
  void fillTabs();
private:
  Ui::ItemDetailsWidget *ui;
  int typeId;
  AttributeSet as;
  QTabWidget* tabs;
  QMap<int, QTableWidget*> tableOfCategory;
  QMap<int, QTreeWidgetItem*> itemOfCategory;
  QTextEdit* descriptionTab;
  AttributeTree* at;
  SkillRequirementTree* srt;
};

#endif // ITEMDETAILSWIDGET_HPP
