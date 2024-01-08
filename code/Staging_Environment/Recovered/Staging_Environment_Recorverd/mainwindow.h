#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlineedit.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QLineEdit* getInput_Username();
    QLineEdit* getInput_Password();
    QString inputUsername;
    QString Pass_inputUsername;
    QString PassUsername();


private:
    Ui::MainWindow *ui;
    QString password;
    QString username;

public slots:
    void openIssueWindow();
    void userEncryptedLogin();
    void openRegistrationWindow();
    void TeamMember_ProjectsWin();
    void closeApp();
    void adminLogin();
    void openParentBoard();
    void onSignInButtonClicked();
    void setPassUsername(const QString& username);
private slots:
    void openSettings();


};
#endif // MAINWINDOW_H
