#ifndef BLUEPRINTCALCULATORWIDGET_HPP
#define BLUEPRINTCALCULATORWIDGET_HPP

#include <QWidget>

#include <QMap>
#include <QLocale>
#include "blueprintcalculator.h"

namespace Ui {
  class BlueprintCalculatorWidget;
}

class QTreeWidgetItem;

class BlueprintCalculatorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BlueprintCalculatorWidget(QWidget *parent = 0);
  ~BlueprintCalculatorWidget();
private slots:
  void blueprintDropped(int blueprintId);
  void priceUpdated(int typeId);
  void manufacturingRunsChanged(int manufacturingRuns);
  void meChanged(int me);
  void basicMaterialInfoButtonClicked(const QModelIndex& index);
  void extraMaterialInfoButtonClicked(const QModelIndex& index);
  void updateBasicMaterialsCost(double cost);
  void updateExtraMaterialsCost(double cost);
  void updateManufacturingMaterialsCost(double cost);
  void updateProductSellPrice(double sellPrice);
  void updateGrossProfit(double grossProfit);
private:
  void requestPrices();
  void fillTables();
  void updateBasicMaterialItem(int typeId);
  void updateExtraMaterialItem(int typeId);
  void updateManufacturingMaterialItems();
  QStringList getStringListForMaterial(int materialTypeId, int quantity, bool withWaste);
private:
  BlueprintCalculator* calculator;
  int blueprintId;
  int productId;
  int portionSize;
  int manufacturingRuns;
  int me;
  QMap<int, int> basicMaterials;
  QMap<int, int> extraMaterials;
  QMap<int, QTreeWidgetItem*> itemOfBasicMaterial;
  QMap<int, QTreeWidgetItem*> itemOfExtraMaterial;
  QMap<QTreeWidgetItem*, int> materialOfItem;
  QLocale locale;
private:
  Ui::BlueprintCalculatorWidget *ui;
};

#endif // BLUEPRINTCALCULATORWIDGET_HPP
