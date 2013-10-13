#ifndef QUERIES_HPP
#define QUERIES_HPP

#include <QString>

#include <QSqlQuery>
#include <QVariant>

namespace Queries {
  QSqlQuery* getSkillMultiplierQuery();
  QSqlQuery* getTypeNameQuery();
  QSqlQuery* getGroupNameQuery();
  QSqlQuery* getMarketGroupNameQuery();
  QSqlQuery* getMarketSubgroupsQuery();
  QSqlQuery* getGroupOfTypeQuery();
  QSqlQuery* getCategoryOfTypeQuery();
  QSqlQuery* getStationNameQuery();
  QSqlQuery* getBlueprintForProductQuery();
  QSqlQuery* getProductForBlueprintQuery();
  QSqlQuery* getTypePortionSizeQuery();
  QSqlQuery* getBasicMaterialsQuery();
  QSqlQuery* getExtraMaterialsQuery();
  QSqlQuery* getUnitDisplayNameQuery();

  QSqlQuery* getLastInsertRowidQuery();

  QSqlQuery* getMarketPriceQuery();
  QSqlQuery* getUpdateMarketPriceQuery();
  QSqlQuery* getInsertMarketPriceQuery();
}

#endif // QUERIES_HPP
