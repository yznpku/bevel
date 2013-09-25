#include "item_index_dock.hpp"

#include "typetree.hpp"

ItemIndexDock::ItemIndexDock(QWidget* parent)
  : QDockWidget(tr("Item Index"), parent) {
  tree = new TypeTree(this);
  tree->setColumns({TypeTree::PriceColumn,
                    TypeTree::UpdatePriceButtonColumn,
                    TypeTree::InfoButtonColumn});
  tree->initWithMarketGroups();
  setWidget(tree);  
}
