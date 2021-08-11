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
    connect(ui->actionConfigure_Text_Editor, &QAction::triggered, this, &TextEditor::actionConfigure_triggered);


    this->restoreGeometry(Settings::GetInstance().value("geometry").toByteArray());
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
    const QString projectInfo = QStringLiteral("Project Name: %1\nVersion: %2\nUsing QT: %3").arg(PROJECT_NAME).arg(TextEditor_VERSION).arg(QT_VERSION_STR);
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

void TextEditor::actionConfigure_triggered()
{
    QCheckBox *checkbox = new QCheckBox("Cursor to end of file");
    QCheckBox *geometryCheckBox = new QCheckBox("Save and restore geometry");
    QMessageBox msgBox(this);
    msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Discard | QMessageBox::Reset | QMessageBox::RestoreDefaults);
    msgBox.setDefaultButton(QMessageBox::Apply);
    checkbox->setToolTip("Option to move cursor to end of text on file open");
    geometryCheckBox->setToolTip("Option to save and restore geometry on open / close");
    msgBox.setCheckBox(checkbox);

    QGridLayout *grid = qobject_cast<QGridLayout *>(msgBox.layout());
    int index = grid->indexOf(checkbox);
    int row, column, rowSpan, columnSpan;
    grid->getItemPosition(index, &row, &column, &rowSpan, &columnSpan);
    grid->addWidget(geometryCheckBox, row + 1,  column, rowSpan, columnSpan);

    static bool checkGeometry = false;
    QObject::connect(geometryCheckBox, &QCheckBox::stateChanged, [&](int state){
        if (static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked) {
            checkGeometry = true;
        }
        else{
            checkGeometry = false;
        }
    });

    static bool changeCursor = false;
    QObject::connect(checkbox, &QCheckBox::stateChanged, [&](int state){
        if (static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked) {
            changeCursor = true;
        }
        else{
            changeCursor = false;
        }
    });


    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Apply:
          if (checkGeometry)
            Settings::GetInstance().setValue("geometry", saveGeometry());
          else
            Settings::GetInstance().removeSettings("geometry");
          if (changeCursor)
              m_texteditor->moveCursor(QTextCursor::End);
          break;
      case QMessageBox::Discard:
          // Don't Save was clicked
          Settings::GetInstance().removeSettings("geometry");
          msgBox.close();
          break;
      case QMessageBox::Reset:
          Settings::GetInstance().removeSettings("geometry");
          // Cancel was clicked
          break;
      case QMessageBox::RestoreDefaults:
        // Restore defaults
        Settings::GetInstance().removeSettings("geometry");
        restoreGeometry(Settings::GetInstance().value("geometry").toByteArray());
        resize(485,491);
        break;
      default:
          // should never be reached
          break;
    }
}
