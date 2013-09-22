#ifndef SKILLTRAININGUNIT_HPP
#define SKILLTRAININGUNIT_HPP

#include <QDataStream>
#include <QList>

class QMimeData;

class SkillTrainingUnit
{
public:
  SkillTrainingUnit();
  SkillTrainingUnit(int skillId, int level);
  static QList<SkillTrainingUnit> fromMimeData(const QMimeData* mimeData);
  bool operator<(const SkillTrainingUnit& o) const;
  bool operator==(const SkillTrainingUnit& o) const;
public:
  int skillId;
  int level;
};

QDataStream& operator<<(QDataStream& stream, const SkillTrainingUnit& tv);
QDataStream& operator>>(QDataStream& stream, SkillTrainingUnit& tv);

#endif // SKILLTRAININGUNIT_HPP
