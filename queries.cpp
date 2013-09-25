#include "queries.hpp"

#include <QSqlQuery>
#include <QSqlDatabase>

QSqlQuery* lastInsertRowidQuery = 0;
QSqlQuery* Queries::getLastInsertRowidQuery() {
  if (!lastInsertRowidQuery) {
    lastInsertRowidQuery = new QSqlQuery(QSqlDatabase::database("user"));
    lastInsertRowidQuery->prepare("SELECT last_insert_rowid()");
  }
  return lastInsertRowidQuery;
}

QSqlQuery* skillMultiplierQuery = 0;
QSqlQuery* Queries::getSkillMultiplierQuery() {
  if (!skillMultiplierQuery) {
    skillMultiplierQuery = new QSqlQuery(QSqlDatabase::database("static"));
    skillMultiplierQuery->prepare("SELECT valueFloat "
                                  "FROM dgmTypeAttributes "
                                  "WHERE typeId = :id "
                                  "AND attributeId = 275");
  }
  return skillMultiplierQuery;
}

QSqlQuery* typeNameQuery = 0;
QSqlQuery* Queries::getTypeNameQuery() {
  if (!typeNameQuery) {
    typeNameQuery = new QSqlQuery(QSqlDatabase::database("static"));
    typeNameQuery->prepare("SELECT typeName "
                           "FROM invTypes "
                           "WHERE typeId = :id");
  }
  return typeNameQuery;
}

QSqlQuery* groupNameQuery = 0;
QSqlQuery* Queries::getGroupNameQuery() {
  if (!groupNameQuery) {
    groupNameQuery = new QSqlQuery(QSqlDatabase::database("static"));
    groupNameQuery->prepare("SELECT groupName "
                            "FROM invGroups "
                            "WHERE groupId = :id");
  }
  return groupNameQuery;
}

QSqlQuery* groupOfTypeQuery = 0;
QSqlQuery* Queries::getGroupOfTypeQuery() {
  if (!groupOfTypeQuery) {
    groupOfTypeQuery = new QSqlQuery(QSqlDatabase::database("static"));
    groupOfTypeQuery->prepare("SELECT groupId "
                              "FROM invTypes "
                              "WHERE typeId = :id");
  }
  return groupOfTypeQuery;
}

QSqlQuery* categoryOfTypeQuery = 0;
QSqlQuery* Queries::getCategoryOfTypeQuery() {
  if (!categoryOfTypeQuery) {
    categoryOfTypeQuery = new QSqlQuery(QSqlDatabase::database("static"));
    categoryOfTypeQuery->prepare("SELECT categoryId "
                                 "FROM ( "
                                 "  SELECT groupId "
                                 "  FROM invTypes "
                                 "  WHERE typeId = :id "
                                 "  ) JOIN invGroups "
                                 "  USING (groupId)");
  }
  return categoryOfTypeQuery;
}

QSqlQuery* stationNameQuery = 0;
QSqlQuery* Queries::getStationNameQuery()
{
  if (!stationNameQuery) {
    stationNameQuery = new QSqlQuery(QSqlDatabase::database("static"));
    stationNameQuery->prepare("SELECT stationName "
                              "FROM staStations "
                              "WHERE stationId = :id");
  }
  return stationNameQuery;
}

QSqlQuery* marketGroupNameQuery = 0;
QSqlQuery* Queries::getMarketGroupNameQuery()
{
  if (!marketGroupNameQuery) {
    marketGroupNameQuery = new QSqlQuery(QSqlDatabase::database("static"));
    marketGroupNameQuery->prepare("SELECT marketGroupName "
                                  "FROM invMarketGroups "
                                  "WHERE marketGroupId = :id");
  }
  return marketGroupNameQuery;
}

QSqlQuery* marketSubgroupsQuery = 0;
QSqlQuery* Queries::getMarketSubgroupsQuery()
{
  if (!marketSubgroupsQuery) {
    marketSubgroupsQuery = new QSqlQuery(QSqlDatabase::database("static"));
    marketSubgroupsQuery->prepare("SELECT marketGroupId "
                                  "FROM invMarketGroups "
                                  "WHERE parentGroupId = :id");
  }
  return marketSubgroupsQuery;
}

QSqlQuery* marketPriceQuery = 0;
QSqlQuery* Queries::getMarketPriceQuery()
{
  if (!marketPriceQuery) {
    marketPriceQuery = new QSqlQuery(QSqlDatabase::database("market"));
    marketPriceQuery->prepare("SELECT sellPrice, buyPrice, averagePrice, updateTime "
                              "FROM prices "
                              "WHERE typeId = :id");
  }
  return marketPriceQuery;
}

QSqlQuery* updateMarketPriceQuery = 0;
QSqlQuery* Queries::getUpdateMarketPriceQuery()
{
  if (!updateMarketPriceQuery) {
    updateMarketPriceQuery = new QSqlQuery(QSqlDatabase::database("market"));
    updateMarketPriceQuery->prepare("UPDATE prices "
                                    "SET sellPrice = :sellPrice, "
                                    "    buyPrice = :buyPrice, "
                                    "    averagePrice = :averagePrice, "
                                    "    updateTime = :updateTime "
                                    "WHERE typeId = :id");
  }
  return updateMarketPriceQuery;
}

QSqlQuery* insertMarketPriceQuery = 0;
QSqlQuery* Queries::getInsertMarketPriceQuery()
{
  if (!insertMarketPriceQuery) {
    insertMarketPriceQuery = new QSqlQuery(QSqlDatabase::database("market"));
    insertMarketPriceQuery->prepare("INSERT INTO prices "
                                    "(typeId, sellPrice, buyPrice, averagePrice, updateTime) "
                                    "VALUES "
                                    "(:id, :sellPrice, :buyPrice, :averagePrice, :updateTime)");
  }
  return insertMarketPriceQuery;
}

QSqlQuery* blueprintForProductQuery = 0;
QSqlQuery* Queries::getBlueprintForProductQuery()
{
  if (!blueprintForProductQuery) {
    blueprintForProductQuery = new QSqlQuery(QSqlDatabase::database("static"));
    blueprintForProductQuery->prepare("SELECT blueprintTypeId "
                                      "FROM invBlueprintTypes "
                                      "WHERE productTypeId = :id");
  }
  return blueprintForProductQuery;
}

QSqlQuery* productForBlueprintQuery = 0;
QSqlQuery* Queries::getProductForBlueprintQuery()
{
  if (!productForBlueprintQuery) {
    productForBlueprintQuery = new QSqlQuery(QSqlDatabase::database("static"));
    productForBlueprintQuery->prepare("SELECT productTypeId "
                                      "FROM invBlueprintTypes "
                                      "WHERE blueprintTypeId = :id");
  }
  return productForBlueprintQuery;
}

QSqlQuery* typePortionSizeQuery = 0;
QSqlQuery* Queries::getTypePortionSizeQuery()
{
  if (!typePortionSizeQuery) {
    typePortionSizeQuery = new QSqlQuery(QSqlDatabase::database("static"));
    typePortionSizeQuery->prepare("SELECT portionSize "
                                  "FROM invTypes "
                                  "WHERE typeId = :id");
  }
  return typePortionSizeQuery;
}
