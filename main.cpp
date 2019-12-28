#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QtPrintSupport>
#endif
#endif
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                              QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        a.installTranslator(&qtBaseTranslator);
    }
    MainWindow w;
    w.setGeometry(100, 100, 900, 600);
    w.show();

    return a.exec();
}
