#include "projectsadmin.h"
#include "ui_projectsadmin.h"
#include "mainwindow.h"
#include "projectcreation.h"

ProjectsAdmin::ProjectsAdmin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectsAdmin)
{
    ui->setupUi(this);

    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(openMainWindowfromAdmin()));
    connect(ui->createProjectButton, SIGNAL(clicked()), this, SLOT(openProjectCreation()));
}

void ProjectsAdmin::openMainWindowfromAdmin()
{
    hide();
    MainWindow* mainWindow = new MainWindow;
    mainWindow->showMaximized();
}

void ProjectsAdmin::openProjectCreation()
{
    hide();
    ProjectCreation* projectCreation = new ProjectCreation;
    projectCreation->showMaximized();
}

ProjectsAdmin::~ProjectsAdmin()
{
    delete ui;
}
