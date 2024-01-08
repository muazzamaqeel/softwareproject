#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QDebug> // Include qDebug for debugging
#include <QTableView>
#include "databasemanager.h"
#include "pb_productbacklog_implementation.h"
#include "qheaderview.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include <QRegularExpression>
#include <QFont>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QMessageBox>

pb_productbacklog_implementation::pb_productbacklog_implementation(parentboard* parentBoardInstance) {
    // Initialize any necessary variables or connections
    parentBoard = parentBoardInstance;
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    userStoriesTable->setGeometry(20, 55, 1250, 500);

    QHeaderView* headerView = userStoriesTable->horizontalHeader();
    connect(headerView, SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));
    parentBoard->getCreationBox()->setVisible(false);


}
void pb_productbacklog_implementation::clearUserStoriesTable() {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    QComboBox* SprintComboBox = parentBoard->get_BL_SprintDropDown();


    userStoriesTable->clearContents();
    userStoriesTable->setRowCount(0);
    SprintComboBox->clear();
    //QComboBox* SprintBox = parentBoard->getSprintComboBox();
    //SprintBox->clear();
}
void pb_productbacklog_implementation::RetrieveAndDisplayBacklog() {

    qDebug() <<"RetrieveAndDisplayBacklog Called";
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    userStoriesTable->setGeometry(20, 55, 1250, 500);
    parentBoard->getCreationBox()->setVisible(false);

    if (userStoriesTable) {
        userStoriesTable->blockSignals(true); // Block signals
        Hide_CreateSection();
        clearUserStoriesTable(); // Clears the table before adding new entries
        TaskPBretrieval(); // Retrieves tasks and adds them to the table
        UserStoryPBretrieval();
        BL_fetechSprints();
        // SendTasksToSprints();
        userStoriesTable->blockSignals(false); // Unblock signals
    }
}


void pb_productbacklog_implementation::Hide_CreateSection(){

    /*
    QPushButton* buttonCreate = parentBoard->getButton_Create();
    QTextBrowser* createAssignee = parentBoard->getCreate_Assignee();
    QTextBrowser* createDescription = parentBoard->getCreate_Description();
    QTextBrowser* createHeader = parentBoard->getCreate_Header();
    QTextBrowser* createPriority = parentBoard->getCreate_Priority();
    QTextBrowser* createStatus = parentBoard->getCreate_Status();
    QTextBrowser* createTitle = parentBoard->getCreate_Title();

    QTextEdit* inputAssignee = parentBoard->getInputAssignee();
    QTextEdit* inputDescription = parentBoard->getInputDescription();
    QTextEdit* inputPriority = parentBoard->getInputPriority();
    QTextEdit* inputStatus = parentBoard->getInputStatus();
    QTextEdit* inputTitle = parentBoard->getInputTitle();
*/
    parentBoard->getCreate_Assignee()->setVisible(false);
    parentBoard->getCreate_Description()->setVisible(false);
    parentBoard->getCreate_Header()->setVisible(false);
    parentBoard->getCreate_Priority()->setVisible(false);
    parentBoard->getCreate_Status()->setVisible(false);
    parentBoard->getCreate_Title()->setVisible(false);

    parentBoard->getInputAssignee()->setVisible(false);
    parentBoard->getInputDescription()->setVisible(false);
    parentBoard->getInputPriority()->setVisible(false);
    parentBoard->getInputStatus()->setVisible(false);
    parentBoard->getInputTitle()->setVisible(false);
    parentBoard->getButton_CreateUserStory()->setVisible(false);
    parentBoard->getButton_CreateTask()->setVisible(false);

    parentBoard->get_BL_SprintDropDown()->setVisible(false);
    parentBoard->get_SelecteSprint()->setVisible(false);

}



void pb_productbacklog_implementation::Show_CreateSection(){
    parentBoard->getCreationBox()->setVisible(true);
    parentBoard->getCreate_Assignee()->setVisible(true);
    parentBoard->getCreate_Description()->setVisible(true);
    parentBoard->getCreate_Header()->setVisible(true);
    parentBoard->getCreate_Priority()->setVisible(true);
    parentBoard->getCreate_Status()->setVisible(true);
    parentBoard->getCreate_Title()->setVisible(true);
    parentBoard->getInputAssignee()->setVisible(true);
    parentBoard->getInputDescription()->setVisible(true);
    parentBoard->getInputPriority()->setVisible(true);
    parentBoard->getInputStatus()->setVisible(true);
    parentBoard->getInputTitle()->setVisible(true);
    parentBoard->get_BL_SprintDropDown()->setVisible(true);
    parentBoard->get_SelecteSprint()->setVisible(true);


}

void pb_productbacklog_implementation::Show_CreateSection_UserStory(){

    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    userStoriesTable->setGeometry(20, 55, 750, 500);

    parentBoard->getCreationBox()->setVisible(true);
    parentBoard->getButton_CreateUserStory()->setVisible(true);
    parentBoard->getButton_CreateTask()->setVisible(false);

    parentBoard->get_BL_SprintDropDown()->setVisible(false);
    parentBoard->get_SelecteSprint()->setVisible(false);

    QTextBrowser* Create_Header = parentBoard->getCreate_Header();
    Create_Header->setAlignment(Qt::AlignCenter);  // Align text to center
    Create_Header->setHtml("<html><head/><body><p style='font-size:12pt; text-align:center;'>Create User Stories</p></body></html>");

}

void pb_productbacklog_implementation::Show_CreateSection_Tasks(){

    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    userStoriesTable->setGeometry(20, 55, 750, 500);
    parentBoard->getCreationBox()->setVisible(true);
    parentBoard->getButton_CreateUserStory()->setVisible(false);
    parentBoard->getButton_CreateTask()->setVisible(true);
    parentBoard->get_BL_SprintDropDown()->setVisible(true);
    parentBoard->get_SelecteSprint()->setVisible(true);
    QTextBrowser* Create_Header = parentBoard->getCreate_Header();
    Create_Header->setAlignment(Qt::AlignCenter);  // Align text to center
    Create_Header->setHtml("<html><head/><body><p style='font-size:12pt; text-align:center;'>Create User Tasks</p></body></html>");

}



//------------------------------------------------------------------------------------------------------------------------------
//FULL IMPLEMENTATION OF CREATION OF TASK AND EDIT FUCNTIONALITY    -----    START
//------------------------------------------------------------------------------------------------------------------------------


