#include "teammember_projectswindow.h"
#include "ui_teammember_projectswindow.h"
#include "databasemanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>
#include "parentboard.h"
#include "mainwindow.h"

TeamMember_ProjectsWindow::TeamMember_ProjectsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeamMember_ProjectsWindow) {

    ui->setupUi(this);
    connect(ui->BackButton, SIGNAL(clicked()), this, SLOT(RetrieveAndDisplayUser_Project()));

    // Initialize table columns and headers
    ui->tm_project_table->setColumnCount(2);
    QStringList headers = {"Project Name", "Description"};
    ui->tm_project_table->setHorizontalHeaderLabels(headers);

    // Set the horizontal header to stretch the columns
    QHeaderView *header = ui->tm_project_table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    // Increase font size
    QFont font = ui->tm_project_table->font();
    font.setPointSize(12); // Set your desired font size
    ui->tm_project_table->setFont(font);

    // Modernize the design using style sheets
    ui->tm_project_table->setStyleSheet(
        "QTableView {"
        "   border: 1px solid #CCCCCC;"
        "   selection-background-color: #87CEEB;"
        "}"
        "QHeaderView::section {"
        "   background-color: #F0F0F0;"
        "   padding: 4px;"
        "   border: 1px solid #CCCCCC;"
        "   font-size: 12pt;"  // Adjust header font size
        "}"
        );

    RetrieveAndDisplayUser_Project();

    connect(ui->tm_project_table, SIGNAL(itemClicked(QTableWidgetItem*)),
            this, SLOT(onProjectNameClicked(QTableWidgetItem*)));
}


TeamMember_ProjectsWindow::~TeamMember_ProjectsWindow() {
    delete ui;
}

void TeamMember_ProjectsWindow::RetrieveAndDisplayUser_Project() {
    Clear_User_Project();
    ProjectRetrieval();
}

void TeamMember_ProjectsWindow::Clear_User_Project() {
    ui->tm_project_table->clearContents();
    ui->tm_project_table->setRowCount(0);
}

void TeamMember_ProjectsWindow::setUsername(const QString& username) {
    m_username = username; // This should match the declaration in the header
}

void TeamMember_ProjectsWindow::ProjectRetrieval() {
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    qDebug() << "Username: " << m_username; // Use the member variable

    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);

        // Adjust the query to filter by the username
        query.prepare("SELECT Project.idProject, Project.ProjectName, Project.Description "
                      "FROM Project "
                      "INNER JOIN Project_has_User ON Project.idProject = Project_has_User.Project_idProject "
                      "INNER JOIN User ON Project_has_User.User_idUser = User.idUser "
                      "WHERE User.Username = :validUsername");
        query.bindValue(":validUsername", m_username);



            if (query.exec()) {
                while (query.next()) {
                    int idProject = query.value(0).toInt(); // idProject is now the first column
                    QString projectName = query.value(1).toString();
                    QString description = query.value(2).toString();
                    int idUser = query.value(2).toInt();
                    qDebug() << "Project: " << projectName << ", Description: " << description << ", User ID: " << idUser;
                    addProject(projectName, description, idProject);
                }
            } else {
                qDebug() << "Failed to retrieve data:" << query.lastError().text();
            }
            dbobj.close();
        } else {
            qDebug() << "Database connection not established.";
        }

    }

void TeamMember_ProjectsWindow::addProject(const QString& projectName, const QString& description, int idProject) {
    int row = ui->tm_project_table->rowCount();
    ui->tm_project_table->insertRow(row);

    QTableWidgetItem *nameItem = new QTableWidgetItem(projectName);
    nameItem->setData(Qt::UserRole, QVariant(idProject)); // Store the idProject

    QTableWidgetItem *descriptionItem = new QTableWidgetItem(description);

    ui->tm_project_table->setItem(row, 0, nameItem);
    ui->tm_project_table->setItem(row, 1, descriptionItem);
}



void TeamMember_ProjectsWindow::onProjectNameClicked(QTableWidgetItem *item) {
    if (item && item->column() == 0) {
        int idProject = item->data(Qt::UserRole).toInt();
        this->deleteLater();
        parentboard* parentboardwindow = parentboard::getInstance();
        parentboardwindow->setProjectId(idProject);
        parentboardwindow->showMaximized();
    }
}

int TeamMember_ProjectsWindow::getUserIdFromUsername(const QString& USERTEST, const QSqlDatabase& db) {
    qDebug() <<"Hello";
    return 1;
}

