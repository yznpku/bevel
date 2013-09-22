#include "settings.hpp"

#include <QVariant>

QSettings* settings;

QVariant getSettingsValue(const QString& key, const QVariant& defaultValue) {
  if (!settings)
    return QVariant();
  if (!settings->contains(key))
    settings->setValue(key, defaultValue);
  return settings->value(key);
}

void Settings::initSettings()
{
  settings = new QSettings("futurecode", "eveop");
}

int Settings::getMarketOrdersTimeLimitSetting()
{
  return getSettingsValue("marketOrdersTimeLimit", 168).toInt();
}
