#include "blueprintcalculator.h"

#include "queries.hpp"
#include "global.hpp"
#include "market.hpp"

BlueprintCalculator::BlueprintCalculator(QObject* parent) :
  QObject(parent)
{
  manufacturingRuns = 1;
  connect(market, SIGNAL(priceUpdated(int)),
          this, SLOT(priceUpdated(int)));
}

void BlueprintCalculator::init(int blueprintId)
{
  this->blueprintId = blueprintId;
  this->productId = queryProductId(blueprintId);
  basicMaterials = queryBasicMaterials(productId);
  extraMaterials = queryExtraMaterials(blueprintId);
  portionSize = queryPortionSize(productId);

  updateBasicMaterialsCost();
  updateExtraMaterialsCost();
  updateManufacturingMaterialsCost();
  updateProductSellPrice();
  updateGrossProfit();
}

QMap<int, int> BlueprintCalculator::getBasicMaterials() const
{
  return basicMaterials;
}

QMap<int, int> BlueprintCalculator::getExtraMaterials() const
{
  return extraMaterials;
}

double BlueprintCalculator::getBasicMaterialsCost() const
{
  return basicMaterialsCost;
}

double BlueprintCalculator::getExtraMaterialsCost() const
{
  return extraMaterialsCost;
}

double BlueprintCalculator::getManufacturingMaterialsCost() const
{
  return manufacturingMaterialsCost;
}

int BlueprintCalculator::queryProductId(int blueprintId)
{
  QSqlQuery* productQuery = Queries::getQuery(Queries::ProductForBlueprintQuery);
  productQuery->bindValue(":id", blueprintId);
  productQuery->exec();
  productQuery->first();
  return productQuery->value(0).toInt();
}

QMap<int, int> BlueprintCalculator::queryBasicMaterials(int productId)
{
  QMap<int, int> basicMaterials;
  QSqlQuery* basicMaterialsQuery = Queries::getQuery(Queries::BasicMaterialsQuery);
  basicMaterialsQuery->bindValue(":id", productId);
  basicMaterialsQuery->exec();
  while (basicMaterialsQuery->next()) {
    int materialTypeId = basicMaterialsQuery->value(0).toInt();
    int quantity = basicMaterialsQuery->value(1).toInt();
    basicMaterials[materialTypeId] = quantity;
  }
  return basicMaterials;
}

QMap<int, int> BlueprintCalculator::queryExtraMaterials(int blueprintId)
{
  QMap<int, int> extraMaterials;
  QSqlQuery* extraMaterialsQuery = Queries::getQuery(Queries::ExtraMaterialsQuery);
  extraMaterialsQuery->bindValue(":id", blueprintId);
  extraMaterialsQuery->exec();
  while (extraMaterialsQuery->next()) {
    int materialTypeId = extraMaterialsQuery->value(0).toInt();
    int quantity = extraMaterialsQuery->value(1).toInt();
    QSqlQuery* categoryQuery = Queries::getQuery(Queries::CategoryOfTypeQuery);
    categoryQuery->bindValue(":id", materialTypeId);
    categoryQuery->exec();
    categoryQuery->next();
    if (categoryQuery->value(0).toInt() == 16) // Do not treat skills as materials
      continue;
    extraMaterials[materialTypeId] = quantity;
  }
  return extraMaterials;
}

double BlueprintCalculator::getWasteFactor(int me)
{
  double wasteFactor;
  if (me < 0)
    wasteFactor = 0.1 * (1 - me);
  else
    wasteFactor = 0.1 / (1 + me);
  return wasteFactor;
}

int BlueprintCalculator::getQuantityWithWaste(int quantity, int me)
{
  return qRound(quantity * (1 + getWasteFactor(me)));
}

int BlueprintCalculator::getMeRequiredForOptimalMaterial(int quantity)
{
  return int(0.2 * quantity);
}

int BlueprintCalculator::queryPortionSize(int productId)
{
  QSqlQuery* portionSizeQuery = Queries::getQuery(Queries::TypePortionSizeQuery);
  portionSizeQuery->bindValue(":id", productId);
  portionSizeQuery->exec();
  portionSizeQuery->next();
  return portionSizeQuery->value(0).toInt();
}

void BlueprintCalculator::priceUpdated(int typeId)
{
  if (basicMaterials.contains(typeId))
    updateBasicMaterialsCost();
  if (extraMaterials.contains(typeId))
    updateExtraMaterialsCost();
  if (basicMaterials.contains(typeId) || extraMaterials.contains(typeId))
    updateManufacturingMaterialsCost();
  if (productId == typeId)
    updateProductSellPrice();
  if (basicMaterials.contains(typeId) || extraMaterials.contains(typeId) || productId == typeId)
    updateGrossProfit();
}

void BlueprintCalculator::setManufacturingRuns(int runs)
{
  this->manufacturingRuns = runs;
  updateBasicMaterialsCost();
  updateExtraMaterialsCost();
  updateManufacturingMaterialsCost();
  updateProductSellPrice();
  updateGrossProfit();
}

void BlueprintCalculator::updateBasicMaterialsCost()
{
  double sum = 0.0;
  for (QMapIterator<int, int> i(basicMaterials); i.hasNext();) {
    i.next();
    double price = market->getSellPrice(i.key());
    sum += price * getQuantityWithWaste(i.value(), 0);
  }
  basicMaterialsCost = sum * manufacturingRuns;
  emit basicMaterialsCostChanged(basicMaterialsCost);
}

void BlueprintCalculator::updateExtraMaterialsCost()
{
  double sum = 0.0;
  for (QMapIterator<int, int> i(extraMaterials); i.hasNext();) {
    i.next();
    double price = market->getSellPrice(i.key());
    sum += price * i.value();
  }
  extraMaterialsCost = sum * manufacturingRuns;
  emit extraMaterialsCostChanged(extraMaterialsCost);
}

void BlueprintCalculator::updateManufacturingMaterialsCost()
{
  manufacturingMaterialsCost = basicMaterialsCost + extraMaterialsCost;
  emit manufacturingMaterialsCostChanged(manufacturingMaterialsCost);
}

void BlueprintCalculator::updateProductSellPrice()
{
  productSellPrice = market->getSellPrice(productId) * portionSize * manufacturingRuns;
  emit productSellPriceChanged(productSellPrice);
}

void BlueprintCalculator::updateGrossProfit()
{
  grossProfit = productSellPrice - manufacturingMaterialsCost;
  emit grossProfitChanged(grossProfit);
}