void pb_productbacklog_implementation::TaskPBretrieval() {
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    int PassedProjectID = parentBoard->getProjectId();
    qDebug() << "Project ID in Product Backlog: " << PassedProjectID;

    if (dbobj.isOpen()) {
        // First, retrieve the sprint titles.
        QStringList sprintTitles;
        QSqlQuery sprintQuery(dbobj);
        sprintQuery.prepare("SELECT Title FROM Sprint WHERE Project_idProject = :projectID");
        sprintQuery.bindValue(":projectID", PassedProjectID);
        if (sprintQuery.exec()) {
            while (sprintQuery.next()) {
                sprintTitles << sprintQuery.value(0).toString();
            }
        } else {
            qDebug() << "Failed to retrieve sprints: " << sprintQuery.lastError().text();
        }


        // Retrieve assignees from the database
        QStringList assigneeList;
        QSqlQuery queryUser(dbobj);
        queryUser.prepare("SELECT User.Username "
                          "FROM Project "
                          "INNER JOIN Project_has_User ON Project.idProject = Project_has_User.Project_idProject "
                          "INNER JOIN User ON Project_has_User.User_idUser = User.idUser "
                          "WHERE Project_has_User.Project_idProject = :projectID");
        queryUser.bindValue(":projectID", PassedProjectID);
        if (queryUser.exec()) {
            while (queryUser.next()) {
                assigneeList << queryUser.value(0).toString();
            }
        } else {
            qDebug() << "Error executing query:" << queryUser.lastError().text();
        }

        // Now retrieve the tasks.
        QSqlQuery taskQuery(dbobj);
        taskQuery.prepare("SELECT idTaskPB, Title, Description, Status, Assignee, Priority, ProductBacklog_idProductBacklog, ProductBacklog_Project_idProject, AssignedSprint FROM scrummy.TaskPB WHERE ProductBacklog_Project_idProject = :projectID");
        taskQuery.bindValue(":projectID", PassedProjectID);

        if (taskQuery.exec()) {
            qDebug() << "Tasks Retrieved Successfully!";
            while (taskQuery.next()) {
                int taskID = taskQuery.value(0).toInt();
                qDebug() << "Retrieved Task ID:" << taskID;
                QString title = taskQuery.value(1).toString();
                QString description = taskQuery.value(2).toString();
                QString status = taskQuery.value(3).toString();
                QString assignee = taskQuery.value(4).toString();
                int priority = taskQuery.value(5).toInt();
                QString assignedSprint = taskQuery.value(8).toString(); // This assumes AssignedSprint is the ninth column

                // Include sprintTitles as the last argument
                Tasks_Added_In_Table("Task", title, description, status, assignee, priority, taskID, sprintTitles, assignedSprint, assigneeList);

                // Store additional information in taskMap, if necessary
                taskMap[taskID] = {title, description, status, assignee, priority, taskID};
            }
        } else {
            qDebug() << "Failed to retrieve tasks: " << taskQuery.lastError().text();
        }
        dbobj.close();
    } else {
        qDebug() << "Connection Not Established - pb_productbacklog_implementation class! - Task";
    }
}


void pb_productbacklog_implementation::Tasks_Added_In_Table(const QString& type_pb, const QString& taskName, const QString& description, const QString& status, QString assignee, int priority, int taskID, const QStringList& sprintTitles, const QString& assignedSprint, const QStringList& assigneeList) {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    if (!userStoriesTable) {
        qDebug() << "Table view not found or accessible.";
        return;
    }

    userStoriesTable->setColumnCount(8); // Include an additional column for Task ID
    userStoriesTable->setHorizontalHeaderLabels({"ID", "Type", "Title", "Description", "Status", "Assignee", "Priority", "Sprint"});
    userStoriesTable->setColumnHidden(0, true);

    QHeaderView* header = userStoriesTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    int rowCount = userStoriesTable->rowCount();
    userStoriesTable->insertRow(rowCount);

    QTableWidgetItem* itemTaskID = new QTableWidgetItem();
    itemTaskID->setData(Qt::UserRole, QVariant(taskID));
    itemTaskID->setText(QString::number(taskID));
    userStoriesTable->setItem(rowCount, 0, itemTaskID);

    QTableWidgetItem* type = new QTableWidgetItem(type_pb);
    type->setFlags(type->flags() & ~Qt::ItemIsEditable);
    userStoriesTable->setItem(rowCount, 1, type);

    QTableWidgetItem* itemTaskName = new QTableWidgetItem(taskName);
    userStoriesTable->setItem(rowCount, 2, itemTaskName);

    QTableWidgetItem* itemDescription = new QTableWidgetItem(description);
    userStoriesTable->setItem(rowCount, 3, itemDescription);

    QComboBox* statusComboBox = new QComboBox();
    statusComboBox->addItems({"ToDo", "InProgress", "Blocked", "Done"});
    statusComboBox->setCurrentText(status);
    userStoriesTable->setCellWidget(rowCount, 4, statusComboBox);
    connect(statusComboBox, &QComboBox::currentTextChanged,
            [this, taskID](const QString &newStatus) {
                onStatusChanged(taskID, newStatus);
            });



    QComboBox* assigneeComboBox = new QComboBox();
    assigneeComboBox->addItems(assigneeList);
    assigneeComboBox->setCurrentText(assignee); // Set the current assignee
    userStoriesTable->setCellWidget(rowCount, 5, assigneeComboBox);

    // Connect the ComboBox signal to a slot
    connect(assigneeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, taskID, assigneeComboBox](int index) {
                QString newAssignee = assigneeComboBox->currentText();
                onTaskAssigneeChanged(taskID, newAssignee);
            });


    QTableWidgetItem* itemPriority = new QTableWidgetItem(QString::number(priority));
    userStoriesTable->setItem(rowCount, 6, itemPriority);

    SprintSelectionComboBox = new QComboBox();
    SprintSelectionComboBox->addItems(sprintTitles);
    // Set the current text of the combo box to the assigned sprint
    if (sprintTitles.contains(assignedSprint)) {
        SprintSelectionComboBox->setCurrentText(assignedSprint);
    } else {
        SprintSelectionComboBox->addItem(assignedSprint);
        SprintSelectionComboBox->setCurrentText(assignedSprint);
    }
    userStoriesTable->setCellWidget(rowCount, 7, SprintSelectionComboBox);

    connect(SprintSelectionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, userStoriesTable, rowCount, taskID](int index) {
                QString title = userStoriesTable->item(rowCount, 2)->text();
                QString description = userStoriesTable->item(rowCount, 3)->text();

                // Retrieve the status from the ComboBox in column 4
                QString status;
                QWidget* statusWidget = userStoriesTable->cellWidget(rowCount, 4);
                if (statusWidget) {
                    QComboBox* statusComboBox = qobject_cast<QComboBox*>(statusWidget);
                    if (statusComboBox) {
                        status = statusComboBox->currentText();
                    }
                }

                // Retrieve the assigneeId from the ComboBox in column 5
                QString assigneeId;
                QWidget* assigneeWidget = userStoriesTable->cellWidget(rowCount, 5);
                if (assigneeWidget) {
                    QComboBox* assigneeComboBox = qobject_cast<QComboBox*>(assigneeWidget);
                    if (assigneeComboBox) {
                        assigneeId = assigneeComboBox->currentText();
                    }
                }

                int priority = userStoriesTable->item(rowCount, 6)->text().toInt();
                QString selectedSprint = qobject_cast<QComboBox *>(userStoriesTable->cellWidget(rowCount, 7))->currentText();
                SendToSprint(taskID, title, description, status, priority, assigneeId, selectedSprint);
            });


}


