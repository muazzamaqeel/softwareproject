#include "parentboard.h"
#include "pb_productbacklog_implementation.h"
#include "pb_sprint_implemenation.h"
#include "ui_parentboard.h"
#include "mainwindow.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QDebug>
#include <QTableView>
#include "pb_confluence_implemenation.h"
#include "pb_productbacklog_implementation.h"
#include "pb_team_implemenation.h"
#include "pb_taskboard_implemenation.h"

parentboard* parentboard::instance = nullptr;


parentboard::parentboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::parentboard),
    scrollAreaLayout(new QVBoxLayout(this))
{
    ui->setupUi(this);
    isTeamTableActive = false;

    taskModel = new QStandardItemModel(this);
    taskModel->setColumnCount(2);
    ui->teamTable->setColumnCount(3);
    ui->teamTable->setHorizontalHeaderLabels({"First name", "Email", "Role"});





    //ui->user_stories->setColumnCount(3); // Add a new column for priority
    //ui->user_stories->setHorizontalHeaderLabels({"Userstory", "Description", "Priority"}); // Set column headers
    //ui->issues->setColumnCount(3); // Add a new column for priority
    //ui->issues->setHorizontalHeaderLabels({"Issue", "Description", "Priority"}); // Set column headers



    //  PB_ProductBacklog ---------------------------------- Calls
    //  PB_ProductBacklog ---------------------------------- Calls
    parentboard *obj = this; // Create an instance of parentboard
    pb_team_implemenation *teamPagePtr = new pb_team_implemenation(obj);
    pb_productbacklog_implementation *pbProductBacklogObj = new pb_productbacklog_implementation(obj);
    pb_sprint_implemenation *pbSprintObj = new pb_sprint_implemenation(obj);
    pbProductBacklogObj->RetrieveAndDisplayBacklog();
    pbProductBacklogObj->Hide_CreateSection();

    connect(ui->backlogButton, &QPushButton::clicked, pbProductBacklogObj, &pb_productbacklog_implementation::RetrieveAndDisplayBacklog);
    connect(ui->button_userstory, &QPushButton::clicked, pbProductBacklogObj, &pb_productbacklog_implementation::Show_CreateSection);
    connect(ui->buttton_issue, &QPushButton::clicked, pbProductBacklogObj, &pb_productbacklog_implementation::Show_CreateSection);
    connect(ui->button_userstory, &QPushButton::clicked, pbProductBacklogObj, &pb_productbacklog_implementation::Show_CreateSection_UserStory);
    connect(ui->buttton_issue, &QPushButton::clicked, pbProductBacklogObj, &pb_productbacklog_implementation::Show_CreateSection_Tasks);
    //CREATE USERSTORY AND TASKS
    connect(ui->Button_CreateTask, &QPushButton::clicked, pbProductBacklogObj, &pb_productbacklog_implementation::onButtonIssueClicked);
    connect(ui->Button_CreateUserStory, &QPushButton::clicked, pbProductBacklogObj, &pb_productbacklog_implementation::on_createUserStory_clicked);
    //connect(ui->CreateSprint_SprintTab, &QPushButton::clicked, pbProductBacklogObj, &pb_productbacklog_implementation::on_createUserStory_clicked);
    connect(ui->user_stories, &QTableWidget::itemChanged, pbProductBacklogObj, &pb_productbacklog_implementation::onTableItemChanged);
    connect(ui->user_stories, &QTableWidget::itemChanged, pbProductBacklogObj, &pb_productbacklog_implementation::onUserStoryTableItemChanged);
    //Lambda Function to delete the tasks in the tables
    connect(ui->delete_item, &QPushButton::clicked, [this, pbProductBacklogObj]() {
        if (!ui->user_stories) {
            qDebug() << "User stories table not found";
            return;
        }
        int selectedRow = -1;
        QList<QTableWidgetItem*> selectedItems = ui->user_stories->selectedItems();
        if (!selectedItems.isEmpty()) {
            selectedRow = selectedItems.first()->row();
        }
        if (selectedRow == -1) {
            qDebug() << "No row selected";
            return;
        }
        QTableWidgetItem* item = ui->user_stories->item(selectedRow, 0); // 0 is the column for taskID
        if (item) {
            pbProductBacklogObj->onRowClicked(item);
            pbProductBacklogObj->RetrieveAndDisplayBacklog();
        } else {
            qDebug() << "Item is null";
            // Optionally, handle this case
        }

    });
    //  pb_productbacklog Implementation Calls ---End





    //  PB_Sprint  ---------------------------------- Calls
    //  PB_Sprint  ---------------------------------- Calls

    pb_sprint_implemenation *pbSprintBObj = new pb_sprint_implemenation(obj); //Accessing the pb_sprint_implementation class
    connect(ui->sprintsButton,&QPushButton::clicked,pbSprintBObj,&pb_sprint_implemenation::OBJECTS_VISIBLE_BOXES);
    connect(ui->sprintsButton,&QPushButton::clicked,pbSprintBObj,&pb_sprint_implemenation::RetrieveAndDisplayTask);
    //connect(ui->sprint_createtask_button, &QPushButton::clicked, pbSprintBObj, &pb_sprint_implemenation::addTaskSprint);
    connect(ui->sprint_create_button, &QPushButton::clicked, pbSprintBObj, &pb_sprint_implemenation::OBJECTS_VISIBLE_on_create_sprint_clicked); //TO JUST MAKE THE CORRECT OBJECTS VISIBLE
    connect(ui->edit_sprint_button, &QPushButton::clicked, pbSprintBObj, &pb_sprint_implemenation::on_editsprint_sprint_clicked);
    //Create Task
    connect(ui->CreateSprint_SprintTab, &QPushButton::clicked, pbSprintObj, &pb_sprint_implemenation::on_createtask_sprint_clicked);
    connect(ui->delete_button,&QPushButton::clicked,pbSprintBObj,&pb_sprint_implemenation::onDeleteButtonClicked);
    // Connect the delete button click signal to the onDeleteButtonClicked() slot
    // connect(ui->delete_button, SIGNAL(clicked()), this, SLOT(onDeleteButtonClicked()));







    //  PB_Confluence  ---------------------------------- Calls
    //  PB_Confluence  ---------------------------------- Calls

    PB_Confluence_Implemenation *pbConfluenceBObj = new PB_Confluence_Implemenation(obj); //Accessing the pb_sprint_implementation class
    connect(ui->confluenceButton,&QPushButton::clicked,pbConfluenceBObj,&PB_Confluence_Implemenation::HideCreationSection);
    connect(ui->ButtonGroup_Confluence,&QPushButton::clicked,pbConfluenceBObj,&PB_Confluence_Implemenation::ShowCreateWindow);
    connect(ui->ButtonPage_Confluence,&QPushButton::clicked,pbConfluenceBObj,&PB_Confluence_Implemenation::ShowCreateWindow_Page);

    connect(ui->Button_Create_Confluence,&QPushButton::clicked,pbConfluenceBObj,&PB_Confluence_Implemenation::CreateGroupFunction);
    connect(ui->Button_CreatePage_Confluence,&QPushButton::clicked,pbConfluenceBObj,&PB_Confluence_Implemenation::CreatePageFunction);

    PB_Confluence_Implemenation* myConfluenceImplementation = new PB_Confluence_Implemenation(this);
    connect(ui->userTaskTreeWidget_2, &QTreeWidget::itemClicked, myConfluenceImplementation, &PB_Confluence_Implemenation::onTreeItemClicked);




    //  PB_Team  ---------------------------------- Calls
    //  PB_Team  ---------------------------------- Calls

    connect(ui->CreateUser_Button, &QPushButton::clicked, teamPagePtr, &pb_team_implemenation::on_createuser_clicked);
    connect(ui->teamButton, &QPushButton::clicked, teamPagePtr, &pb_team_implemenation::UserRetrieval);

    //Old Way
    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(goBackToMainWindow()));
    connect(ui->button_userstory, SIGNAL(clicked()), this, SLOT(test()));

    //  PB_TaskBoard ---------------------------------- Calls
    //  PB_TaskBoard ---------------------------------- Calls















    pb_taskboard_implemenation *pbTaskboardBObj = new pb_taskboard_implemenation(obj);
}





