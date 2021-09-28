#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    loadSettings();

    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, [this] (QAbstractButton * abutton) {
        QPushButton * button = qobject_cast<QPushButton *>(abutton);
        if (!button)
            done(0);

        if (button == ui->buttonBox->button(QDialogButtonBox::Apply)) {
            saveSettings();
            done(1);
        }

        if (button == ui->buttonBox->button(QDialogButtonBox::Discard))
            done(0);

        if (button == ui->buttonBox->button(QDialogButtonBox::Reset))
            loadSettings();

        if (button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
            restoreDefaultSettings();
    });

    updateTooltips();
    adjustSize();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::updateTooltips()
{
    ui->buttonBox->button(QDialogButtonBox::Apply)->setToolTip(tr("Close and save changes"));
    ui->buttonBox->button(QDialogButtonBox::Discard)->setToolTip(tr("Close and forget changes"));
    ui->buttonBox->button(QDialogButtonBox::Reset)->setToolTip(tr("Reset values to stored settings"));
    ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->setToolTip(tr("Reset values to defaults"));
}

void SettingsDialog::loadSettings()
{
    ui->rbYMoveCursor->setChecked(Settings::GetInstance().value(SETTINGS::MOVECURSORONLOAD, true).toBool());
    ui->rbNMoveCursor->setChecked(!Settings::GetInstance().value(SETTINGS::MOVECURSORONLOAD, true).toBool());
    ui->cbRestoreGeometry->setChecked(Settings::GetInstance().value(SETTINGS::REMEMBERGEOMETRY, true).toBool());
}

void SettingsDialog::saveSettings()
{
    Settings::GetInstance().setValue(SETTINGS::REMEMBERGEOMETRY, ui->cbRestoreGeometry->isChecked());
    QByteArray geo = saveGeometry();
    Settings::GetInstance().setValue(SETTINGS::MAINGEOMETRY, ui->cbRestoreGeometry->isChecked() ? QVariant() : QVariant(geo));
    Settings::GetInstance().setValue(SETTINGS::MOVECURSORONLOAD, ui->rbYMoveCursor->isChecked());
}

void SettingsDialog::restoreDefaultSettings()
{
    Settings::GetInstance().setValue(SETTINGS::REMEMBERGEOMETRY, true);
    Settings::GetInstance().setValue(SETTINGS::MAINGEOMETRY, saveGeometry());
    Settings::GetInstance().setValue(SETTINGS::MOVECURSORONLOAD, true);
    loadSettings();
}