void pb_productbacklog_implementation::SendToSprint(int taskID, const QString& title, const QString& description, const QString& status, int priority, const QString& assigneeId, const QString& selectedSprint){

    //First we need to delete exisiting row from TaskPB in the Table
    deleteTaskFromDatabase(taskID);


    //Then we Add a new Entry in TaskPB
    QTableWidget* table = parentBoard->getUserStoriesTableView(); // Assuming there's a method to get the task table view
    int PassedProjectID = parentBoard->getProjectId();
    QComboBox* SprintComboBox = parentBoard->get_BL_SprintDropDown();
    QString selectedValue = SprintComboBox->currentText().trimmed(); // Trim to remove any whitespace
    qDebug() << "Project ID in TASKPB - TASKSB: " << PassedProjectID;
    //Getting the assignee value from the database
    if (!table) {
        qDebug() << "Task table view not found or accessible.";
        return;
    }
    DatabaseManager database1;
    QSqlDatabase dbobj12 = database1.getDatabase();
    if (!dbobj12.isOpen()) {
        qDebug() << "Connection Not Established - pb_productbacklog_implmentation!";
        return;
    }
    //-----------------Taking the Correct ProductBacklog ID--------------------
    QSqlQuery querySelect(dbobj12);
    querySelect.prepare("SELECT PB.idProductBacklog "
                        "FROM Project AS P "
                        "INNER JOIN ProductBacklog AS PB ON P.idProject = PB.Project_idProject "
                        "WHERE P.idProject = :projectId");
    querySelect.bindValue(":projectId", PassedProjectID);

    int productBacklogId = 0;
    if (querySelect.exec()) {
        if (querySelect.next()) {
            productBacklogId = querySelect.value(0).toInt();
        }
    } else {
        qDebug() << "Failed to retrieve ProductBacklog ID:" << querySelect.lastError().text();
        return;
    }
    QSqlDatabase TaskPB = database.getDatabase();
    QSqlQuery query(TaskPB);
    query.prepare("INSERT INTO scrummy.TaskPB(Title, Description, Status, Priority, Assignee, ProductBacklog_idProductBacklog, ProductBacklog_Project_idProject, AssignedSprint) "
                  "VALUES (:title, :description, :status, :priority, :assignee, :productBacklogId, :projectId, :assignedSprint)");
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":status", status);
    query.bindValue(":priority", priority);
    query.bindValue(":assignee", assigneeId);
    query.bindValue(":productBacklogId", productBacklogId); // Assuming this value is correctly provided
    query.bindValue(":projectId", PassedProjectID);
    query.bindValue(":assignedSprint", selectedSprint); // Corrected the double colon and misspelling

    if (!query.exec()) {
        qDebug() << "Failed to insert data into TaskPB table:" << query.lastError().text();
        dbobj12.close();
        return;
    }
    qDebug() << "Data inserted into TaskPB table successfully!";
    // Retrieve the auto-incremented idTaskPB from the TaskPB table
    QVariant lastInsertId = query.lastInsertId();
    if (!lastInsertId.isValid()) {
        qDebug() << "Failed to retrieve the last inserted TaskPB ID.";
        dbobj12.close();
        return;
    }
    int SameValueTaskPB = lastInsertId.toInt(); // This will be used for TaskSB

    /*
    //Then we update the Assigned Sprint Column
    //Udpate in the TaskPB-Column "AssignedSprint"
    //Udpate in the TaskPB-Column "AssignedSprint"
    //Udpate in the TaskPB-Column "AssignedSprint"
    DatabaseManager database;
    QSqlDatabase dbobj1 = database.getDatabase();
    QSqlQuery UpdateTaskPB(dbobj1);
    UpdateTaskPB.prepare("UPDATE TaskPB "
                         "SET AssignedSprint = :selectedSprint "
                         "WHERE idTaskPB = :taskID" );
    UpdateTaskPB.bindValue(":selectedSprint", selectedSprint);
    UpdateTaskPB.bindValue(":taskID", SameValueTaskPB);   //Take the new entry
    if (!UpdateTaskPB.exec()) {
        qDebug() << "Update failed:" << UpdateTaskPB.lastError().text();
    } else {
        // If the update affects no rows, it may be because the taskID does not exist.
        if (UpdateTaskPB.numRowsAffected() > 0) {
            qDebug() << "TaskPB updated successfully for taskID" << taskID;
        } else {
            qDebug() << "No rows affected. Check if the taskID exists.";
        }
    }
    //Udpate in the TaskPB-Column "AssignedSprint"
    //Udpate in the TaskPB-Column "AssignedSprint"
    //Udpate in the TaskPB-Column "AssignedSprint"
    */




    //Add Task To a Sprint"
    //Add Task To a Sprint"
    //Add Task To a Sprint"

    QSqlDatabase dbobj = database.getDatabase();
    QString idSprintBacklog;
    QString Sprint_idSprint;
    QString selectedValue1 = selectedSprint;
    qDebug() << "Selected Value from ComboBox: " << selectedValue;


    if (selectedSprint == "Unassigned") {
        qDebug() << "Nothing Happend: " << selectedValue;
    }else{
    deleteTaskFromDatabase(taskID);



    //Take the Correct ID of the Username
    QSqlQuery queryUserId(dbobj);
    queryUserId.prepare("SELECT U.idUser FROM User AS U WHERE U.Username = :username");
    queryUserId.bindValue(":username", assigneeId);

    int userId = -1; // Default value, in case the query fails
    if(queryUserId.exec()) {
        if(queryUserId.next()) {
            userId = queryUserId.value(0).toInt();
            qDebug() << "Fetched User ID: " << userId;
        } else {
            qDebug() << "No user found for username: " << assigneeId;
        }
    } else {
        qDebug() << "Query error while fetching user ID:" << queryUserId.lastError().text();
    }


    QSqlQuery querySprintValues(dbobj);
    querySprintValues.prepare("SELECT SB.idSprintBacklog, SB.Sprint_idSprint "
                              "FROM SprintBacklog SB "
                              "INNER JOIN Sprint S ON S.idSprint = SB.Sprint_idSprint "
                              "WHERE S.Title = :sprintTitle");
    querySprintValues.bindValue(":sprintTitle", selectedValue1); // Binding the selected value

    if(querySprintValues.exec()) {
        if(querySprintValues.next()) {
            idSprintBacklog = querySprintValues.value(0).toString();
            Sprint_idSprint = querySprintValues.value(1).toString();
            qDebug() << "Fetched Values: " << idSprintBacklog << ", " << Sprint_idSprint;
        } else {
            qDebug() << "No data fetched. Check if the query returns any rows.";
        }
    } else {
        qDebug() << "Query error:" << querySprintValues.lastError().text();
    }
    qDebug() << "Executed query:" << querySprintValues.executedQuery();
    //COPY IN THE TASKSB TABLE
    QSqlQuery query1(dbobj);
    query1.prepare("INSERT INTO scrummy.TaskSB(idTask, Title, Description, Status, Priority, Assignee, SprintBacklog_idSprintBacklog, SprintBacklog_Sprint_idSprint, SprintBacklog_Sprint_Project_idProject) "
                   "VALUES (:idTask, :title, :description, :status, :priority, :assignee, :sprintBacklogId, :sprintId, :projectId)");
    query1.bindValue(":idTask", SameValueTaskPB);
    query1.bindValue(":title", title);
    query1.bindValue(":description", description);
    query1.bindValue(":status", status);
    query1.bindValue(":priority", priority);
    query1.bindValue(":assignee", userId);
    query1.bindValue(":sprintBacklogId", idSprintBacklog);
    query1.bindValue(":sprintId", Sprint_idSprint);
    query1.bindValue(":projectId", PassedProjectID);

    if (!query1.exec()) {
        qDebug() << "Failed to insert data into TaskSB table:" << query1.lastError().text();
        dbobj.close();
        return;
    }
    qDebug() << "Data inserted into TaskSB table successfully!";
    dbobj.close();
    }
}

