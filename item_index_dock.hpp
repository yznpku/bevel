#ifndef _ITEM_INDEX_DOCK_HPP_
#define _ITEM_INDEX_DOCK_HPP_

#include <QDockWidget>

class QTreeWidget;
class TypeTree;

class ItemIndexDock : public QDockWidget {
public:
  ItemIndexDock(QWidget* parent = 0);
private:
  TypeTree* tree;
};

#endif
