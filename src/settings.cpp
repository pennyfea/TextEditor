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
    if (QFile(QStringLiteral("%1/settings.ini").arg(QCoreApplication::applicationDirPath())).exists()) {
         m_settings = new QSettings(QStringLiteral("%1/settings.ini").arg(QCoreApplication::applicationDirPath()),
         QSettings::IniFormat);
    } else {
         m_settings = new QSettings(QSettings::NativeFormat,  QSettings::UserScope,
            QStringLiteral("texteditor"), QStringLiteral("settings"), nullptr);
    }
    QTextStream(stdout) << "Settings file in use: " << m_settings->fileName();
}

void Settings::setValue(const QString &key, const QVariant &value)
{
   if(key.isEmpty())
        return;
   if (m_settings->value(key) == value)
       return;
   if (value.isNull())
       m_settings->remove(key);
   else
       m_settings->setValue(key, value);
    m_settings->sync();
    emit settingsChanged();
}

QVariant Settings::value(const QString &setting, const QVariant &defaultValue)
{
   return m_settings->value(setting, defaultValue);
}

void Settings::cleanSettings()
{
    const QStringList keys = m_settings->allKeys();
    for(const QString &key : keys) {
        if (!SETTINGS::VALIDKEYS.contains(key))
            m_settings->remove(key);
        if (m_settings->value(key).isNull() && !m_settings->value(key).isValid())
            m_settings->remove(key);
    }
}

void Settings::removeSettings(const QString &key)
{
    m_settings->remove(key);
}

int Settings::count()
{
    QStringList keys = m_settings->allKeys();
    QStringListIterator it(keys);
    while (it.hasNext())
        qDebug() << it.next();
    return keys.size();
}