void pb_productbacklog_implementation::onTaskAssigneeChanged(int taskID, const QString& newAssignee) {
    // Update the database with the new assignee for the given taskID

    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    if (!userStoriesTable) return;

    // Find the row for the taskID. This assumes taskID is unique.
    int row = -1;
    for (int i = 0; i < userStoriesTable->rowCount(); ++i) {
        if (userStoriesTable->item(i, 0)->data(Qt::UserRole).toInt() == taskID) {
            row = i;
            break;
        }
    }
    if (row == -1) return; // Task ID not found

    QString title = userStoriesTable->item(row, 2)->text();
    QString description = userStoriesTable->item(row, 3)->text();

    // Retrieve the status from the ComboBox in another column (update the column index as needed)
    QString status;
    QWidget* statusWidget = userStoriesTable->cellWidget(row, 4); // Replace [statusColumnIndex] with the actual column index
    if (statusWidget) {
        QComboBox* statusComboBox = qobject_cast<QComboBox*>(statusWidget);
        if (statusComboBox) {
            status = statusComboBox->currentText();
        }
    }

    int priority = userStoriesTable->item(row, 6)->text().toInt();

    DatabaseManager database1;
    QSqlDatabase onTaskAssigneeChangedConnection = database1.getDatabase();
    QSqlQuery query(onTaskAssigneeChangedConnection);
    query.prepare("UPDATE scrummy.TaskPB SET Assignee = ? WHERE idTaskPB = ?");
    query.addBindValue(newAssignee);
    query.addBindValue(taskID);

    if (!query.exec()) {
        qDebug() << "Update failed for Task ID:" << taskID << " Error:" << query.lastError();
    } else {
        qDebug() << "Assignee updated successfully for task ID:" << taskID;
    }
    if (taskMap.contains(taskID)) {
        updateTaskInDatabase(taskID, title, description, status, newAssignee, priority);
    } else {
        qDebug() << "TaskID in onTaskAssigneeChanged not found";
    }
}



void pb_productbacklog_implementation::onStatusChanged(int taskID, const QString& status) {
    qDebug() << "onStatusChanged called with taskID:" << taskID << " status:" << status;
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    if (!userStoriesTable) return;

    // Find the row for the taskID. This assumes taskID is unique.
    int row = -1;
    for (int i = 0; i < userStoriesTable->rowCount(); ++i) {
        if (userStoriesTable->item(i, 0)->data(Qt::UserRole).toInt() == taskID) {
            row = i;
            break;
        }
    }
    if (row == -1) return; // Task ID not found

    QString title = userStoriesTable->item(row, 2)->text();
    QString description = userStoriesTable->item(row, 3)->text();

    // Retrieve the assignee from the ComboBox in column 5
    QString assignee;
    QWidget* assigneeWidget = userStoriesTable->cellWidget(row, 5);
    if (assigneeWidget) {
        QComboBox* assigneeComboBox = qobject_cast<QComboBox*>(assigneeWidget);
        if (assigneeComboBox) {
            assignee = assigneeComboBox->currentText();
        }
    }

    int priority = userStoriesTable->item(row, 6)->text().toInt();

    if (taskMap.contains(taskID)) {
        updateTaskInDatabase(taskID, title, description, status, assignee, priority);
    }else{
        qDebug() << "TaskID in onStatusChagned not found";

    }
}




//EDIT FUNCTIONALITY
void pb_productbacklog_implementation::onTableItemChanged(QTableWidgetItem* item) {
    if (!item) {
        qDebug() << "Item is null";
        return;
    }

    int row = item->row();
    int column = item->column();
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();

    if (!userStoriesTable) {
        qDebug() << "User stories table not found";
        return;
    }

    // Debugging information
    qDebug() << "Changed item at row:" << row << "column:" << column << "Value:" << item->text();

    int taskID = userStoriesTable->item(row, 0)->data(Qt::UserRole).toInt();
    qDebug() << "Task ID to update:" << taskID;

    if (!taskMap.contains(taskID)) {
        qDebug() << "Task ID not found in taskMap:" << taskID;
        return;
    }

    QString title = userStoriesTable->item(row, 2)->text();
    QString description = userStoriesTable->item(row, 3)->text();
    QString status;
    QString assignee;
    int priority = userStoriesTable->item(row, 6)->text().toInt();

    // Retrieve status and assignee from their ComboBoxes
    QWidget* statusWidget = userStoriesTable->cellWidget(row, 4);
    QWidget* assigneeWidget = userStoriesTable->cellWidget(row, 5);

    if (statusWidget) {
        QComboBox* statusComboBox = qobject_cast<QComboBox*>(statusWidget);
        if (statusComboBox) {
            status = statusComboBox->currentText();
        }
    }

    if (assigneeWidget) {
        QComboBox* assigneeComboBox = qobject_cast<QComboBox*>(assigneeWidget);
        if (assigneeComboBox) {
            assignee = assigneeComboBox->currentText();
        }
    }

    // Update the task in the database
    updateTaskInDatabase(taskID, title, description, status, assignee, priority);
}




void pb_productbacklog_implementation::updateTaskInDatabase(int taskID, const QString& title, const QString& description, const QString& status, QString assignee, int priority) {

    qDebug() << "updateTaskInDatabase Function has been called";


    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database is not open";
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE scrummy.TaskPB SET Title = ?, Description = ?, Status = ?, Assignee = ?, Priority = ? WHERE idTaskPB = ?");
    query.addBindValue(title);
    query.addBindValue(description);
    query.addBindValue(status);
    query.addBindValue(assignee);
    query.addBindValue(priority);
    query.addBindValue(taskID);

    if (!query.exec()) {
        qDebug() << "Update failed: " << query.lastError();
    } else {
        qDebug() << "Update successful for task ID:" << taskID;
    }

    //Taking the correct ID of the UserName
    QSqlQuery query3;
    query3.prepare("SELECT U.idUser FROM User AS U WHERE U.Username = :username");
    query3.bindValue(":username", assignee);

    int userId = -1; // Default value, in case the query fails
    if(query3.exec()) {
        if(query3.next()) {
            userId = query3.value(0).toInt();
            qDebug() << "Fetched User ID: " << userId;
        } else {
            qDebug() << "No user found for username: " << assignee;
        }
    } else {
        qDebug() << "Query error while fetching user ID:" << query3.lastError().text();
    }

    qDebug() << "Status:" << status;
    qDebug() << "Assignee:" << userId;

    //Update TaskSB at the Same time
    QSqlQuery query2;
    query2.prepare("UPDATE scrummy.TaskSB SET Title = ?, Description = ?, Status = ?, Assignee = ?, Priority = ? WHERE idTask = ?");
    query2.addBindValue(title);
    query2.addBindValue(description);
    query2.addBindValue(status);
    query2.addBindValue(userId);
    query2.addBindValue(priority);
    query2.addBindValue(taskID);

    if (!query2.exec()) {
        qDebug() << "Update failed: " << query2.lastError();
    } else {
        qDebug() << "Update successful for task ID:" << taskID;
    }
}