//------------------------------------------------------------------------------------------------------------------------------------------------
parentboard::~parentboard()
{
    delete ui;
}

//------------------------------------------------------------------------------------------------------------------------------------------------
//Top Bar Button's Functions
//------------------------------------------------------------------------------------------------------------------------------------------------

void parentboard::on_taskboardButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    const int numOfRowsToAdd = 3;
}
void parentboard::on_Issue_selected(){

}
void parentboard::on_backlogButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    const int numOfRowsToAdd = 3;
}
void parentboard::on_teamButton_clicked(){
    ui->stackedWidget->setCurrentIndex(2);
}
void parentboard::on_sprintsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    const int numOfRowsToAdd = 3;
}
void parentboard::on_confluenceButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void parentboard::goBackToMainWindow() {
    MainWindow* mainWindow = new MainWindow; // Create a new instance of the main window
    hide(); // Hide the registration window
    mainWindow->showMaximized(); // Show the main window
    ui->~parentboard();
}

int parentboard::setProjectId(int id) {
    currentProjectId = id;
    qDebug() << "ProjectID in ParentBoard: " << currentProjectId;
    return currentProjectId;
}

int parentboard::getProjectId() const {
    return currentProjectId;
}
//------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------
//Public Access to the Objects of the parentboard.ui components for the other classes
//------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------Blacklog----------------------------------------------------
QTableWidget* parentboard::getUserStoriesTableView() {
    return ui->user_stories;
}
QPushButton* parentboard::getButton_CreateUserStory(){
    return ui->Button_CreateUserStory;
}
QPushButton* parentboard::getButton_CreateTask(){
    return ui->Button_CreateTask;
}
QTextBrowser* parentboard::getCreate_Assignee(){
    return ui->CreateAssignee;
}
QTextBrowser* parentboard::getCreate_Description(){
    return ui->CreateDescription;
}
QTextBrowser* parentboard::getCreate_Header(){
    return ui->CreateHeader;
}
QTextBrowser* parentboard::getCreate_Priority(){
    return ui->CreatePriority;
}
QTextBrowser* parentboard::getCreate_Status(){
    return ui->CreateStatus;
}
QTextBrowser* parentboard::getCreate_Title(){
    return ui->CreateTitle;
}
QComboBox* parentboard::getInputAssignee(){
    return ui->InputAssignee;
}
QTextEdit* parentboard::getInputDescription(){
    return ui->InputDescription;
}
QTextEdit* parentboard::getInputPriority(){
    return ui->InputPriority;
}
QTextEdit* parentboard::getInputStatus(){
    return ui->InputStatus;
}
QTextEdit* parentboard::getInputTitle(){
    return ui->InputTitle;
}
QTableWidget* parentboard::getTeamTableView(){
    return ui->teamTable;
}

