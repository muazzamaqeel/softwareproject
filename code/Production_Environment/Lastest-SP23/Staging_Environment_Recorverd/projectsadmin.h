#ifndef PROJECTSADMIN_H
#define PROJECTSADMIN_H

#include <QWidget>

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

private:
    Ui::ProjectsAdmin *ui;
};

#endif // PROJECTSADMIN_H
