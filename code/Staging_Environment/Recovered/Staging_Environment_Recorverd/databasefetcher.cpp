#include "databasefetcher.h"
#include "databasemanager.h"
#include "QDebug"
#include "QSql"
#include "QSqlQuery"

DatabaseFetcher::DatabaseFetcher() {


}


void DatabaseFetcher::Retrieve_UserTable(){
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (!dbobj.isOpen()) {
        qDebug() << "Connection Not Established - pb_productbacklog_implmentation!";
        return;
    }
    QSqlQuery querySelect(dbobj);
    querySelect.prepare("SELECT * "
                        "FROM User ");
}