// Slot to handle row deletion when a row is clicked
void pb_productbacklog_implementation::onRowClicked(QTableWidgetItem* item) {
    if (!item) {
        qDebug() << "Item is null";
        return;
    }

    int row = item->row();
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();

    if (!userStoriesTable) {
        qDebug() << "User stories table not found";
        return;
    }

    // Retrieve the task ID associated with the clicked row
    int taskID = userStoriesTable->item(row, 0)->data(Qt::UserRole).toInt();

    // Remove the row from the table
    userStoriesTable->removeRow(row);

    // Call a function to delete the task from the database
    deleteTaskFromDatabase(taskID);
}

// Function to delete a task from the database
void pb_productbacklog_implementation::deleteTaskFromDatabase(int taskID) {
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database is not open";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM scrummy.TaskPB WHERE idTaskPB = ?");
    query.addBindValue(taskID);

    if (!query.exec()) {
        qDebug() << "Delete failed: " << query.lastError();
    } else {
        qDebug() << "Delete successful for task ID:" << taskID;
    }

    //Also Gets Deleted in
    QSqlQuery query1;
    query.prepare("DELETE FROM scrummy.TaskSB WHERE idTask = ?");
    query.addBindValue(taskID);

    if (!query.exec()) {
        qDebug() << "Delete failed: " << query.lastError();
    } else {
        qDebug() << "Delete successful for task ID:" << taskID;
    }





}




//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Creation of Tasks  - - - - - - - - - - - - - - - - - - - - NEW WINDOW ON THE RIGHT
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------


void pb_productbacklog_implementation::onButtonIssueClicked() {
    //QComboBox* inputAssignee = parentBoard->getInputAssignee();
    QTextEdit* inputDescription = parentBoard->getInputDescription();
    QTextEdit* inputPriority = parentBoard->getInputPriority();
    QTextEdit* inputStatus = parentBoard->getInputStatus();
    QTextEdit* inputTitle = parentBoard->getInputTitle();
    QString title = inputTitle->toPlainText();
    QString description = inputDescription->toPlainText();
    QString priorityText = inputPriority->toPlainText();
    int priority = priorityText.toInt();
    QString status = inputStatus->toPlainText();
    QComboBox* SprintDropDown = parentBoard->get_BL_SprintDropDown();
    QComboBox* InputAssignee = parentBoard->getInputAssignee();
    // To get the currently selected text from the combo boxes
    QString Pass_sprint = SprintDropDown->currentText();
    QString assignee = InputAssignee->currentText();
    if (title.isEmpty() || description.isEmpty() || priorityText.isEmpty() || status.isEmpty()) {
        // One or more fields are empty
        QMessageBox::warning(nullptr, "Missing Values", "Please fill in all fields.");
    } else {
        addTaskToBacklog(title, description, status, priority, assignee, Pass_sprint);
    }
}





void pb_productbacklog_implementation::addTaskToBacklog(const QString& title, const QString& description, const QString& status, int priority, QString assignee, QString SelectedSprint) {
    /*
     *
     * // Adding the task in the TaskPB
     *
    */

    QTableWidget* table = parentBoard->getUserStoriesTableView(); // Assuming there's a method to get the task table view
    int PassedProjectID = parentBoard->getProjectId();
    QComboBox* SprintComboBox = parentBoard->get_BL_SprintDropDown();
    QString selectedValue = SprintComboBox->currentText().trimmed(); // Trim to remove any whitespace
    qDebug() << "Project ID in TASKPB - TASKSB: " << PassedProjectID;
    //Getting the assignee value from the database
    if (!table) {
        qDebug() << "Task table view not found or accessible.";
        return;
    }
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"ID", "Title", "Description", "Status", "Assignee", "Priority", "ProductBacklog ID", "Sprint"});
    QHeaderView* header = table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();
    if (!dbobj.isOpen()) {
        qDebug() << "Connection Not Established - pb_productbacklog_implmentation!";
        return;
    }
    //-----------------Taking the Correct ProductBacklog ID--------------------
    QSqlQuery querySelect(dbobj);
    querySelect.prepare("SELECT PB.idProductBacklog "
                        "FROM Project AS P "
                        "INNER JOIN ProductBacklog AS PB ON P.idProject = PB.Project_idProject "
                        "WHERE P.idProject = :projectId");
    querySelect.bindValue(":projectId", PassedProjectID);

    int productBacklogId = 0;
    if (querySelect.exec()) {
        if (querySelect.next()) {
            productBacklogId = querySelect.value(0).toInt();
        }
    } else {
        qDebug() << "Failed to retrieve ProductBacklog ID:" << querySelect.lastError().text();
        return;
    }
    QSqlQuery query(dbobj);
    query.prepare("INSERT INTO scrummy.TaskPB(Title, Description, Status, Priority, Assignee, ProductBacklog_idProductBacklog, ProductBacklog_Project_idProject, AssignedSprint) "
                  "VALUES (:title, :description, :status, :priority, :assignee, :productBacklogId, :projectId, :assignedSprint)");
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":status", status);
    query.bindValue(":priority", priority);
    query.bindValue(":assignee", assignee);
    query.bindValue(":productBacklogId", productBacklogId); // Assuming this value is correctly provided
    query.bindValue(":projectId", PassedProjectID);
    query.bindValue(":assignedSprint", selectedValue); // Corrected the double colon and misspelling

    if (!query.exec()) {
        qDebug() << "Failed to insert data into TaskPB table:" << query.lastError().text();
        dbobj.close();
        return;
    }
    //We need to get the Same Value of the Primary key of this table to store in TaskSB, where we are now able to make a relation mannually
    QVariant lastInsertId = query.lastInsertId();
    if (!lastInsertId.isValid()) {
        qDebug() << "Failed to retrieve the last inserted TaskPB ID.";
        dbobj.close();
        return;
    }
    int SameValueTaskPB = lastInsertId.toInt(); // This will be used for TaskSB

    qDebug() << "Data inserted into TaskPB table successfully!";








    /*
     *
     * // Condition to either add in TaskSB or skip it)
     *
    */







    if (SelectedSprint == "Unassigned" ) {
        goto ONLYTASKPB_END;
    } else {
    //-----------------START-----------------------------Copy Of the Task in the Sprint Table
    QSqlQuery queryUser(dbobj);
    queryUser.prepare("SELECT idUser FROM User WHERE Username = :username");
    queryUser.bindValue(":username", assignee);

    int assigneeId = 0;
    if (queryUser.exec()) {
        if (queryUser.next()) {
            assigneeId = queryUser.value(0).toInt(); // Assuming the first column is the user ID
        }
    } else {
        qDebug() << "Failed to retrieve user ID:" << queryUser.lastError().text();
        return;
    }
    //TAKE THE CORRECT VALUES FROM THE SPRINT BACKLOG TABLE
    /*
    */
    QString idSprintBacklog;
    QString Sprint_idSprint;
    QComboBox* SprintComboBox = parentBoard->get_BL_SprintDropDown();
    QString selectedValue = SprintComboBox->currentText().trimmed(); // Trim to remove any whitespace
    qDebug() << "Selected Value from ComboBox: " << selectedValue;

    QSqlQuery querySprintValues(dbobj);
    querySprintValues.prepare("SELECT SB.idSprintBacklog, SB.Sprint_idSprint "
                              "FROM SprintBacklog SB "
                              "INNER JOIN Sprint S ON S.idSprint = SB.Sprint_idSprint "
                              "WHERE S.Title = :sprintTitle");
    querySprintValues.bindValue(":sprintTitle", selectedValue); // Binding the selected value

    if(querySprintValues.exec()) {
        if(querySprintValues.next()) {
            idSprintBacklog = querySprintValues.value(0).toString();
            Sprint_idSprint = querySprintValues.value(1).toString();
            qDebug() << "Fetched Values: " << idSprintBacklog << ", " << Sprint_idSprint;
        } else {
            qDebug() << "No data fetched. Check if the query returns any rows.";
        }
    } else {
        qDebug() << "Query error:" << querySprintValues.lastError().text();
    }

    qDebug() << "Executed query:" << querySprintValues.executedQuery();
    //COPY IN THE TASKSB TABLE
    QSqlQuery query1(dbobj);
    query1.prepare("INSERT INTO scrummy.TaskSB(idTask, Title, Description, Status, Priority, Assignee, SprintBacklog_idSprintBacklog, SprintBacklog_Sprint_idSprint, SprintBacklog_Sprint_Project_idProject) "
                   "VALUES (:idTask, :title, :description, :status, :priority, :assignee, :sprintBacklogId, :sprintId, :projectId)");
    query1.bindValue(":idTask", SameValueTaskPB);
    query1.bindValue(":title", title);
    query1.bindValue(":description", description);
    query1.bindValue(":status", status);
    query1.bindValue(":priority", priority);
    query1.bindValue(":assignee", assigneeId); // Assuming 'assignee' is a variable holding the assignee's ID
    query1.bindValue(":sprintBacklogId", idSprintBacklog); // Assuming this is a constant or should be obtained dynamically
    query1.bindValue(":sprintId", Sprint_idSprint); // Assuming SelectedSprint is a variable holding the sprint ID
    query1.bindValue(":projectId", PassedProjectID); // Assuming PassedProjectID is a variable holding the project ID

    if (!query1.exec()) {
        qDebug() << "Failed to insert data into TaskSB table:" << query1.lastError().text();
        dbobj.close();
        return;
    }
    qDebug() << "Data inserted into TaskSB table successfully!";
    dbobj.close();
    }
    //-----------------END-----------------------------Copy Of the Task in the Sprint Table



