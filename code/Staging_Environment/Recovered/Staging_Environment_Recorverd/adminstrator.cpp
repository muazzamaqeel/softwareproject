#include "adminstrator.h"
#include "projectsadmin.h"
#include "databasemanager.h"
#include <QPixmap>
#include "ui_adminstrator.h"
#include "mainwindow.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <iostream>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QCryptographicHash> // Include this header for hashing
#include "projectcreation.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QDebug>
#include <QTableView>


adminstrator::adminstrator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminstrator)
{
    ui->setupUi(this);

    connect(ui->adminloginButton, SIGNAL(clicked()), this, SLOT(checkCredentials()));
    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(previousWindow()));
}

void adminstrator::projectSelection()
{
    ProjectsAdmin* projectsAdmin = new ProjectsAdmin;
    projectsAdmin->showMaximized();
    ui->~adminstrator();
}

void adminstrator::previousWindow()
{
    MainWindow* mainWindow = new MainWindow;
    mainWindow->showMaximized();
}

// Function to store user input values and display them for testing
void adminstrator::storeInputValues() {

    // Capture user input from various fields
    QString username = ui->input_username->text();
    QString password = ui->input_password->text();


    // Create a DatabaseManager object and attempt to establish a database connection
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (dbobj.isOpen()) {
        qDebug() << "Connection Established - Adminstrator class!";

        // Prepare the SQL query to insert data into the User table
        QSqlQuery query(dbobj);

        // Construct the SQL command
        QString selectQuery = "Select username from User, password ";

        // Prepare the query with the command
        query.prepare(selectQuery);

        // Bind values to the query placeholders




        // Execute the query
        if (query.exec()) {
            qDebug() << "Data inserted into User table successfully!";
        } else {
            qDebug() << "Failed to insert data into User table:" << query.lastError().text();
        }

        dbobj.close(); // Close the database connection
    } else {
        qDebug() << "Connection Not Established - Registration class!";
    }
}


// Function to check user credentials against the database

void adminstrator::checkCredentials() {
    // Capture user input from various fields
    QString username = ui->input_username->text();
    QString password = ui->input_password->text();

    // Hash the input password using SHA-256 (or any suitable hashing algorithm)
    //QByteArray hashedpassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    // Create a DatabaseManager object and attempt to establish a database connection
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (dbobj.isOpen()) {
        qDebug() << "Connection Established - Adminstrator class!";

        // Prepare the SQL query to check credentials with hashed password
        QSqlQuery query(dbobj);

        // Construct the SQL command to select matching username and hashed password
        QString selectQuery = "SELECT * FROM Adminstrate WHERE Username = :Username AND Password = :Password";

        // Prepare the query with the command
        query.prepare(selectQuery);

        // Bind values to the query placeholders
        query.bindValue(":Username", username);
        query.bindValue(":Password", password);

        // Execute the query
        if (query.exec()) {
            if (query.next()) {
                hide();
                qDebug() << "Login Successful!";
                // Perform actions for successful login, such as opening a new window or granting access
                ProjectsAdmin *projectadmi_obj = new ProjectsAdmin;
                projectadmi_obj->showMaximized();
                ui->~adminstrator();
            } else {
                qDebug() << "Invalid username or password.";
                // Display an error message indicating invalid credentials
            }
        } else {
            qDebug() << "Query execution error:" << query.lastError().text();
            // Handle query execution error
        }

        dbobj.close(); // Close the database connection
    } else {
        qDebug() << "Connection Not Established - Adminstrator class!";
        // Handle connection error
    }
}






adminstrator::~adminstrator()
{
    delete ui;
}
