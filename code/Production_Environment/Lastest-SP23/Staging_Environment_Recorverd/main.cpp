#include "mainwindow.h"
//#include "registration_window.h"
#include <QApplication>
#include <QPixmap>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <iostream>
#include <QtSql/QSqlError>
#include <QtTest/QTest>
//#include "qtunittest.h"
// Remove this line if it's causing issues or make sure the moc file is generated correctly
//#include "testqstring.moc"

// Launch Screen window
#include "launchscreen.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    //MainWindow w;
    //w.showMaximized();
    LaunchScreen launchScreen;
    launchScreen.show();

    // Test Object
    //QTUnitTest testObject;
    //QTest::qExec(&testObject, argc, argv);
    return app.exec();
}

