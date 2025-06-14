#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDir>

int main(int argc, char *argv[])
{
    // Initialize resources at compile time

    QApplication app(argc, argv);
    QApplication::setApplicationName("EduWord");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("EduSoft");

    QCommandLineParser parser;
    parser.setApplicationDescription("EduWord - Word Processor for Linux");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow mainWin;
    
    const QStringList args = parser.positionalArguments();
    if (!args.isEmpty())
        mainWin.loadFile(args.first());

    mainWin.show();
    return app.exec();
} 