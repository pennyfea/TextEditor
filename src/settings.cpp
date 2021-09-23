#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    initSettings();

}

Settings& Settings::GetInstance()
{
    static Settings instance;
    return instance;
}

void Settings::initSettings()
{
    if (QFile(QStringLiteral("%1/settings.ini").arg(QCoreApplication::applicationDirPath())).exists())
    {
         settings = new QSettings(QStringLiteral("%1/settings.ini").arg(QCoreApplication::applicationDirPath()),
         QSettings::IniFormat);
    } else{
         settings = new QSettings(QSettings::NativeFormat,  QSettings::UserScope,
            QStringLiteral("PROJECT_NAME"), QStringLiteral("settings"), nullptr);
    }
}

void Settings::setValue(const QString &setting, const QVariant &value)
{
   if (settings->value(setting) == value)
       return;
   if (value.isNull())
       settings->remove(setting);
   else
       settings->setValue(setting, value);
   emit settingsChanged();

   settings->sync();
}

QVariant Settings::value(const QString &setting, const QVariant &defaultValue)
{
   return settings->value(setting, defaultValue);
}

void Settings::cleanSettings()
{
    QStringList keys = settings->allKeys();
    foreach (const QString &key, keys)
    {
        if (key.isEmpty() || key.contains(" "))
            settings->remove(key);
    }
}

void Settings::removeSettings(const QString &key)
{
    settings->remove(key);
}

int Settings::count()
{
    QStringList keys = settings->allKeys();
    QStringListIterator it(keys);
    while (it.hasNext())
        qDebug() << it.next();
    return keys.size();
}
