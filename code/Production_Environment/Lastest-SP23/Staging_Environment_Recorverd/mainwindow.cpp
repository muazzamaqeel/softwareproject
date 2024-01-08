#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include "databasemanager.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "registration_window.h"
#include "settings.h"
#include "issuecreation.h"
#include "parentboard.h"
#include "adminstrator.h"
#include <QCryptographicHash>




// Constructor of MainWindow Class
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // For Background
    QPixmap pix("C:/programming/softwareproject/softwareproject/code/Production_Environment/SP23/assets/mainwindow/1.jpg");
    ui->bg_main->setPixmap(pix);

    //Buttons on the mainwindow
    // Using the connect function to call the openRegistrationWindow() function
    connect(ui->registerbutton_main, SIGNAL(clicked()), this, SLOT(openRegistrationWindow()));

    connect(ui->signIn_Button, SIGNAL(clicked()), this, SLOT(userEncryptedLogin()));

    connect(ui->settingsbutton_main, SIGNAL(clicked()), this, SLOT(openSettings()));

    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(closeApp()));

    //temporary
    connect(ui->parentboardButton, SIGNAL(clicked()), this, SLOT(openParentBoard()));

    //temporary
    connect(ui->adminButton, SIGNAL(clicked()), this, SLOT(adminLogin()));

}

void MainWindow::openIssueWindow()
{
    close();
    IssueCreation* issueWindow = new IssueCreation;
    issueWindow -> showMaximized();
}

// Function that opens the registration_window
void MainWindow::openRegistrationWindow()
{
    // qDebug() << "Register button clicked.";

    // To close the MainWindow screen when the registration window is opened
    hide();

    // Create an instance of the registration window
    registration_window* registrationWindow = new registration_window;
    registrationWindow->showMaximized();
    ui->~MainWindow();
}

// Function that opens the settings_windows
void MainWindow::openSettings()
{

    hide();
    Settings* settingWindow = new Settings;
    settingWindow -> showMaximized();
    ui->~MainWindow();

}
void MainWindow::closeApp()
{
    QApplication::quit(); // This will close the application.
}

// Why is this defined here ? - Wesley
void Settings::goBackToMainWindow()
{
    hide(); // Hide the settings window
    MainWindow* mainWindow = new MainWindow; // Create a new instance of the main window
    mainWindow->showMaximized(); // Show the main window
}

//temporary - Cosmin
void MainWindow::openParentBoard()
{
    // qDebug() << "Parentboard button clicked.";

    // To close the MainWindow screen when the parentboard window is opened
    hide();

    // Create an instance of the parentboard window
    parentboard* parentBoard = new parentboard;
    parentBoard->showMaximized();
    ui->~MainWindow();
}

// temporary - Cosmin
void MainWindow::adminLogin()
{
    // qDebug() << "Administrator button clicked.";

    // To close the MainWindow screen when the admininstrator window is opened
    hide();

    // Create an instance of the parentboard window
    adminstrator* adminLogin = new adminstrator;
    adminLogin->showMaximized();
    ui->~MainWindow();
}



void MainWindow::userEncryptedLogin()
{
    QString inputUsername = ui->input_username->text();
    QString inputPassword = ui->input_password->text();

    // Hash the input password using SHA-256
    QByteArray passwordData = inputPassword.toUtf8();
    QByteArray hashedInputPassword = QCryptographicHash::hash(passwordData, QCryptographicHash::Sha256).toHex();

    // Database operations to verify hashed passwords
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);
        query.prepare("SELECT password FROM User WHERE username = :username");
        query.bindValue(":username", inputUsername);

        if (query.exec()) {
            if (query.next()) {
                QString storedHash = query.value(0).toString();
                qDebug() << "Input Hash: " << hashedInputPassword;
                qDebug() << "Stored Hash: " << storedHash;

                if (hashedInputPassword == storedHash.toUtf8()) {
                    qDebug() << "Password is correct.";
                } else {
                    qDebug() << "Password is incorrect.";
                }
            }
        } else {
            qDebug() << "Failed to retrieve user data:" << query.lastError().text();
        }
        dbobj.close();
    } else {
        qDebug() << "Connection Not Established - Login class!";
    }
}


// Destructor to avoid memory leak problems
MainWindow::~MainWindow()
{
    delete ui;
}