QComboBox* parentboard::get_BL_SprintDropDown(){
    return ui->BL_SprintDropDown;

}
QTextBrowser* parentboard::get_SelecteSprint(){
    return ui->SelecteSprint;
}
QGroupBox* parentboard::getCreationBox(){
    return ui->CreationBox;

}
QPushButton* parentboard::get_delete_item(){
    return ui->delete_item;
}


//------------------------------------------------------Sprint--------------------------------------------------------
QTableWidget* parentboard::getSprintTableView() {
    return ui->sprint_table;
}
QComboBox* parentboard::getSprint_Top_Down(){
    return ui->sprint_top_down;
}
QTextBrowser* parentboard::getSprintDate(){
    return ui->sprint_date;
}
QLineEdit* parentboard::getFirstNameField(){
    return ui->input_firstname;
}
QGroupBox* parentboard::getSprintGroupBox() const {
    return ui->sprintGroupBox;
}

QLineEdit* parentboard::get_Input_SprintName(){
    return ui->Input_SprintName;
}
QDateEdit* parentboard::get_Input_StartDate(){
    return ui->Input_StartDate;
}
QDateEdit* parentboard::get_Input_EndDate(){
    return ui->Input_EndDate;
}
QPushButton* parentboard::get_Button_CreateSprint(){
    return ui->CreateSprint_SprintTab;
}


//Sprint_View_Box

