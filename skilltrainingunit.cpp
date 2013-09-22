#include "skilltrainingunit.hpp"

#include <QMimeData>

SkillTrainingUnit::SkillTrainingUnit() {
  skillId = -1;
  level = -1;
}

SkillTrainingUnit::SkillTrainingUnit(int skillId, int level) {
  this->skillId = skillId;
  this->level = level;
}

QList<SkillTrainingUnit> SkillTrainingUnit::fromMimeData(const QMimeData* mimeData) {
  QList<SkillTrainingUnit> stul;
  QByteArray data = mimeData->data("eveop/skill-training-unit");
  QDataStream stream(data);
  stream >> stul;
  return stul;
}

bool SkillTrainingUnit::operator<(const SkillTrainingUnit& o) const {
  if (skillId < o.skillId)
    return true;
  if (skillId == o.skillId)
    if (level < o.level)
      return true;
  return false;
}

bool SkillTrainingUnit::operator==(const SkillTrainingUnit& o) const {
  return skillId == o.skillId && level == o.level;
}

QDataStream& operator<<(QDataStream& stream, const SkillTrainingUnit& tv) {
  stream << tv.skillId << tv.level;
  return stream;
}

QDataStream& operator>>(QDataStream& stream, SkillTrainingUnit& tv) {
  stream >> tv.skillId >> tv.level;
  return stream;
}
