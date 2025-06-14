#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QFontComboBox>
#include <QComboBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadFile(const QString &fileName);

private slots:
    void newDocument();
    void openDocument();
    bool saveDocument();
    bool saveAsDocument();
    void printDocument();
    void cutText();
    void copyText();
    void pasteText();
    void boldText();
    void italicText();
    void underlineText();
    void fontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);
    void textColor();
    void textAlign(QAction *action);
    void about();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void setupEditor();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    QTextEdit *textEdit;
    QString currentFile;
    bool isUntitled;

    // Actions
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *underlineAct;
    QAction *leftAlignAct;
    QAction *centerAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *colorAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;

    // Toolbars
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *formatToolBar;

    // Font controls
    QFontComboBox *fontComboBox;
    QComboBox *fontSizeComboBox;
};

#endif // MAINWINDOW_H 