ONLYTASKPB_END:
    int rowCount = table->rowCount();
    table->insertRow(rowCount);

    QTableWidgetItem* itemID = new QTableWidgetItem(query.lastInsertId().toString()); // Assuming auto-increment ID
    QTableWidgetItem* itemTitle = new QTableWidgetItem(title);
    QTableWidgetItem* itemDescription = new QTableWidgetItem(description);
    QTableWidgetItem* itemStatus = new QTableWidgetItem(status);
    QTableWidgetItem* itemPriority = new QTableWidgetItem(QString::number(priority));
    QTableWidgetItem* itemAssignee = new QTableWidgetItem(assignee);

    itemID->setFlags(itemID->flags() & ~Qt::ItemIsEditable);

    table->setItem(rowCount, 0, itemID);
    table->setItem(rowCount, 1, itemTitle);
    table->setItem(rowCount, 2, itemDescription);
    table->setItem(rowCount, 3, itemStatus);
    table->setItem(rowCount, 4, itemPriority);
    table->setItem(rowCount, 5, itemAssignee);

    RetrieveAndDisplayBacklog();
}




//------------------------------------------------------------------------------------------------------------------------------
//FULL IMPLEMENTATION OF CREATION OF TASK AND EDIT FUCNTIONALITY    -----    END
//------------------------------------------------------------------------------------------------------------------------------












//------------------------------------------------------------------------------------------------------------------------------












//------------------------------------------------------------------------------------------------------------------------------
//FULL IMPLEMENTATION OF CREATION OF USER-STORIES AND EDIT FUCNTIONALITY    -----    START
//------------------------------------------------------------------------------------------------------------------------------



void pb_productbacklog_implementation::UserStoryPBretrieval() {
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    int PassedProjectID = parentBoard->getProjectId();

    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);
        query.prepare("SELECT idUserStoryPB, Title, Description, Status, Priority, Assignee FROM scrummy.UserStoryPB WHERE ProductBacklog_Project_idProject = :projectID");
        query.bindValue(":projectID", PassedProjectID);

        if (query.exec()) {
            qDebug() << "User Stories Retrieved Successfully!";
            while (query.next()) {
                int storyID = query.value(0).toInt();
                QString title = query.value(1).toString();
                QString description = query.value(2).toString();
                QString status = query.value(3).toString();
                int priority = query.value(4).toInt();
                int assignee = query.value(5).toInt();
                /*
                qDebug() << "Retrieved User Story ID:" << storyID
                         << " Title:" << title
                         << " Description:" << description
                         << " Status:" << status
                         << " Priority:" << priority
                         << " Assignee:" << assignee;
                */
                UserStories_Added_In_Table("User Story", title, description, status, assignee, priority, storyID);
                storyMap[storyID] = {title, description, status, priority, assignee, storyID};
            }
        } else {
            qDebug() << "Failed to retrieve data: " << query.lastError().text();
        }
        dbobj.close();
    } else {
        qDebug() << "Connection Not Established - pb_productbacklog_implementation class! - User Story";
    }
}

