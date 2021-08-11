#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCoreApplication>
#include <QObject>
#include <QSettings>
#include <QFile>
#include "textEditorConfig.h"

class Settings : public QObject
{
    Q_OBJECT
public:
     static Settings& GetInstance();
     void setValue(const QString& setting = QString(), const QVariant& value = QVariant());
     QVariant value(const QString& setting = QString(), const QVariant& defaultValue = QVariant());
     void removeSettings(const QString &key);
     int  count();
     void cleanSettings();

signals:
    void settingsChanged();

private:
    explicit Settings(QObject *parent = nullptr);
    Settings* operator=(Settings& other) = delete;
    Settings(const Settings&) = delete;
    ~Settings() = default;
    void initSettings();
    QSettings* settings = nullptr;
};

#endif // SETTINGS_H
