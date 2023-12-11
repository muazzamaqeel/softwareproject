#ifndef TEAMMEMBER_PROJECTSWINDOW_H
#define TEAMMEMBER_PROJECTSWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

namespace Ui {
class TeamMember_ProjectsWindow;
}

class TeamMember_ProjectsWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit TeamMember_ProjectsWindow(QWidget *parent = nullptr);
    ~TeamMember_ProjectsWindow();

public slots:
    void RetrieveAndDisplayUser_Project();
    void Clear_User_Project();
    void ProjectRetrieval();
    void addProject(const QString& taskName, const QString& description);
    void onProjectNameClicked(QTableWidgetItem *item);

private:
    Ui::TeamMember_ProjectsWindow *ui;

};

#endif // TEAMMEMBER_PROJECTSWINDOW_H
