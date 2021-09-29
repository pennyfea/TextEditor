#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QStandardPaths>
#include <iostream>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>
#include <QTextStream>

#include "settings.h"
#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TextEditor; }
QT_END_NAMESPACE

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);
    ~TextEditor();
    void updateTitle();
    void saveFile();
    void saveFileAs();
    void openFile(const QUrl &fileUrl = QUrl());

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void actionOpen_triggered();
    void actionAbout_triggered();
    void actionSave_triggered();
    void actionNew_triggered();
    void actionConfigure_triggered();
    void documentWasModified();

private:
    void writeFile(const QUrl &fileUrl = QUrl());
    Ui::TextEditor *ui;
    QString m_title;
    QTextEdit *m_texteditor;
    QString m_textFilter = QStringLiteral("%1 (*.txt)").arg(tr("Text Files"));
    QString m_fileFilter = QStringLiteral("%1;;%2(*)").arg(m_textFilter, tr("All Files"));
    QUrl m_currentOpenUrl;


};
#endif // TEXTEDITOR_H
