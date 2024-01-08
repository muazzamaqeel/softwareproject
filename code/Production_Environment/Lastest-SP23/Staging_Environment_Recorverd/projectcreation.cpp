#include "projectcreation.h"
#include "ui_projectcreation.h"
#include "projectsadmin.h"

ProjectCreation::ProjectCreation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProjectCreation)
{
    ui->setupUi(this);

    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(openProjectsAdmin()));
}

void ProjectCreation::openProjectsAdmin()
{
    hide();
    ProjectsAdmin* projectAdmin = new ProjectsAdmin;
    projectAdmin->showMaximized();
}

ProjectCreation::~ProjectCreation()
{
    delete ui;
}
