#include "mainwindow.h"
#include <QTextDocument>
#include <QTextCursor>
#include <QIcon>
#include <QFontDatabase>
#include <QSettings>
#include <QCloseEvent>
#include <QApplication>
#include <QStyle>
#include <QDir>
#include <QFileInfo>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isUntitled(true)
{
    setWindowTitle("EduWord");
    setWindowIcon(QIcon(":/resources/eduword.png"));
    resize(800, 600);

    setupEditor();
    createActions();
    createMenus();
    createToolBars();

    setCurrentFile("");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupEditor()
{
    textEdit = new QTextEdit(this);
    textEdit->setFontPointSize(12);
    setCentralWidget(textEdit);

    connect(textEdit->document(), &QTextDocument::contentsChanged,
            this, [this]() { setWindowModified(true); });
}

void MainWindow::createActions()
{
    // File actions
    newAct = new QAction(QIcon::fromTheme("document-new"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new document"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newDocument);

    openAct = new QAction(QIcon::fromTheme("document-open"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing document"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openDocument);

    saveAct = new QAction(QIcon::fromTheme("document-save"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveDocument);

    saveAsAct = new QAction(QIcon::fromTheme("document-save-as"), tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsDocument);

    printAct = new QAction(QIcon::fromTheme("document-print"), tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    connect(printAct, &QAction::triggered, this, &MainWindow::printDocument);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    // Edit actions
    cutAct = new QAction(QIcon::fromTheme("edit-cut"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(cutAct, &QAction::triggered, this, &MainWindow::cutText);

    copyAct = new QAction(QIcon::fromTheme("edit-copy"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(copyAct, &QAction::triggered, this, &MainWindow::copyText);

    pasteAct = new QAction(QIcon::fromTheme("edit-paste"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(pasteAct, &QAction::triggered, this, &MainWindow::pasteText);

    // Format actions
    boldAct = new QAction(QIcon::fromTheme("format-text-bold"), tr("&Bold"), this);
    boldAct->setShortcut(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    boldAct->setCheckable(true);
    connect(boldAct, &QAction::triggered, this, &MainWindow::boldText);

    italicAct = new QAction(QIcon::fromTheme("format-text-italic"), tr("&Italic"), this);
    italicAct->setShortcut(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    italicAct->setCheckable(true);
    connect(italicAct, &QAction::triggered, this, &MainWindow::italicText);

    underlineAct = new QAction(QIcon::fromTheme("format-text-underline"), tr("&Underline"), this);
    underlineAct->setShortcut(QKeySequence::Underline);
    underlineAct->setStatusTip(tr("Underline the text"));
    underlineAct->setCheckable(true);
    connect(underlineAct, &QAction::triggered, this, &MainWindow::underlineText);

    leftAlignAct = new QAction(QIcon::fromTheme("format-justify-left"), tr("&Left Align"), this);
    leftAlignAct->setStatusTip(tr("Left align the paragraph"));
    leftAlignAct->setCheckable(true);
    
    centerAct = new QAction(QIcon::fromTheme("format-justify-center"), tr("&Center"), this);
    centerAct->setStatusTip(tr("Center the paragraph"));
    centerAct->setCheckable(true);
    
    rightAlignAct = new QAction(QIcon::fromTheme("format-justify-right"), tr("&Right Align"), this);
    rightAlignAct->setStatusTip(tr("Right align the paragraph"));
    rightAlignAct->setCheckable(true);
    
    justifyAct = new QAction(QIcon::fromTheme("format-justify-fill"), tr("&Justify"), this);
    justifyAct->setStatusTip(tr("Justify the paragraph"));
    justifyAct->setCheckable(true);

    QActionGroup *alignGroup = new QActionGroup(this);
    alignGroup->addAction(leftAlignAct);
    alignGroup->addAction(centerAct);
    alignGroup->addAction(rightAlignAct);
    alignGroup->addAction(justifyAct);
    leftAlignAct->setChecked(true);

    connect(leftAlignAct, &QAction::triggered, this, [this]() { textAlign(leftAlignAct); });
    connect(centerAct, &QAction::triggered, this, [this]() { textAlign(centerAct); });
    connect(rightAlignAct, &QAction::triggered, this, [this]() { textAlign(rightAlignAct); });
    connect(justifyAct, &QAction::triggered, this, [this]() { textAlign(justifyAct); });

    colorAct = new QAction(QIcon::fromTheme("format-text-color"), tr("&Color..."), this);
    colorAct->setStatusTip(tr("Change the text color"));
    connect(colorAct, &QAction::triggered, this, &MainWindow::textColor);

    // Help actions
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    formatMenu = menuBar()->addMenu(tr("&Format"));
    formatMenu->addAction(boldAct);
    formatMenu->addAction(italicAct);
    formatMenu->addAction(underlineAct);
    formatMenu->addSeparator();
    formatMenu->addAction(leftAlignAct);
    formatMenu->addAction(centerAct);
    formatMenu->addAction(rightAlignAct);
    formatMenu->addAction(justifyAct);
    formatMenu->addSeparator();
    formatMenu->addAction(colorAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);

    formatToolBar = addToolBar(tr("Format"));
    formatToolBar->addAction(boldAct);
    formatToolBar->addAction(italicAct);
    formatToolBar->addAction(underlineAct);
    formatToolBar->addSeparator();
    formatToolBar->addAction(leftAlignAct);
    formatToolBar->addAction(centerAct);
    formatToolBar->addAction(rightAlignAct);
    formatToolBar->addAction(justifyAct);
    formatToolBar->addAction(colorAct);

    // Font combo box
    fontComboBox = new QFontComboBox(formatToolBar);
    formatToolBar->addWidget(fontComboBox);
    connect(fontComboBox, &QFontComboBox::currentFontChanged,
            this, &MainWindow::fontChanged);

    // Font size combo box
    fontSizeComboBox = new QComboBox(formatToolBar);
    fontSizeComboBox->setEditable(true);
    
    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes)
        fontSizeComboBox->addItem(QString::number(size));
    
    formatToolBar->addWidget(fontSizeComboBox);
    connect(fontSizeComboBox, &QComboBox::currentTextChanged,
            this, &MainWindow::fontSizeChanged);
    
    fontSizeComboBox->setCurrentText("12");
}

void MainWindow::newDocument()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile("");
    }
}

void MainWindow::openDocument()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Document"), "",
                                   tr("Rich Text (*.rtf);;HTML Files (*.html *.htm);;Text Files (*.txt);;All Files (*)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::saveDocument()
{
    if (isUntitled) {
        return saveAsDocument();
    } else {
        return saveFile(currentFile);
    }
}

bool MainWindow::saveAsDocument()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                               tr("Save Document"), "",
                               tr("Rich Text (*.rtf);;HTML Files (*.html *.htm);;Text Files (*.txt)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::printDocument()
{
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    if (dialog->exec() != QDialog::Accepted)
        return;

    textEdit->print(&printer);
}

void MainWindow::cutText()
{
    textEdit->cut();
}

void MainWindow::copyText()
{
    textEdit->copy();
}

void MainWindow::pasteText()
{
    textEdit->paste();
}

void MainWindow::boldText()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldAct->isChecked() ? QFont::Bold : QFont::Normal);
    textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::italicText()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italicAct->isChecked());
    textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::underlineText()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underlineAct->isChecked());
    textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::fontChanged(const QFont &font)
{
    QTextCharFormat fmt;
    fmt.setFont(font);
    textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::fontSizeChanged(const QString &size)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(size.toFloat());
    textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::textColor()
{
    QColor color = QColorDialog::getColor(textEdit->textColor(), this);
    if (!color.isValid())
        return;

    QTextCharFormat fmt;
    fmt.setForeground(color);
    textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::textAlign(QAction *action)
{
    if (action == leftAlignAct)
        textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (action == centerAct)
        textEdit->setAlignment(Qt::AlignHCenter);
    else if (action == rightAlignAct)
        textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (action == justifyAct)
        textEdit->setAlignment(Qt::AlignJustify);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About EduWord"),
                       tr("<b>EduWord</b> is a word processor application "
                          "similar to Microsoft Word, designed for Linux."));
}

bool MainWindow::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;

    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("EduWord"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (ret == QMessageBox::Save)
        return saveDocument();
    else if (ret == QMessageBox::Cancel)
        return false;

    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("EduWord"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    textEdit->setHtml(text);

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("EduWord"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out << textEdit->toHtml();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    isUntitled = fileName.isEmpty();
    
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = isUntitled ? "untitled.rtf" : QFileInfo(fileName).fileName();
    setWindowTitle(tr("%1[*] - %2").arg(shownName, tr("EduWord")));
} 