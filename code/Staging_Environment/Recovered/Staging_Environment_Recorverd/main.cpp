#include <Python.h>
#include "launchscreen.h"
#include "mainwindow.h"
#include "qtunittest.h"
#include "databasemanager.h"
#include <iostream>
//#include "registration_window.h"
#include <QApplication>
#include <QPixmap>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QtTest/QTest>
#include <Python.h>

// Remove this line if it's causing issues or make sure the moc file is generated correctly

//#include "testqstring.moc"

// Launch Screen window
//#include "launchscreen.h"

int main(int argc, char *argv[]) {
    Py_Initialize();

    //Application-------------------------------------------------------------------
    QApplication app(argc, argv);

    QString currentPath = QDir::currentPath();
    qDebug() << "Current Working Directory:" << currentPath;

    // Application Icon
    //app.setWindowIcon(QIcon(":/path"));
    app.setWindowIcon(QIcon(":/ScrummyIconLast.png"));

    // Launch Screen window
    LaunchScreen launchScreen;
    launchScreen.show();

    //MainWindow w;
    //w.showMaximized();
    //------------------------------------------------------------------------------



    //Test--------------------------------------------------------------------------
    //QTUnitTest testObj;
    //return QTest::qExec(&testObj, argc, argv);
    //------------------------------------------------------------------------------

    //Test
    /*
//Database-Backup---------------------------------------------------------------

    DatabaseManager databaseManager;
    QString backupFilePath = "C:/programming/GIT-REPO-SP23/softwareproject/code/Staging_Environment/Recovered/database_backup_files/backup_file.sql";

    if (databaseManager.createBackup(backupFilePath)) {
        qDebug() << "Backup created successfully!";
    } else {
        qDebug() << "Backup creation failed!";
    }
//------------------------------------------------------------------------------
*/
    int result = app.exec();
    Py_Finalize();
    return result;
    return app.exec();
}
