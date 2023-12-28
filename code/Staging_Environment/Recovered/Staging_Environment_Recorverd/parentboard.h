#ifndef PARENTBOARD_H
#define PARENTBOARD_H
#include "QtWidgets/qboxlayout.h"
#include "QTreeWidget"
#include "qcombobox.h"
#include "qgroupbox.h"
#include "qpushbutton.h"
#include "qtextbrowser.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QTableWidget> // Include QTableWidget header
#include <QLineEdit>
#include <QLabel>
#include <QDateEdit>

namespace Ui {
class parentboard;
}

class parentboard : public QWidget
{
    Q_OBJECT

public:
    bool isTeamTableActive;
    explicit parentboard(QWidget *parent = nullptr);
    ~parentboard();
    QWidget* getSomeWidget(); // Declare the function
    QTableWidget* getUserStoriesTableView();
    QTableWidget* getIssuesTableView();
    QComboBox* getSprint_Top_Down();
    QPushButton* on_create_sprint_clicked();
    QTableWidget* getSprintTableView();
    QTableWidget* getTeamTableView();
    QTextBrowser *getSprintDate();
    QLineEdit *getFirstNameField();

    QGroupBox* getSprintGroupBox() const;
    //QComboBox* getSprintComboBox();


    QPushButton* getButton_Create();
    QTextBrowser* getCreate_Assignee();
    QTextBrowser* getCreate_Description();
    QTextBrowser* getCreate_Header();
    QTextBrowser* getCreate_Priority();
    QTextBrowser* getCreate_Status();
    QTextBrowser* getCreate_Title();

    QComboBox* getInputAssignee();
    QTextEdit* getInputDescription();
    QTextEdit* getInputPriority();
    QTextEdit* getInputStatus();
    QTextEdit* getInputTitle();
    QPushButton* getButton_CreateTask();
    QPushButton* getButton_CreateUserStory();
    QPushButton* get_delete_item();
    QComboBox* get_BL_SprintDropDown();
    QTextBrowser* get_SelecteSprint();

    QLineEdit* get_Input_SprintName();
    QDateEdit* get_Input_StartDate();
    QDateEdit* get_Input_EndDate();
    QPushButton* get_Button_CreateSprint();
    QGroupBox* getCreationBox();
    QPushButton* get_ButtoCreatePage_Confluence();


    //Confluence
    QTextEdit* get_InputTitle_2();
    QComboBox* get_InputGroup_Confluence_Heading();
    QTextEdit* get_InputDescription_Confluence_Heading();
    QTextBrowser* get_Confluence_NameText();
    QTextBrowser* get_Confluence_HeadingText();
    QTextBrowser* get_Confluence_GroupText();
    QTextBrowser* get_Confluence_DescriptionText();
    QPushButton* get_Button_Create_Confluence();
    QTreeWidget* getuserTaskTreeWidget_2();
    QGroupBox* getCreationBox_Confluence() const;

    static parentboard* getInstance(QWidget *parent = nullptr) {
        if (instance == nullptr) {
            instance = new parentboard(parent);
        }
        return instance;
    }

    int setProjectId(int id);
    int getProjectId() const;

    // Taskboard
    QComboBox* getSprintDropdown() const;
    QVBoxLayout* getScrollAreaLayout() const;
    QTextEdit* getTaskboardTextEdit() const;
    QTreeWidget* getTaskTreeWidget() const;
    void updateSprintDateLabel(const QString& newText);


private slots:
    void on_taskboardButton_clicked();
    void on_Issue_selected();
    void on_teamButton_clicked();
    void on_backlogButton_clicked();
    void on_sprintsButton_clicked();
    void on_confluenceButton_clicked();
    void goBackToMainWindow();
private:

    Ui::parentboard *ui;
    QStandardItemModel *taskModel;
    int currentProjectId;
    static parentboard* instance;  // Static instance

    // Taskboard
    QVBoxLayout *scrollAreaLayout;
    QTextEdit* taskboardTextEdit;
    QTreeWidget* userTaskTreeWidget;

};

#endif // PARENTBOARD_H