void pb_productbacklog_implementation::UserStories_Added_In_Table(const QString& type_pb, const QString& storyName, const QString& description, const QString& status, int assignee, int priority, int storyID) {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    userStoriesTable->setColumnCount(8);
    userStoriesTable->setHorizontalHeaderLabels({"ID", "Type", "Title", "Description", "Status", "Assignee", "Priority", "Sprint"});
    userStoriesTable->setColumnHidden(0, true);
    //userStoriesTable->setColumnHidden(6, true);
    QHeaderView* header = userStoriesTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    if (userStoriesTable) {
        int rowCount = userStoriesTable->rowCount();
        userStoriesTable->insertRow(rowCount);

        QTableWidgetItem* itemStoryID = new QTableWidgetItem(QString::number(storyID));
        itemStoryID->setData(Qt::UserRole, QVariant(storyID));
        userStoriesTable->setItem(rowCount, 0, itemStoryID);

        QTableWidgetItem* type = new QTableWidgetItem(type_pb);
        QTableWidgetItem* itemStoryName = new QTableWidgetItem(storyName);
        QTableWidgetItem* itemDescription = new QTableWidgetItem(description);
        //QTableWidgetItem* itemStatus = new QTableWidgetItem(status);
        QTableWidgetItem* itemPriority = new QTableWidgetItem(QString::number(priority));
        QTableWidgetItem* itemAssignee = new QTableWidgetItem(QString::number(assignee));

        type->setFlags(type->flags() & ~Qt::ItemIsEditable);
        itemStoryID->setFlags(itemStoryID->flags() & ~Qt::ItemIsEditable);

        userStoriesTable->setItem(rowCount, 1, type);
        userStoriesTable->setItem(rowCount, 2, itemStoryName);
        userStoriesTable->setItem(rowCount, 3, itemDescription);


        QComboBox* statusComboBox = new QComboBox();
        statusComboBox->addItems({"ToDo", "InProgress", "Blocked", "Done"});
        statusComboBox->setCurrentText(status);
        userStoriesTable->setCellWidget(rowCount, 4, statusComboBox);

        // Correct the connection to use the overload of currentIndexChanged that passes the current text
        connect(statusComboBox, &QComboBox::currentTextChanged,
                [this, storyID, statusComboBox](const QString &newStatus) {
                    // Since storyID is captured by value, it is the correct ID for the row
                    onUserStoryStatusChanged(storyID, newStatus);
                });

        userStoriesTable->setItem(rowCount, 5, itemPriority);
        userStoriesTable->setItem(rowCount, 6, itemAssignee);
        /*
        qDebug() << "Adding User Story to Table: ID:" << storyID
                 << " Title:" << storyName
                 << " Description:" << description
                 << " Status:" << status
                 << " Priority:" << priority
                 << " Assignee:" << assignee;
        */
    } else {
        qDebug() << "Table view not found or accessible.";
    }
}


void pb_productbacklog_implementation::onUserStoryStatusChanged(int storyID, const QString& newStatus) {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    if (!userStoriesTable) return;

    // Find the row for the storyID. This assumes storyID is unique.
    int row = -1;
    for (int i = 0; i < userStoriesTable->rowCount(); ++i) {
        if (userStoriesTable->item(i, 0)->data(Qt::UserRole).toInt() == storyID) {
            row = i;
            break;
        }
    }
    if (row == -1) return; // storyID not found

    // Retrieve the rest of the details from the row
    QString title = userStoriesTable->item(row, 2)->text();
    QString description = userStoriesTable->item(row, 3)->text();
    int priority = userStoriesTable->item(row, 5)->text().toInt();
    int assignee = userStoriesTable->item(row, 6)->text().toInt();

    updateUserStoryInDatabase(storyID, title, description, newStatus, priority, assignee);
}


void pb_productbacklog_implementation::updateUserStoryInDatabase(int storyID, const QString& title, const QString& description, const QString& status, int priority, int assignee) {
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database is not open";
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE scrummy.UserStoryPB SET Title = ?, Description = ?, Status = ?, Priority = ?, Assignee = ? WHERE idUserStoryPB = ?");
    query.addBindValue(title);
    query.addBindValue(description);
    query.addBindValue(status);
    query.addBindValue(priority);
    query.addBindValue(assignee);
    query.addBindValue(storyID);

    if (!query.exec()) {
        qDebug() << "Update failed for User Story ID:" << storyID << " Error:" << query.lastError();
    } else {
        qDebug() << "Update successful for user story ID:" << storyID;
    }
}

void pb_productbacklog_implementation::onUserStoryTableItemChanged(QTableWidgetItem* item) {
    if (!item) {
        qDebug() << "Item is null";
        return;
    }

    int row = item->row();
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();

    if (!userStoriesTable) {
        qDebug() << "User stories table not found";
        return;
    }

    int storyID = userStoriesTable->item(row, 0)->data(Qt::UserRole).toInt();

    qDebug() << "Attempting to update User Story ID:" << storyID;

    if (storyMap.contains(storyID)) {
        QString title = userStoriesTable->item(row, 2)->text();
        QString description = userStoriesTable->item(row, 3)->text();
        // ...
        QString status;
        QWidget* widget = userStoriesTable->cellWidget(row, 4);
        if (widget) {
            QComboBox* comboBox = qobject_cast<QComboBox*>(widget);
            if (comboBox) {
                status = comboBox->currentText();
            }
        }
        int priority = userStoriesTable->item(row, 5)->text().toInt();
        int assignee = userStoriesTable->item(row, 6)->text().toInt();
        qDebug() << "Updating User Story: ID:" << storyID
                 << " Title:" << title
                 << " Description:" << description
                 << " Status:" << status
                 << " Priority:" << priority
                 << " Assignee:" << assignee;
        updateUserStoryInDatabase(storyID, title, description, status, priority, assignee);
    } else {
        qDebug() << "User story ID not found in storyMap. ID:" << storyID;
    }
}





//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Creation of UserStories
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------


void pb_productbacklog_implementation::on_createUserStory_clicked() {
    qDebug() << "Create User Story button clicked.";


    //QTextEdit* inputAssignee = parentBoard->getInputAssignee();
    QTextEdit* inputDescription = parentBoard->getInputDescription();
    QTextEdit* inputPriority = parentBoard->getInputPriority();
    QTextEdit* inputStatus = parentBoard->getInputStatus();
    QTextEdit* inputTitle = parentBoard->getInputTitle();

    QString title = inputTitle->toPlainText();
    // QString assignee = inputAssignee->toPlainText();
    QString description = inputDescription->toPlainText();
    QString priorityText = inputPriority->toPlainText();
    int priority = priorityText.toInt();
    QString status = inputStatus->toPlainText();

    if (title.isEmpty() || description.isEmpty() || priorityText.isEmpty() || status.isEmpty()) {
        // One or more fields are empty
        QMessageBox::warning(nullptr, "Missing Values", "Please fill in all fields.");
    } else {
        addUserStoryToBacklog(title, description, status, priority, "assignee");
    }

    /*
    QString title = QInputDialog::getText(nullptr, "Enter Title", "Title:");
    QString description = QInputDialog::getText(nullptr, "Enter Description", "Description:");
    QStringList statuses = {"To Do", "In Progress", "Done"};
    QString status = QInputDialog::getItem(nullptr, "Select Status", "Status:", statuses, 0, false);
    int priority = QInputDialog::getInt(nullptr, "Enter Priority", "Priority:", 1, 1, 5, 1);
    QString assignee = QInputDialog::getText(nullptr, "Enter Assignee", "Assignee:");
    */
}

