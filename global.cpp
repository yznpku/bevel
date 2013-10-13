#include "global.hpp"

#include <QApplication>
#include <QMap>
#include <QMimeData>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "character.hpp"

QString decamelize(const QString& str) {
  QString result;
  if (!str.size())
    return result;
  result.append(str.at(0).toUpper());
  for (int i = 1; i < str.size(); i++) {
    if (str.at(i).isUpper())
      result.append(' ');
    result.append(str.at(i));
  }
  return result;
}

bool makeSureUserDatabaseAvailable() {
  QSqlDatabase userDb = QSqlDatabase::database("user");
  QSqlQuery tableQuery(userDb);
  tableQuery.prepare("SELECT * "
                     "FROM sqlite_master "
                     "WHERE type = 'table' "
                     "  AND name = :name");
  tableQuery.bindValue(":name", "characters");
  tableQuery.exec();
  if (!tableQuery.next()) {
    QSqlQuery createTableQuery(userDb);
    createTableQuery.exec("CREATE TABLE characters ( \n"
                          "  characterId  INTEGER PRIMARY KEY, \n"
                          "  name         VARCHAR(50), \n"
                          "  charisma     INTEGER, \n"
                          "  intelligence INTEGER, \n"
                          "  memory       INTEGER, \n"
                          "  perception   INTEGER, \n"
                          "  willpower    INTEGER, \n"
                          "  skills \n"
                          ")");
    Character noSkillsCharacter;
    noSkillsCharacter.as.name = QObject::tr("No Skills");
    noSkillsCharacter.create();

    Character allSkillsVCharacter;
    allSkillsVCharacter.as.name = QObject::tr("All Skills V");
    QSqlQuery skillQuery(QSqlDatabase::database("static"));
    skillQuery.exec("SELECT typeId, valueFloat "
                    "FROM "
                    "  (SELECT typeId "
                    "     FROM invTypes "
                    "     JOIN invGroups "
                    "   USING (groupId) "
                    "   WHERE categoryId = 16 "
                    "     AND invTypes.published = 1) "
                    "  JOIN dgmTypeAttributes "
                    "  USING (typeId) "
                    "WHERE attributeId = 275");
    while (skillQuery.next()) {
      int skillId = skillQuery.value(0).toInt();
      double skillTimeConstant = skillQuery.value(1).toDouble();
      allSkillsVCharacter.skills[skillId] = BASE_SKILL_POINTS[5] * skillTimeConstant;
    }
    allSkillsVCharacter.create();
  }
  return true;
}


bool makeSureMarketDatabaseAvailable()
{
  QSqlDatabase marketDb = QSqlDatabase::database("market");
  QSqlQuery tableQuery(marketDb);
  tableQuery.prepare("SELECT * "
                     "FROM sqlite_master "
                     "WHERE type = 'table' "
                     "  AND name = :name");
  tableQuery.bindValue(":name", "prices");
  tableQuery.exec();
  if (!tableQuery.next()) {
    QSqlQuery createTableQuery(marketDb);
    createTableQuery.exec("CREATE TABLE prices ( \n"
                          "  typeId       INTEGER PRIMARY KEY, \n"
                          "  sellPrice    FLOAT, \n"
                          "  buyPrice     FLOAT, \n"
                          "  averagePrice FLOAT, \n"
                          "  updateTime   INTEGER \n"
                          ")");

  }
  return true;
}

QMimeData* listOfTypeVariantToMimeData(const QList<TypeVariant>& typeVariants) {
  QMimeData* data = new QMimeData();

  QByteArray typeVariantData;
  QDataStream typeVariantDataStream(&typeVariantData, QIODevice::WriteOnly);
  typeVariantDataStream << typeVariants;
  data->setData("bevel/type-variant", typeVariantData);

  if (!typeVariants.isEmpty()) {
    QString str = QString::number(typeVariants[0].toTypeId());
    for (int i = 1; i < typeVariants.size(); i++)
      str.append(", ").append(QString::number(typeVariants[i].toTypeId()));
    data->setText(str);
  }

  return data;
}

QMimeData* listOfSkillTrainingUnitToMimeData(const QList<SkillTrainingUnit>& units) {
  QMimeData* data = new QMimeData();

  QByteArray unitsData;
  QDataStream stream(&unitsData, QIODevice::WriteOnly);
  stream << units;
  data->setData("bevel/skill-training-unit", unitsData);

  return data;
}

QMap<int, QPixmap*> typePixmap32;
QMap<int, QPixmap*> typePixmap64;
QMap<QString, QPixmap> iconPixmap;

QPixmap* getTypePixmap32(int typeId) {
  if (!typePixmap32.contains(typeId))
    typePixmap32[typeId] = new QPixmap(QString(qApp->applicationDirPath() + "/Types/%1_32.png")
                                       .arg(QString::number(typeId)));
  return typePixmap32[typeId];
}

QPixmap* getTypePixmap64(int typeId) {
  if (!typePixmap64.contains(typeId))
    typePixmap64[typeId] = new QPixmap(QString(qApp->applicationDirPath() + "/Types/%1_64.png")
                                       .arg(QString::number(typeId)));
  return typePixmap64[typeId];
}

QPixmap getIconPixmap(const QString& name)
{
  if (!iconPixmap.contains(name))
    iconPixmap[name] = QPixmap(QString(qApp->applicationDirPath() +
                                       "/Icons/items/" + name + ".png"));
  return iconPixmap[name];
}


QString intOrDoubleToString(double x, int precision)
{
  if (x > 1e9)
    return QString::number(x, 'g', precision);
  else if (x == int(x))
    return QString::number(int(x));
  else
    return QString::number(x, 'g', precision);
}
