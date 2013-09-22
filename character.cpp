#include "character.hpp"

#include <QtSql>
#include <QtDebug>
#include "global.hpp"
#include "queries.hpp"

bool Character::queriesInitiated;
QSqlQuery Character::characterQuery;
QSqlQuery Character::skillsQuery;


Character::Character() {
  id = -1;

  // initial attributes for new characters
  as.attr[CHARISMA_ID]     = 19;
  as.attr[INTELLIGENCE_ID] = 20;
  as.attr[MEMORY_ID]       = 20;
  as.attr[PERCEPTION_ID]   = 20;
  as.attr[WILLPOWER_ID]    = 20;
}

Character* Character::fromId(int id) {
  Character* chr = new Character();
  chr->id = id;
  if (!queriesInitiated)
    initQueries();
  
  characterQuery.bindValue(":id", id);
  characterQuery.exec();
  characterQuery.next();
  chr->as.name                  = characterQuery.value(0).toString();
  chr->as.attr[CHARISMA_ID]     = characterQuery.value(1).toInt();
  chr->as.attr[INTELLIGENCE_ID] = characterQuery.value(2).toInt();
  chr->as.attr[MEMORY_ID]       = characterQuery.value(3).toInt();
  chr->as.attr[PERCEPTION_ID]   = characterQuery.value(4).toInt();
  chr->as.attr[WILLPOWER_ID]    = characterQuery.value(5).toInt();

  QByteArray skillsData = characterQuery.value(6).toByteArray();
  QDataStream stream(skillsData);
  stream >> chr->skills;
  
  return chr;
}

void Character::save() {
  if (id == -1) {
    create();
    return;
  }
  QSqlQuery saveCharacterQuery(QSqlDatabase::database("user"));
  saveCharacterQuery.prepare("UPDATE characters "
                             "SET name = :name, "
                             "    charisma = :charisma, "
                             "    intelligence = :intelligence, "
                             "    memory = :memory, "
                             "    perception = :perception, "
                             "    willpower = :willpower, "
                             "    skills = :skills "
                             "WHERE characterId = :id");
  saveCharacterQuery.bindValue(":id", id);
  saveCharacterQuery.bindValue(":name", as.name);
  saveCharacterQuery.bindValue(":charisma", as.attr[CHARISMA_ID]);
  saveCharacterQuery.bindValue(":intelligence", as.attr[INTELLIGENCE_ID]);
  saveCharacterQuery.bindValue(":memory", as.attr[MEMORY_ID]);
  saveCharacterQuery.bindValue(":perception", as.attr[PERCEPTION_ID]);
  saveCharacterQuery.bindValue(":willpower", as.attr[WILLPOWER_ID]);
  QByteArray skillsData;
  QDataStream stream(&skillsData, QIODevice::WriteOnly);
  stream << skills;
  saveCharacterQuery.bindValue(":skills", skillsData);
  saveCharacterQuery.exec();
}

bool Character::create() {
  QSqlQuery createCharacterQuery(QSqlDatabase::database("user"));
  createCharacterQuery.prepare("INSERT INTO characters "
                               "(name, charisma, intelligence, memory, "
                               " perception, willpower, skills) "
                               "VALUES "
                               "(:name, :charisma, :intelligence, :memory, "
                               " :perception, :willpower, :skills)");
  createCharacterQuery.bindValue(":name", as.name);
  createCharacterQuery.bindValue(":charisma", as.attr[CHARISMA_ID]);
  createCharacterQuery.bindValue(":intelligence", as.attr[INTELLIGENCE_ID]);
  createCharacterQuery.bindValue(":memory", as.attr[MEMORY_ID]);
  createCharacterQuery.bindValue(":perception", as.attr[PERCEPTION_ID]);
  createCharacterQuery.bindValue(":willpower", as.attr[WILLPOWER_ID]);
  QByteArray skillsData;
  QDataStream stream(&skillsData, QIODevice::WriteOnly);
  stream << skills;
  qDebug("%d", skillsData.size());
  createCharacterQuery.bindValue(":skills", skillsData);
  createCharacterQuery.exec();

  if (createCharacterQuery.numRowsAffected() != 1)
    return false;

  QSqlQuery* lastInsertRowidQuery = Queries::getLastInsertRowidQuery();
  lastInsertRowidQuery->exec();
  lastInsertRowidQuery->next();
  id = lastInsertRowidQuery->value(0).toInt();

  return true;
}

void Character::initQueries() {
  QSqlDatabase userDb = QSqlDatabase::database("user");
  characterQuery = QSqlQuery(userDb);
  characterQuery.prepare("SELECT name, charisma, intelligence, "
                         "       memory, perception, willpower, skills "
                         "FROM characters "
                         "WHERE characterId = :id");
  queriesInitiated = true;
}

void Character::setSkillPoints(int skillId, double skillPoints) {
  if (skills[skillId] != skillPoints) {
    skills[skillId] = skillPoints;
    emit skillPointsUpdated(skillId);
  }
}

void Character::deleteSkill(int skillId) {
  if (skills.remove(skillId))
    emit skillDeleted(skillId);
}

void Character::addSkill(int skillId) {
  if (!skills.contains(skillId)) {
    skills[skillId] = 0.0;
    emit skillAdded(skillId);
  }
}

int Character::getSkillLevel(int skillId) {
  if (!skills.contains(skillId))
    return -1;
  QSqlQuery* skillMultiplierQuery = Queries::getSkillMultiplierQuery();
  skillMultiplierQuery->bindValue(":id", skillId);
  skillMultiplierQuery->exec();
  skillMultiplierQuery->next();
  double skillMultiplier = skillMultiplierQuery->value(0).toDouble();
  int skillLevel;
  for (skillLevel = 0; skillLevel < 5; skillLevel++)
    if (skillMultiplier * BASE_SKILL_POINTS[skillLevel + 1] > skills[skillId])
      break;
  return skillLevel;
}