void pb_productbacklog_implementation::addUserStoryToBacklog(const QString& title, const QString& description, const QString& status, int priority, const QString& assignee) {
    QTableWidget* table = parentBoard->getUserStoriesTableView();
    if (!table) {
        qDebug() << "Table view not found or accessible.";
        return;
    }
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"ID", "Type", "Title", "Description", "Status", "Priority", "Assignee", "Sprint"});

    QHeaderView* header = table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();
    if (!dbobj.isOpen()) {
        qDebug() << "Connection Not Established - pb_productbacklog_implmentation!";
        return;
    }

    QSqlQuery query(dbobj);
    query.prepare("INSERT INTO scrummy.UserStoryPB(Title, Description, Status, Priority, Assignee, ProductBacklog_idProductBacklog, ProductBacklog_Project_idProject)"
                  "VALUES (:title, :description, :status, :priority, :assignee, 1, 1)");
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":status", status);
    query.bindValue(":priority", priority);
    query.bindValue(":assignee", assignee);

    if (!query.exec()) {
        qDebug() << "Failed to insert data into UserStoryPB table:" << query.lastError().text();
        dbobj.close();
        return;
    }
    qDebug() << "Data inserted into UserStoryPB table successfully!";
    dbobj.close();

    int rowCount = table->rowCount();
    table->insertRow(rowCount);

    QTableWidgetItem* itemID = new QTableWidgetItem(query.lastInsertId().toString()); // Assuming auto-increment ID
    QTableWidgetItem* itemTitle = new QTableWidgetItem(title);
    QTableWidgetItem* itemDescription = new QTableWidgetItem(description);
    QTableWidgetItem* itemStatus = new QTableWidgetItem(status);
    QTableWidgetItem* itemPriority = new QTableWidgetItem(QString::number(priority));
    QTableWidgetItem* itemAssignee = new QTableWidgetItem(assignee);

    itemID->setFlags(itemID->flags() & ~Qt::ItemIsEditable);

    table->setItem(rowCount, 0, itemID);
    table->setItem(rowCount, 1, itemTitle);
    table->setItem(rowCount, 2, itemDescription);
    table->setItem(rowCount, 3, itemStatus);
    table->setItem(rowCount, 4, itemPriority);
    table->setItem(rowCount, 5, itemAssignee);

    RetrieveAndDisplayBacklog();
}
//------------------------------------------------------------------------------------------------------------------------------
//FULL IMPLEMENTATION OF CREATION OF USER-STORIES AND EDIT FUCNTIONALITY    -----    END
//------------------------------------------------------------------------------------------------------------------------------









//------------------------------------------------------------------------------------------------------------------------------
//Fetch Spirits and Username - According to the project you are logged in with
//------------------------------------------------------------------------------------------------------------------------------



void pb_productbacklog_implementation::BL_fetechSprints() {
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (!dbobj.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QComboBox* SprintComboBox = parentBoard->get_BL_SprintDropDown();
    QComboBox* InputAssignee = parentBoard->getInputAssignee();
    int PassedProjectID = parentBoard->getProjectId();
    qDebug() << "Project ID in Product Backlog: " << PassedProjectID;

    // Set font size for the combo boxes
    QFont comboBoxFont;
    comboBoxFont.setPointSize(12);
    SprintComboBox->setFont(comboBoxFont);
    InputAssignee->setFont(comboBoxFont);

    // Clear existing items in both combo boxes
    SprintComboBox->clear();
    InputAssignee->clear();
    SprintComboBox->addItem("Unassigned");
    InputAssignee->addItem("Unassigned");

    // Fetch Sprints
    QSqlQuery query(dbobj);
    query.prepare("SELECT Title FROM Sprint WHERE Project_idProject = :projectID");
    query.bindValue(":projectID", PassedProjectID);

    if (!query.exec()) {
        qDebug() << "Sprint query failed: " << query.lastError();
        return;
    }

    if (query.size() == 0) {
        qDebug() << "No sprints found for Project ID: " << PassedProjectID;
        SprintComboBox->addItem("No Sprints Available");
    } else {
        while (query.next()) {
            QString title = query.value(0).toString();
            SprintComboBox->addItem(title);
        }
    }

    // Fetch Assignees
    QSqlQuery query1(dbobj);
    query1.prepare("SELECT User.Username "
                   "FROM Project "
                   "INNER JOIN Project_has_User ON Project.idProject = Project_has_User.Project_idProject "
                   "INNER JOIN User ON Project_has_User.User_idUser = User.idUser "
                   "WHERE Project_has_User.Project_idProject = :projectID");
    query1.bindValue(":projectID", PassedProjectID);

    if (!query1.exec()) {
        qDebug() << "Assignee query failed: " << query1.lastError();
        return;
    }

    if (query1.size() == 0) {
        qDebug() << "No assignees found for Project ID: " << PassedProjectID;
        InputAssignee->addItem("No Assignees Available");
    } else {
        while (query1.next()) {
            QString username = query1.value(0).toString();
            InputAssignee->addItem(username);
        }
    }
}




/**
 * @brief Handles the sorting of a QTableWidget based on the clicked header column.
 *
 * This function sorts the QTableWidget named userStoriesTable based on the clicked header column.
 * If the column clicked is column 6 (assumed to represent "Sprint"), the table is sorted in ascending order.
 * If the column clicked is column 5 (assumed to represent "Assignee"), a custom sort is applied where "Unassigned"
 * items are moved to the top of the sorted list. It temporarily alters the text of "Unassigned" items during sorting
 * by replacing it with a space (' '), ensuring it appears at the beginning, then restores the original text
 * after sorting.
 *
 * @param column The index of the column header that was clicked.
 */
void pb_productbacklog_implementation::onHeaderClicked(int column) {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();

    if (column == 6) { // Assuming column 6 is "Sprint"
        userStoriesTable->sortByColumn(column, Qt::AscendingOrder);
    } else if (column == 5) { // Assuming column 5 is "Assignee"
        // Custom sort: "Unassigned" comes first
        for (int i = 0; i < userStoriesTable->rowCount(); ++i) {
            QTableWidgetItem* item = userStoriesTable->item(i, column);
            if (item->text() == "Unassigned") {
                // Set the item's text to something that will come first in the sort
                item->setData(Qt::UserRole, item->text()); // Save the original text
                item->setText(" "); // A space will sort before any other text
            }
        }

        userStoriesTable->sortByColumn(column, Qt::AscendingOrder);

        // Restore the original text after sorting
        for (int i = 0; i < userStoriesTable->rowCount(); ++i) {
            QTableWidgetItem* item = userStoriesTable->item(i, column);
            if (item->data(Qt::UserRole).isValid()) {
                item->setText(item->data(Qt::UserRole).toString());
                item->setData(Qt::UserRole, QVariant()); // Clear the user role data
            }
        }
    }
}






















void pb_productbacklog_implementation::SendTasksToSprints() {
    qDebug() << "SendTasksToSprints called";


    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();


    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);
        query.prepare("SELECT Title FROM scrummy.Sprint");

        if (query.exec()) {
            qDebug() << "Sprint Data Retrieved Successfully!";

            while (query.next()) {
                QString sprintTitle = query.value(0).toString(); // Assuming Title is in the first column of the result
            }

        } else {
            qDebug() << "Failed to retrieve Sprint Data : " << query.lastError().text();
        }
        dbobj.close();
    } else {
        qDebug() << "Connection Not Established - pb_productbacklog_implementation class - ! - Sprint Data";
    }

}
