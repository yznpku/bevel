#include "queries.hpp"

#include <QMap>
#include <QSqlQuery>
#include <QSqlDatabase>

QMap<Queries::QueryType, QSqlQuery*>* queries = 0;

void Queries::initQueries()
{
  queries = new QMap<QueryType, QSqlQuery*>();
  registerQuery(SkillMultiplierQuery, "static",
                "SELECT valueFloat "
                "FROM dgmTypeAttributes "
                "WHERE typeId = :id "
                "AND attributeId = 275");
  registerQuery(TypeNameQuery, "static",
                "SELECT typeName "
                "FROM invTypes "
                "WHERE typeId = :id");
  registerQuery(GroupNameQuery, "static",
                "SELECT groupName "
                "FROM invGroups "
                "WHERE groupId = :id");
  registerQuery(GroupOfTypeQuery, "static",
                "SELECT groupId "
                "FROM invTypes "
                "WHERE typeId = :id");
  registerQuery(CategoryOfTypeQuery, "static",
                "SELECT categoryId "
                "FROM ( "
                "  SELECT groupId "
                "  FROM invTypes "
                "  WHERE typeId = :id "
                "  ) JOIN invGroups "
                "  USING (groupId)");
  registerQuery(StationNameQuery, "static",
                "SELECT stationName "
                "FROM staStations "
                "WHERE stationId = :id");
  registerQuery(MarketGroupNameQuery, "static",
                "SELECT marketGroupName "
                "FROM invMarketGroups "
                "WHERE marketGroupId = :id");
  registerQuery(BlueprintForProductQuery, "static",
                "SELECT blueprintTypeId "
                "FROM invBlueprintTypes "
                "WHERE productTypeId = :id");
  registerQuery(ProductForBlueprintQuery, "static",
                "SELECT productTypeId "
                "FROM invBlueprintTypes "
                "WHERE blueprintTypeId = :id");
  registerQuery(TypePortionSizeQuery, "static",
                "SELECT portionSize "
                "FROM invTypes "
                "WHERE typeId = :id");
  registerQuery(BasicMaterialsQuery, "static",
                "SELECT materialTypeId, quantity "
                "FROM invTypeMaterials "
                "WHERE typeId = :id");
  registerQuery(ExtraMaterialsQuery, "static",
                "SELECT requiredTypeId, quantity "
                "FROM ramTypeRequirements "
                "WHERE typeId = :id "
                "  AND activityId = 1");
  registerQuery(UnitDisplayNameQuery, "static",
                "SELECT displayName "
                "FROM eveUnits "
                "WHERE unitId = :id");
  registerQuery(MetaGroupOfTypeQuery, "static",
                "SELECT metaGroupId "
                "FROM invMetaTypes "
                "WHERE typeId = :id");
  registerQuery(MetaGroupNameQuery, "static",
                "SELECT metaGroupName "
                "FROM invMetaGroups "
                "WHERE metaGroupId = :id");
  registerQuery(MetaLevelQuery, "static",
                "SELECT valueInt, valueFloat "
                "FROM dgmTypeAttributes "
                "WHERE typeId = :id "
                "  AND attributeId = 633");

  registerQuery(LastInsertRowidQuery, "user",
                "SELECT last_insert_rowid()");

  registerQuery(MarketPriceQuery, "market",
                "SELECT sellPrice, buyPrice, averagePrice, updateTime "
                "FROM prices "
                "WHERE typeId = :id");
  registerQuery(UpdateMarketPriceQuery, "market",
                "UPDATE prices "
                "SET sellPrice = :sellPrice, "
                "    buyPrice = :buyPrice, "
                "    averagePrice = :averagePrice, "
                "    updateTime = :updateTime "
                "WHERE typeId = :id");
  registerQuery(InsertMarketPriceQuery, "market",
                "INSERT INTO prices "
                "(typeId, sellPrice, buyPrice, averagePrice, updateTime) "
                "VALUES "
                "(:id, :sellPrice, :buyPrice, :averagePrice, :updateTime)");

}

void Queries::registerQuery(Queries::QueryType type, const QString& databaseName, const QString& queryString)
{
  (*queries)[type] = new QSqlQuery(QSqlDatabase::database(databaseName));
  (*queries)[type]->prepare(queryString);
}

QSqlQuery* Queries::getQuery(Queries::QueryType type)
{
  if (!queries->contains(type))
    qWarning("No such type registered.");
  return (*queries)[type];
}
