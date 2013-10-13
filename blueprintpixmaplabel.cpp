#include "blueprintpixmaplabel.hpp"

#include "queries.hpp"

BlueprintPixmapLabel::BlueprintPixmapLabel(QWidget* parent)
  : TypePixmapLabel(parent)
{

}

int BlueprintPixmapLabel::filterTypeId(int typeId) const
{
  QSqlQuery* categoryQuery = Queries::getQuery(Queries::CategoryOfTypeQuery);
  categoryQuery->bindValue(":id", typeId);
  categoryQuery->exec();
  categoryQuery->next();
  int categoryId = categoryQuery->value(0).toInt();
  if (categoryId == 9) // This is a blueprint
    return typeId;
  QSqlQuery* blueprintQuery = Queries::getQuery(Queries::BlueprintForProductQuery);
  blueprintQuery->bindValue(":id", typeId);
  blueprintQuery->exec();
  if (!blueprintQuery->next())
    return -1;
  int blueprintTypeId = blueprintQuery->value(0).toInt();
  return blueprintTypeId;
}
