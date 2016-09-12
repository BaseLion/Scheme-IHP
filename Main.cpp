#include "MainWindow.h"
#include <QApplication>

#include <QDebug>
//#include "Units/Pressure.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(schemeIhp);

    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);

    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
