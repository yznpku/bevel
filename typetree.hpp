#ifndef _TYPE_TREE_HPP_
#define _TYPE_TREE_HPP_

#include <QTreeWidget>

#include <QModelIndex>

class QContextMenuEvent;
class QTreeWidgetItem;

class TypeTree : public QTreeWidget {
  Q_OBJECT
public:
  enum Column {InfoButtonColumn, PriceColumn, UpdatePriceButtonColumn};
public:
  TypeTree(QWidget* parent = 0);
  void setColumns(const QList<Column>& columns);
  void initWithDefaultGroups();
  void initWithMarketGroups();
protected:
  virtual void contextMenuEvent(QContextMenuEvent* e);
  virtual QStringList mimeTypes() const;
  virtual QMimeData* mimeData(const QList<QTreeWidgetItem*> items) const;
  virtual void startDrag(Qt::DropActions supportedActions);
private slots:
  void typeViewDetailsClicked();
  void infoButtonClicked(const QModelIndex& index);
  void updatePriceButtonClicked(const QModelIndex& index);
  void priceUpdated(int typeId);
private:
  QStringList getStringListForType(int typeId);
private:
  QLocale locale;
  QList<Column> columns;
  QMap<int, QTreeWidgetItem*> itemOfType;
  QMap<QTreeWidgetItem*, int> typeOfItem;
};

#endif
