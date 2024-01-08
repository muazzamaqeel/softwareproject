#include "adminstrator.h"
#include "projectsadmin.h"
#include "ui_adminstrator.h"
#include "mainwindow.h"

adminstrator::adminstrator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminstrator)
{
    ui->setupUi(this);

    connect(ui->adminloginButton, SIGNAL(clicked()), this, SLOT(projectSelection()));
    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(previousWindow()));
}

void adminstrator::projectSelection()
{
    hide();
    ProjectsAdmin* projectsAdmin = new ProjectsAdmin;
    projectsAdmin->showMaximized();
}

void adminstrator::previousWindow()
{
    hide();
    MainWindow* mainWindow = new MainWindow;
    mainWindow->showMaximized();
}


adminstrator::~adminstrator()
{
    delete ui;
}
