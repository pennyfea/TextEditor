#include "texteditor.h"
#include "./ui_texteditor.h"
#include <iostream>
TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextEditor)
    , m_title("Text Editor")
    , m_texteditor(new QTextEdit(this))
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/accessories-text-editor.svg"));
    setWindowTitle(m_title);
    setCurrentFile(QString());
    setCentralWidget(m_texteditor);

    connect(ui->actionOpen, &QAction::triggered, this, &TextEditor::actionOpen_triggered);
    connect(ui->actionAbout, &QAction::triggered, this, &TextEditor::actionAbout_triggered);
    connect(ui->actionSave, &QAction::triggered, this, &TextEditor::actionSave_triggered);
    connect(ui->actionNew, &QAction::triggered, this, &TextEditor::actionNew_triggered);
    connect(m_texteditor->document(),  &QTextDocument::contentsChanged, this, &TextEditor::documentWasModified);
    connect(ui->actionSave_As, &QAction::triggered, this, &TextEditor::askToSave);
    connect(ui->actionExit, &QAction::triggered, [](){  QApplication::quit();});
}

TextEditor::~TextEditor()
{
    delete ui;
}


void TextEditor::actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("Text Files(*.txt)"));
    if (fileName.isEmpty())
        return;
    if (m_curFile == fileName)
        return;
    m_texteditor->clear();
    loadFile(fileName);
}

void TextEditor::actionAbout_triggered()
{
    const QString projectInfo = QStringLiteral("Project Name: %1\nVersion: %2\nUsing QT: %3").arg(PROJECT_NAME).arg(TextEditor_VERSION).arg(QT_VERSION_MAJOR);
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About Text Editor");
    msgBox.setText(projectInfo);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void TextEditor::actionNew_triggered()
{
    m_texteditor->clear();
    setCurrentFile(QString());
}

void TextEditor::actionSave_triggered()
{
    QString fileName;
    if (fileExists(m_curFile))
    {
        fileName = m_curFile;
        saveFile(fileName);
    }
    else
    {
        askToSave();
    }
}

void TextEditor::askToSave()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                               QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                               tr("Text Files(*.txt)"));

    saveFile(fileName);
}

void TextEditor::updateTitle(const QString &filename)
{
    QString _filename =  QString(" - " +  strippedName(filename));
    setWindowTitle(m_title + _filename + "[*]");
}

void TextEditor::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::information(this, tr("Unable to open file"),
        file.errorString());
        return;
     }
    QTextStream out(&file);
    out << m_texteditor->toPlainText();
    setCurrentFile(file.fileName());
}

bool TextEditor::fileExists(const QString &path)
{
    QFileInfo checkFile(path);
    return checkFile.exists() && checkFile.isFile();
}


void TextEditor::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(this, tr("Unable to open file"),
        file.errorString());
        return;
    }
    while (!file.atEnd()) {
         QByteArray line = file.readLine();
         m_texteditor->append(line);
     }
    setCurrentFile(file.fileName());
}


void TextEditor::setCurrentFile(const QString &fileName)
{
        m_curFile = fileName;
        m_texteditor->document()->setModified(false);
        setWindowModified(false);

        QString shownName = m_curFile;
        qDebug() << shownName;
        if (m_curFile.isEmpty())
            shownName = "untitled.txt";
        updateTitle(shownName);
}

void TextEditor::documentWasModified()
{
    setWindowModified(m_texteditor->document()->isModified());
}

QString TextEditor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

