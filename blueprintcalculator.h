#ifndef BLUEPRINTCALCULATOR_H
#define BLUEPRINTCALCULATOR_H

#include <QObject>

#include <QMap>

class BlueprintCalculator : public QObject
{
  Q_OBJECT
public:
  BlueprintCalculator(QObject* parent = 0);
  void init(int blueprintId);
  QMap<int, int> getBasicMaterials() const;
  QMap<int, int> getExtraMaterials() const;
  double getBasicMaterialsCost() const;
  double getExtraMaterialsCost() const;
  double getManufacturingMaterialsCost() const;
public:
  static int queryProductId(int blueprintId);
  static QMap<int, int> queryBasicMaterials(int productId);
  static QMap<int, int> queryExtraMaterials(int blueprintId);
  static double getWasteFactor(int me);
  static int getQuantityWithWaste(int quantity, int me);
  static int getMeRequiredForOptimalMaterial(int quantity);
  static int queryPortionSize(int productId);
signals:
  void basicMaterialsCostChanged(double cost);
  void extraMaterialsCostChanged(double cost);
  void manufacturingMaterialsCostChanged(double cost);
  void productSellPriceChanged(double sellPrice);
  void grossProfitChanged(double grossProfit);
public slots:
  void priceUpdated(int typeId);
  void meChanged(int me);
  void setManufacturingRuns(int runs);
private:
  void updateBasicMaterialsCost();
  void updateExtraMaterialsCost();
  void updateManufacturingMaterialsCost();
  void updateProductSellPrice();
  void updateGrossProfit();
private:
  int blueprintId;
  int productId;
  int portionSize;
  int manufacturingRuns;
  int me;
  QMap<int, int> basicMaterials;
  QMap<int, int> extraMaterials;
  double basicMaterialsCost;
  double extraMaterialsCost;
  double manufacturingMaterialsCost;
  double productSellPrice;
  double grossProfit;
};

#endif // BLUEPRINTCALCULATOR_H
