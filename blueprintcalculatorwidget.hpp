#ifndef BLUEPRINTCALCULATORWIDGET_HPP
#define BLUEPRINTCALCULATORWIDGET_HPP

#include <QWidget>

#include <QMap>
#include <QLocale>

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
private:
  QMap<int, int> getBasicMaterials() const;
  QMap<int, int> getExtraMaterials() const;
  double getBasicMaterialsCost() const;
  double getExtraMaterialsCost() const;
  void updateBasicMaterialsCost();
  void updateExtraMaterialsCost();
  void updateTotalMaterialsCost();
  void requestPrices();
  void fillTables();
  void updateBasicMaterialItem(int typeId);
  void updateExtraMaterialItem(int typeId);
  QStringList getStringListForMaterial(int materialTypeId, int quantity, bool withWaste);
  int getMeRequiredForOptimalMaterial(int materialQuantity) const;
  int getQuantityWithWaste(int quantity, int me) const;
private:
  int blueprintId;
  int productId;
  int portionSize;
  QMap<int, int> basicMaterials;
  QMap<int, int> extraMaterials;
  QMap<int, QTreeWidgetItem*> itemOfBasicMaterial;
  QMap<int, QTreeWidgetItem*> itemOfExtraMaterial;
  QLocale locale;
private:
  Ui::BlueprintCalculatorWidget *ui;
};

#endif // BLUEPRINTCALCULATORWIDGET_HPP
