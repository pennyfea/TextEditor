#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QStandardPaths>
#include <QCheckBox>
#include <QGridLayout>
#include <iostream>
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TextEditor; }
QT_END_NAMESPACE

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);
    ~TextEditor();

protected:
 //   void closeEvent(QCloseEvent *event) override;

private slots:
    void actionOpen_triggered();
    void actionAbout_triggered();
    void actionSave_triggered();
    void actionNew_triggered();
    void actionConfigure_triggered();
    void askToSave();
    void documentWasModified();



private:
    void updateTitle(const QString& filename);
    void setCurrentFile(const QString &fileName);
    void saveFile(const QString &fileName);
    void loadFile(const QString &fileName);
    bool fileExists(const QString& path);
    QString strippedName(const QString &fullFileName);

    Ui::TextEditor *ui;
    QString m_title;
    QString m_curFile;
    QTextEdit * m_texteditor;


};
#endif // TEXTEDITOR_H
