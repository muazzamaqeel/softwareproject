#include "databasemanager.h"
#include <QDebug>
#include <QtSql/QSqlError>
#include <QProcess>
#include <QDateTime>

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("aws-dbtest.cjqugotdygrg.eu-central-1.rds.amazonaws.com");
    db.setDatabaseName("scrummy");
    db.setUserName("admin");
    db.setPassword("sofproj23");

    if (!db.open()) {
        qDebug() << "Connection Not Established - Database Class:" << db.lastError().text();
    } else {
        qDebug() << "Connection Established - Database Class!";
    }
}
QSqlDatabase DatabaseManager::getDatabase() const {
    return db;
}

DatabaseManager::~DatabaseManager(){
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database connection closed - Database Class.";
    }
}

bool DatabaseManager::createBackup(const QString& backupFilePath) {
    QString program = "C:\\Program Files\\MySQL\\MySQL Server 8.0\\bin\\mysqldump";

    QStringList arguments;
    arguments << "--host=" + db.hostName()
              << "--user=" + db.userName()
              << "--password=" + db.password()
              << db.databaseName();

    QProcess process;
    process.setStandardOutputFile(backupFilePath);

    process.start(program, arguments);
    if (!process.waitForStarted() || !process.waitForFinished()) {
        qDebug() << "Backup failed:" << process.errorString();
        return false;
    }

    qDebug() << "Backup created successfully at:" << backupFilePath;
    return true;
}

