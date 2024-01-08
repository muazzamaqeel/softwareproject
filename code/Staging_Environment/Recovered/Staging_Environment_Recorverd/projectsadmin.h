#ifndef PROJECTSADMIN_H
#define PROJECTSADMIN_H

#include <QWidget>
#include "adminstrator.h"
#include <QStandardItemModel>
#include <QTableWidget> // Include QTableWidget header

namespace Ui {
class ProjectsAdmin;
}

class ProjectsAdmin : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectsAdmin(QWidget *parent = nullptr);
    ~ProjectsAdmin();
public slots:
    void openMainWindowfromAdmin();
    void openProjectCreation();
    void openProjectCreation1();
    void deleteProject();
    void addProject(const QString& taskName, const QString& description);
    void deleteProjectFromDatabase(const QString& projectName);
    void ProjectRetrieval();
    void RetrieveAndDisplayProjectBacklog();
    void clearProjectTable();
    void addProjectToDatabase(const QString& taskName, const QString& description);

private:
    Ui::ProjectsAdmin *ui;
};

#endif // PROJECTSADMIN_H

