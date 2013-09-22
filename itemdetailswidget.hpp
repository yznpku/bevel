#ifndef ITEMDETAILSWIDGET_HPP
#define ITEMDETAILSWIDGET_HPP

#include <QWidget>
#include <attribute_set.hpp>

class QTableWidget;
class QTabWidget;
class QTextEdit;
class SkillRequirementTree;

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
  void fillTables();
  void fillSkillRequirementTree();
  void fillTabs();
private:
  Ui::ItemDetailsWidget *ui;
  int typeId;
  AttributeSet as;
  QTabWidget* tabs;
  QMap<int, QTableWidget*> tableOfCategory;
  QTextEdit* descriptionTab;
  SkillRequirementTree* srt;
};

#endif // ITEMDETAILSWIDGET_HPP
