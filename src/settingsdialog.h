#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDir>
#include <QPushButton>
#include "settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
    void updateTooltips();
    void loadSettings();
    void saveSettings();
    void restoreDefaultSettings();
};

#endif // SETTINGSDIALOG_H
