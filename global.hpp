#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_

#include <QString>
#include <QList>
#include <QPixmap>
#include "typevariant.hpp"
#include "skilltrainingunit.hpp"
#include "main_window.hpp"
class QMimeData;
class Market;

// global objects
extern MainWindow* mainWindow;  // top level widget
extern Market* market;

// helper functions
QString decamelize(const QString& str); 
QString intOrDoubleToString(double x, int precision = 2);

// resource access
QPixmap* getTypePixmap32(int typeId);
QPixmap* getTypePixmap64(int typeId);
QPixmap getIconPixmap(const QString& name);
// consistency maintenance functions
bool makeSureUserDatabaseAvailable();
bool makeSureMarketDatabaseAvailable();

// character attributes' id
const int CHARISMA_ID     = 164;
const int INTELLIGENCE_ID = 165;
const int MEMORY_ID       = 166;
const int PERCEPTION_ID   = 167;
const int WILLPOWER_ID    = 168;

const int SKILL_TIME_CONSTANT_ID = 275;

// base skill points
const double BASE_SKILL_POINTS[] = {0, 250, 1414.21356,
                                    8000, 45254.834, 256000};

QMimeData* listOfTypeVariantToMimeData(const QList<TypeVariant>& typeVariants);
QMimeData* listOfSkillTrainingUnitToMimeData(const QList<SkillTrainingUnit>& units);

#endif
