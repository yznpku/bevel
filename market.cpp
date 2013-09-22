#include "market.hpp"

#include "queries.hpp"
#include <cmath>

Market::Market(QObject *parent) :
  QObject(parent)
{
}

double Market::getPrice(int typeId)
{
  QSqlQuery* marketPriceQuery = Queries::getMarketPriceQuery();
  marketPriceQuery->bindValue(":id", typeId);
  marketPriceQuery->exec();

  if (!marketPriceQuery->next())
    return std::nan();

  double price = marketPriceQuery->value(0).toDouble();
  return price;
}