QGroupBox* parentboard::get_Sprint_View_Box() {
    return ui->Sprint_View_Box;
}
QLineEdit* parentboard::get_Line_EditSprintEndDate() {
    return ui->Edit_Sprint_EndDate;
}
QLineEdit* parentboard::get_LineEdit_SprintName() {
    return ui->Edit_Sprint_Name;
}

QLineEdit* parentboard::get_LineEdit_SprintStartDate() {
    return ui->Edit_Sprint_StartDate;
}

QLabel* parentboard::get_Label_Sprint_EndDate() {
    return ui->Label_Sprint_EndDate;
}

QLabel* parentboard::get_Label_SprintStartDate() {
    return ui->Label_Sprint_StartDate;
}

QLabel* parentboard::get_Label_SprintTitle() {
    return ui->Label_Sprint_Title;
}

QLabel* parentboard::get_Label_TaskName() {
    return ui->Label_Task_TableName;
}

QTableWidget* parentboard::get_Table_SprintDetails() {
    return ui->SprintDetails_Table;
}

//Sprint Group Box
QGroupBox* parentboard::get_GroupBox_SprintGroup() {
    return ui->sprintGroupBox;
}

QPushButton* parentboard::get_Button_CreateSprintTab() {
    return ui->CreateSprint_SprintTab;
}

QDateEdit* parentboard::get_DateEdit_InputEndDate() {
    return ui->Input_EndDate;
}

QTextEdit* parentboard::get_TextEdit_InputSprintGoal() {
    return ui->Input_SprintGoal;
}

QLineEdit* parentboard::get_LineEdit_InputSprintName() {
    return ui->Input_SprintName;
}

QDateEdit* parentboard::get_DateEdit_InputStartDate() {
    return ui->Input_StartDate;
}

QLabel* parentboard::get_DateEdit_Text_EndDate() {
    return ui->Text_EndDate;
}
QLabel* parentboard::get_Label_TextName() {
    return ui->Text_Name;
}

QLabel* parentboard::get_Label_TextSprintGoal() {
    return ui->Text_SprintGoal;
}

QLabel* parentboard::get_Label_TextStartDate() {
    return ui->Text_StartDate;
}
















//Confluence

QTreeWidget* parentboard::getuserTaskTreeWidget_2(){
    return ui->userTaskTreeWidget_2;
}

QTextEdit* parentboard::get_InputTitle_2() {
    return ui->InputTitle_2;
}
QComboBox* parentboard::get_InputGroup_Confluence_Heading() {
    return ui->InputGroup_Confluence_Heading;
}
QTextBrowser* parentboard::get_Confluence_NameText() {
    return ui->Confluence_NameText;
}
QTextBrowser* parentboard::get_Confluence_HeadingText() {
    return ui->Confluence_HeadingText;
}
QTextBrowser* parentboard::get_Confluence_GroupText() {
    return ui->Confluence_GroupText;
}
QGroupBox* parentboard::getCreationBox_Confluence() const {
    return ui->CreationBox_Confluence;
}

//BAR-BUTTONS
QPushButton* parentboard::get_ButtonGroup_Confluence(){
    return ui->ButtonGroup_Confluence;
}
QPushButton* parentboard::get_ButtonPage_Confluence(){
    return ui->ButtonPage_Confluence;
}
//LOWER-BUTTONS
QPushButton* parentboard::get_Button_CreatePage_Confluence(){
    return ui->Button_CreatePage_Confluence;
}
QPushButton* parentboard::get_Button_Create_Confluence() {
    return ui->Button_Create_Confluence;
}


// ----------------------------------------------------Taskboard----------------------------------------------------
QComboBox* parentboard::getSprintDropdown() const
{
    return ui->sprintDropdownT;
}
QVBoxLayout* parentboard::getScrollAreaLayout() const
{
    return scrollAreaLayout;
}
QTextEdit* parentboard::getTaskboardTextEdit() const {
    return taskboardTextEdit;
}
QTreeWidget* parentboard::getTaskTreeWidget() const {
    return ui->userTaskTreeWidget;
}
void parentboard::updateSprintDateLabel(const QString& newText)
{
    ui->labelDateT->setText(newText);
}








//------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------



