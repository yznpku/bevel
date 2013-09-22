#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include <QList>
#include <QMap>
#include <QObject>
#include <QString>
#include <QSqlQuery>
#include "attribute_set.hpp"
#include "skilltrainingunit.hpp"

class Character : public QObject {
  Q_OBJECT
public:
  Character();
  static Character* fromId(int id);
  void save();
  bool create();
public:
  int id;
  AttributeSet as;
  QMap<int, double> skills;     // skill points by id
  QList<SkillTrainingUnit> skillTrainingQueue;
  QMap<int, int> implants;      // implant id by slot
  QMap<int, int> boosters;      // booster id by slot
public:
  void setSkillPoints(int skillId, double skillPoints);
  void deleteSkill(int skillId);
  void addSkill(int skillId);
  int getSkillLevel(int skillId);
signals:
  void skillPointsUpdated(int skillId);
  void skillDeleted(int skillId);
  void skillAdded(int skillId);
private:
  static void initQueries();
  static bool queriesInitiated;
  static QSqlQuery characterQuery;
  static QSqlQuery skillsQuery;
};

#endif
