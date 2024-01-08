#ifndef TEAMMEMBER_PROJECTSWINDOW_H
#define TEAMMEMBER_PROJECTSWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QSqlDatabase>
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
    void addProject(const QString& projectName, const QString& description, int idProject);
    void onProjectNameClicked(QTableWidgetItem *item);
    int getUserIdFromUsername(const QString& USERTEST, const QSqlDatabase& db);
    void setUsername(const QString& username);


private:
    Ui::TeamMember_ProjectsWindow *ui;
    QString m_username; // This is where you declare the m_username variable
};

#endif // TEAMMEMBER_PROJECTSWINDOW_H
