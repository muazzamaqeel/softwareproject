#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include "registration_window.h"
#include "settings.h"

#include "issuecreation.h"
=======
#include "projectcreation.h"


//temporary - Cosmin
#include "parentboard.h"


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

    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(openIssueWindow()));

    connect(ui->settingsbutton_main, SIGNAL(clicked()), this, SLOT(openSettings()));

    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(closeApp()));

    // temporary
    connect(ui->parentboardButton, SIGNAL(clicked()), this, SLOT(openParentBoard()));

    //temporary Aida's test
    connect(ui->ProjectCreationTesButton, SIGNAL(clicked()), this, SLOT(openProjectcreationTest()));




}


void MainWindow::openIssueWindow()
{
    close();
    IssueCreation* issueWindow = new IssueCreation;
    issueWindow -> showMaximized();
}
=======
void MainWindow::openProjectcreationTest(){
    close();
    ProjectCreation* Project_Creation= new ProjectCreation;
    Project_Creation->showMaximized();

}




// Function that opens the registration_window
void MainWindow::openRegistrationWindow()
{
    // qDebug() << "Register button clicked.";

    // To close the MainWindow screen when the registration window is opened
    close();

    // Create an instance of the registration window
    registration_window* registrationWindow = new registration_window;
    registrationWindow->showMaximized();
}

// Function that opens the settings_windows
void MainWindow::openSettings()
{

    close();
    Settings* settingWindow = new Settings;
    settingWindow -> showMaximized();

}
void MainWindow::closeApp()
{
    QApplication::quit(); // This will close the application.
}

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

    // To close the MainWindow screen when the registration window is opened
    close();

    // Create an instance of the parentboard window
    parentboard* parentBoard = new parentboard;
    parentBoard->showMaximized();
}

// Destructor to avoid memory leak problems
MainWindow::~MainWindow()
{
    delete ui;
}
