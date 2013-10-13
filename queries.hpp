#ifndef QUERIES_HPP
#define QUERIES_HPP

#include <QString>

#include <QSqlQuery>
#include <QVariant>

namespace Queries {
  enum QueryType {
    SkillMultiplierQuery,
    TypeNameQuery,
    GroupNameQuery,
    MarketGroupNameQuery,
    GroupOfTypeQuery,
    CategoryOfTypeQuery,
    StationNameQuery,
    BlueprintForProductQuery,
    ProductForBlueprintQuery,
    TypePortionSizeQuery,
    BasicMaterialsQuery,
    ExtraMaterialsQuery,
    UnitDisplayNameQuery,

    LastInsertRowidQuery,

    MarketPriceQuery,
    UpdateMarketPriceQuery,
    InsertMarketPriceQuery
  };

  void initQueries();
  void registerQuery(QueryType type, const QString& databaseName, const QString& queryString);
  QSqlQuery* getQuery(QueryType type);
}

#endif // QUERIES_HPP
