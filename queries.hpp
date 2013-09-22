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

  QSqlQuery* getLastInsertRowidQuery();

  QSqlQuery* getMarketPriceQuery();
}

#endif // QUERIES_HPP
