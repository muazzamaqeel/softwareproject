#include "issuecreation.h"
#include "ui_issuecreation.h"
#include "mainwindow.h"
#include <QDebug>

IssueCreation::IssueCreation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IssueCreation)
{
    ui->setupUi(this);
    connect(ui->userStoryCheck, SIGNAL(stateChanged(int)), this, SLOT(UserStoryCheck(int)));
    connect(ui->taskCheck, SIGNAL(stateChanged(int)), this, SLOT(TaskCheck(int)));
    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(BackButton()));
}

IssueCreation::~IssueCreation()
{
    delete ui;
}

void IssueCreation::UserStoryCheck(int state)
{
    if (state == Qt::Checked)
    {
        // If User Story is checked, uncheck Task
        ui->taskCheck->setChecked(false);
    }
}

void IssueCreation::BackButton()
{
    hide(); // Hide the registration window
    MainWindow* mainWindow = new MainWindow; // Create a new instance of the main window
    mainWindow->showMaximized(); // Show the main window
    ui->~IssueCreation();
}

void IssueCreation::TaskCheck(int state)
{
    if (state == Qt::Checked)
    {
        // If Task is checked, uncheck User Story
        ui->userStoryCheck->setChecked(false);
    }
}

void IssueCreation::BackToMain()
{
    MainWindow* mainWindow = new MainWindow;
    hide();
    mainWindow->showMaximized();
    IssueCreation::~IssueCreation();
}
