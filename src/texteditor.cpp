#include "texteditor.h"
#include "./ui_texteditor.h"

TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextEditor)
    , m_title("Text Editor")
    , m_texteditor(new QTextEdit(this))
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowIcon(QIcon(":/icons/accessories-text-editor.svg"));
    setCentralWidget(m_texteditor);

    connect(ui->actionOpen, &QAction::triggered, this, &TextEditor::actionOpen_triggered);
    connect(ui->actionAbout, &QAction::triggered, this, &TextEditor::actionAbout_triggered);
    connect(ui->actionSave, &QAction::triggered, this, &TextEditor::actionSave_triggered);
    connect(ui->actionNew, &QAction::triggered, this, &TextEditor::actionNew_triggered);
    connect(m_texteditor->document(),  &QTextDocument::contentsChanged, this, &TextEditor::documentWasModified);
    connect(ui->actionSave_As, &QAction::triggered, this, &TextEditor::saveFileAs);
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
    QUrl fileUrl = QFileDialog::getOpenFileUrl(this, tr("Open File"), QUrl::fromLocalFile(QDir::homePath()), m_fileFilter, &m_textFilter);
    openFile(fileUrl);
}

void TextEditor::actionAbout_triggered()
{
    const QString projectInfo = QStringLiteral("Project Name: %1\nVersion: %2\nUsing QT: %3").arg(qAppName()).arg(qApp->applicationVersion()).arg(qVersion());
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About Text Editor");
    msgBox.setText(projectInfo);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void TextEditor::actionNew_triggered()
{
    m_texteditor->clear();
    setWindowTitle(QString());
}

void TextEditor::actionSave_triggered()
{
    saveFile();
}

void TextEditor::actionConfigure_triggered()
{
    SettingsDialog dialog(this);
    dialog.exec();
}

void TextEditor::writeFile(const QUrl &fileUrl)
{
    if (fileUrl.isEmpty())
         return;
    QFile file(fileUrl.toLocalFile());
    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::information(this, tr("Unable to open file"),
        file.errorString());
        return;
     }
    file.write(m_texteditor->toPlainText().toLocal8Bit());
    file.close();
    m_currentOpenUrl = fileUrl;
    setWindowModified(false);
    updateTitle();
}

void TextEditor::saveFileAs()
{
    QUrl fileUrl = QFileDialog::getSaveFileUrl(this, tr("Save File"), QUrl::fromLocalFile(QDir::homePath()), m_fileFilter, &m_textFilter);
    writeFile(fileUrl);
}

void TextEditor::updateTitle()
{
    QString title = m_title;
    if(!m_currentOpenUrl.fileName().isEmpty())
        title.append(QStringLiteral(" - %2").arg(m_currentOpenUrl.fileName()));
    if(m_texteditor->document()->isModified())
        title.append("[*]");
    setWindowTitle(title);
}

void TextEditor::saveFile()
{
    if (m_currentOpenUrl.isEmpty()){
        saveFileAs();
        return;
    }
    writeFile(m_currentOpenUrl);
}

void TextEditor::openFile(const QUrl &fileUrl)
{
    if (fileUrl.isEmpty())
          return;

    QFile file(fileUrl.toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          QMessageBox::information(this, tr("Unable to open file"),
          file.errorString());
          return;
    }

    m_currentOpenUrl = fileUrl;
    QTextStream textFile(&file);
    m_texteditor->setText(textFile.readAll());
    if (Settings::GetInstance().value(SETTINGS::MOVECURSORONLOAD, true).toBool())
            m_texteditor->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    setWindowTitle(QStringLiteral("%1 - %2").arg(qApp->applicationName(), fileUrl.fileName()));//
}

void TextEditor::documentWasModified()
{
    updateTitle();
    setWindowModified(m_texteditor->document()->isModified());

}

void TextEditor::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
        e->acceptProposedAction();
    else
        e->ignore();
}

void TextEditor::dropEvent(QDropEvent *e)
{
    openFile(e->mimeData()->urls().at(0));
}

void TextEditor::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    bool saveGeo = Settings::GetInstance().value(SETTINGS::REMEMBERGEOMETRY, true).toBool();
    Settings::GetInstance().setValue(SETTINGS::MAINGEOMETRY) , saveGeo ? QVariant(saveGeometry()) : QVariant();
}
