#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCoreApplication>
#include <QObject>
#include <QSettings>
#include <QFile>
#include <QVariant>
#include <QDebug>
#include <QTextStream>


class QSettings;

namespace SETTINGS {
    inline const static QString MAINGEOMETRY = QStringLiteral("MainGeometry");
    inline const static QString MOVECURSORONLOAD = QStringLiteral("editor/moveCursorOnLoad");
    inline const static QString REMEMBERGEOMETRY = QStringLiteral("RememberGeometry");

    inline static auto VALIDKEYS = QStringList {
            SETTINGS::MAINGEOMETRY
            , SETTINGS::MOVECURSORONLOAD
            , SETTINGS::REMEMBERGEOMETRY
    };
}

class Settings : public QObject
{
    Q_OBJECT
public:
     static Settings& GetInstance();
     void setValue(const QString& key = QString(), const QVariant& value = QVariant());
     QVariant value(const QString& key = QString(), const QVariant& defaultValue = QVariant());
     int  count();


signals:
    void settingsChanged();

private:
    explicit Settings(QObject *parent = nullptr);
    Settings* operator=(Settings& other) = delete;
    Settings(const Settings&) = delete;
    ~Settings() = default;
    void initSettings();
    void cleanSettings();
    void removeSettings(const QString &key);
    QSettings* m_settings = nullptr;
};

#endif // SETTINGS_H
