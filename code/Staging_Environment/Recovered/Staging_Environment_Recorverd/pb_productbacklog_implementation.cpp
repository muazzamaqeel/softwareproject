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

#include <QMap>
#include <QVariant>

pb_productbacklog_implementation::pb_productbacklog_implementation(parentboard* parentBoardInstance) {
    // Initialize any necessary variables or connections
    parentBoard = parentBoardInstance;

}
void pb_productbacklog_implementation::clearUserStoriesTable() {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    userStoriesTable->clearContents();
    userStoriesTable->setRowCount(0);

    QComboBox* SprintBox = parentBoard->getSprintComboBox();
    SprintBox->clear();
}
void pb_productbacklog_implementation::RetrieveAndDisplayBacklog() {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();

    if (userStoriesTable) {
        userStoriesTable->blockSignals(true); // Block signals

        clearUserStoriesTable(); // Clears the table before adding new entries
        TaskPBretrieval(); // Retrieves tasks and adds them to the table
        UserStoryPBretrieval();
       // SendTasksToSprints();
        userStoriesTable->blockSignals(false); // Unblock signals
    }
}




//------------------------------------------------------------------------------------------------------------------------------
//FULL IMPLEMENTATION OF CREATION OF TASK AND EDIT FUCNTIONALITY    -----    START
//------------------------------------------------------------------------------------------------------------------------------


void pb_productbacklog_implementation::TaskPBretrieval() {
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);
        query.prepare("SELECT idTaskPB, Title, Description, Status, Assignee, Priority, ProductBacklog_idProductBacklog, ProductBacklog_Project_idProject FROM scrummy.TaskPB");

        if (query.exec()) {
            qDebug() << "Tasks Retrieved Successfully!";
            while (query.next()) {
                int taskID = query.value(0).toInt();
                qDebug() << "Retrieved Task ID:" << taskID;
                QString title = query.value(1).toString();
                QString description = query.value(2).toString();
                QString status = query.value(3).toString();
                int assignee = query.value(4).toInt();
                int priority = query.value(5).toInt();

                // Include taskID as the seventh argument
                Tasks_Added_In_Table("Task", title, description, status, assignee, priority, taskID);

                // Store additional information in taskMap
                taskMap[taskID] = {title, description, status, assignee, priority, taskID};
            }

        } else {
            qDebug() << "Failed to retrieve data: " << query.lastError().text();
        }
        dbobj.close();
    } else {
        qDebug() << "Connection Not Established - pb_productbacklog_implementation class! - Task";
    }
}
void pb_productbacklog_implementation::Tasks_Added_In_Table(const QString& type_pb, const QString& taskName, const QString& description, const QString& status, int assignee, int priority, int taskID) {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    userStoriesTable->setColumnCount(7); // Include an additional column for Task ID
    userStoriesTable->setHorizontalHeaderLabels({"ID", "Type", "Title", "Description", "Status", "Assignee", "Priority"});

    QHeaderView* header = userStoriesTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    if (userStoriesTable) {
        int rowCount = userStoriesTable->rowCount();
        userStoriesTable->insertRow(rowCount);

        QTableWidgetItem* itemTaskID = new QTableWidgetItem();
        itemTaskID->setData(Qt::UserRole, QVariant(taskID));
        userStoriesTable->setItem(rowCount, 0, itemTaskID);

        QTableWidgetItem* type = new QTableWidgetItem(type_pb);
        QTableWidgetItem* itemTaskName = new QTableWidgetItem(taskName);
        QTableWidgetItem* itemDescription = new QTableWidgetItem(description);
        QTableWidgetItem* itemStatus = new QTableWidgetItem(status);
        QTableWidgetItem* itemAssignee = new QTableWidgetItem(QString::number(assignee));
        QTableWidgetItem* itemPriority = new QTableWidgetItem(QString::number(priority));

        type->setFlags(type->flags() & ~Qt::ItemIsEditable);
        itemTaskID->setFlags(itemTaskID->flags() & ~Qt::ItemIsEditable); // Make the Task ID column non-editable

        userStoriesTable->setItem(rowCount, 1, type);
        userStoriesTable->setItem(rowCount, 2, itemTaskName);
        userStoriesTable->setItem(rowCount, 3, itemDescription);
        userStoriesTable->setItem(rowCount, 4, itemStatus);
        userStoriesTable->setItem(rowCount, 5, itemAssignee);
        userStoriesTable->setItem(rowCount, 6, itemPriority);
    } else {
        qDebug() << "Table view not found or accessible.";
    }
}
void pb_productbacklog_implementation::onTableItemChanged(QTableWidgetItem* item) {
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

    // Retrieve the task ID from the first column
    int taskID = userStoriesTable->item(row, 0)->data(Qt::UserRole).toInt();
    qDebug() << "Task ID to update:" << taskID;

    if (taskMap.contains(taskID)) {
        // Retrieve the current values from all columns of the row
        QString title = userStoriesTable->item(row, 2)->text();
        QString description = userStoriesTable->item(row, 3)->text();
        QString status = userStoriesTable->item(row, 4)->text();
        int assignee = userStoriesTable->item(row, 5)->text().toInt();
        int priority = userStoriesTable->item(row, 6)->text().toInt();

        // Call the update function with all values
        updateTaskInDatabase(taskID, title, description, status, assignee, priority);
    } else {
        qDebug() << "Task ID not found in taskMap";
    }
}
void pb_productbacklog_implementation::updateTaskInDatabase(int taskID, const QString& title, const QString& description, const QString& status, int assignee, int priority) {
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

    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);
        query.prepare("SELECT idUserStoryPB, Title, Description, Status, Priority, Assignee FROM scrummy.UserStoryPB");

        if (query.exec()) {
            qDebug() << "User Stories Retrieved Successfully!";
            while (query.next()) {
                int storyID = query.value(0).toInt();
                QString title = query.value(1).toString();
                QString description = query.value(2).toString();
                QString status = query.value(3).toString();
                int priority = query.value(4).toInt();
                int assignee = query.value(5).toInt();

                qDebug() << "Retrieved User Story ID:" << storyID
                         << " Title:" << title
                         << " Description:" << description
                         << " Status:" << status
                         << " Priority:" << priority
                         << " Assignee:" << assignee;

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
    userStoriesTable->setColumnCount(7);
    userStoriesTable->setHorizontalHeaderLabels({"ID", "Type", "Title", "Description", "Status", "Priority", "Assignee"});

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
        QTableWidgetItem* itemStatus = new QTableWidgetItem(status);
        QTableWidgetItem* itemPriority = new QTableWidgetItem(QString::number(priority));
        QTableWidgetItem* itemAssignee = new QTableWidgetItem(QString::number(assignee));

        type->setFlags(type->flags() & ~Qt::ItemIsEditable);
        itemStoryID->setFlags(itemStoryID->flags() & ~Qt::ItemIsEditable);

        userStoriesTable->setItem(rowCount, 1, type);
        userStoriesTable->setItem(rowCount, 2, itemStoryName);
        userStoriesTable->setItem(rowCount, 3, itemDescription);
        userStoriesTable->setItem(rowCount, 4, itemStatus);
        userStoriesTable->setItem(rowCount, 5, itemPriority);
        userStoriesTable->setItem(rowCount, 6, itemAssignee);

        qDebug() << "Adding User Story to Table: ID:" << storyID
                 << " Title:" << storyName
                 << " Description:" << description
                 << " Status:" << status
                 << " Priority:" << priority
                 << " Assignee:" << assignee;
    } else {
        qDebug() << "Table view not found or accessible.";
    }
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
        QString status = userStoriesTable->item(row, 4)->text();
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
//FULL IMPLEMENTATION OF CREATION OF USER-STORIES AND EDIT FUCNTIONALITY    -----    END
//------------------------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//CREATION OF USER STORIES
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

void pb_productbacklog_implementation::on_createuserstories_backlog_clicked() {
    qDebug() << "Create user story button clicked in the pb class.";

    QString type_pb = "User Story";
    QString UserStoryName = QInputDialog::getText(nullptr, "Enter Userstory", "Userstory:");
    QString UserStoryDescription = QInputDialog::getText(nullptr, "Enter Userstory", "Userstory Description:");
    int priority = QInputDialog::getInt(nullptr, "Enter Priority", "Priority:", 1, 1, 3, 1);

    // Assuming default values for 'Status', 'Assignee', 'ProductBacklog_idProductBacklog', and 'ProductBacklog_Project_idProject'
    QString status = "New"; // Default status
    int assignee = 0; // Default assignee (adjust as needed)
    int productBacklogId = 1; // Default product backlog ID (adjust as needed)
    int projectId = 1; // Default project ID (adjust as needed)

    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);
        query.prepare("INSERT INTO scrummy.UserStoryPB (Title, Description, Priority, Status, Assignee, ProductBacklog_idProductBacklog, ProductBacklog_Project_idProject) "
                      "VALUES (:UserStoryName, :UserStoryDescription, :priority, :status, :assignee, :productBacklogId, :projectId)");

        query.bindValue(":UserStoryName", UserStoryName);
        query.bindValue(":UserStoryDescription", UserStoryDescription);
        query.bindValue(":priority", priority);
        query.bindValue(":status", status);
        query.bindValue(":assignee", assignee);
        query.bindValue(":productBacklogId", productBacklogId);
        query.bindValue(":projectId", projectId);

        if (query.exec()) {
            qDebug() << "User story added to the database successfully";
        } else {
            qDebug() << "Failed to add user story to the database:" << query.lastError().text();
        }

        dbobj.close();
    } else {
        qDebug() << "Connection Not Established - pb_productbacklog_implementation class - ! - Insert User Story";
    }

    // Add to the backlog view as well
    addBacklog(type_pb, UserStoryName, UserStoryDescription, priority);

    qDebug() << "Userstory: " << UserStoryName;
    qDebug() << "Userstory Description: " << UserStoryDescription;
    qDebug() << "Priority: " << priority;
}

void pb_productbacklog_implementation::addBacklog(const QString& type_pb,const QString& taskName, const QString& description, int priority) {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();
    userStoriesTable->setColumnCount(4); // Ensure there are four columns in the table
    userStoriesTable->setHorizontalHeaderLabels({"Type", "User Story", "Description", "Priority"});

    // Adjust the column widths to take up the available space
    QHeaderView* header = userStoriesTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    if (userStoriesTable) {
        int rowCount = userStoriesTable->rowCount(); // Get current row count
        userStoriesTable->insertRow(rowCount); // Insert a new row at the end

        QTableWidgetItem* type = new QTableWidgetItem(type_pb);
        QTableWidgetItem* itemTaskName = new QTableWidgetItem(taskName);
        QTableWidgetItem* itemDescription = new QTableWidgetItem(description);
        QTableWidgetItem* itemPriority = new QTableWidgetItem(QString::number(priority));

        //Making is non editable
        type->setFlags(type->flags() & ~Qt::ItemIsEditable);

        userStoriesTable->setItem(rowCount, 0, type); // Insert type in column 0
        userStoriesTable->setItem(rowCount, 1, itemTaskName); // Insert taskName in column 1
        userStoriesTable->setItem(rowCount, 2, itemDescription); // Insert description in column 2
        userStoriesTable->setItem(rowCount, 3, itemPriority); // Insert priority in column 3

    } else {
        qDebug() << "Table view not found or accessible.";
    }
}


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Creation of Tasks
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
void pb_productbacklog_implementation::on_createissue_clicked() {
    qDebug() << "Create issue button clicked.";

    QString taskTitle = QInputDialog::getText(nullptr, "Enter Title", "Title:");
    QString taskDescription = QInputDialog::getText(nullptr, "Enter Description", "Description:");
    QStringList statuses = {"To Do", "In Progress", "Done"};
    QString taskStatus = QInputDialog::getItem(nullptr, "Select Status", "Status:", statuses, 0, false);
    int taskPriority = QInputDialog::getInt(nullptr, "Enter Priority", "Priority:", 1, 1, 5, 1);
    QString taskAssignee = QInputDialog::getText(nullptr, "Enter Assignee", "Assignee:");

    addTaskToBacklog(taskTitle, taskDescription, taskStatus, taskPriority, taskAssignee);
}

void pb_productbacklog_implementation::addTaskToBacklog(const QString& title, const QString& description, const QString& status, int priority, const QString& assignee) {
    QTableWidget* table = parentBoard->getUserStoriesTableView();
    if (!table) {
        qDebug() << "Table view not found or accessible.";
        return;
    }

    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Title", "Description", "Status", "Priority", "Assignee"});

    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();
    if (!dbobj.isOpen()) {
        qDebug() << "Connection Not Established - pb_productbacklog_implmentation!";
        return;
    }

    QSqlQuery query(dbobj);
    query.prepare("INSERT INTO scrummy.TaskPB(Title, Description, Status, Priority, Assignee, ProductBacklog_idProductBacklog, ProductBacklog_Project_idProject)"
                  "VALUES (:title, :description, :status, :priority, :assignee, 1, 1)");
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":status", status);
    query.bindValue(":priority", priority);
    query.bindValue(":assignee", assignee);

    if (!query.exec()) {
        qDebug() << "Failed to insert data into TaskPB table:" << query.lastError().text();
        dbobj.close();
        return;
    }
    qDebug() << "Data inserted into TaskPB table successfully!";
    dbobj.close();

    int rowCount = table->rowCount();
    table->insertRow(rowCount);

    QTableWidgetItem* itemTitle = new QTableWidgetItem(title);
    QTableWidgetItem* itemDescription = new QTableWidgetItem(description);
    QTableWidgetItem* itemStatus = new QTableWidgetItem(status);
    QTableWidgetItem* itemPriority = new QTableWidgetItem(QString::number(priority));
    QTableWidgetItem* itemAssignee = new QTableWidgetItem(assignee);

    itemTitle->setFlags(itemTitle->flags() & ~Qt::ItemIsEditable);

    table->setItem(rowCount, 0, itemTitle);
    table->setItem(rowCount, 1, itemDescription);
    table->setItem(rowCount, 2, itemStatus);
    table->setItem(rowCount, 3, itemPriority);
    table->setItem(rowCount, 4, itemAssignee);
}


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------


















//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Creation of UserStories
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------


void pb_productbacklog_implementation::on_createUserStory_clicked() {
    qDebug() << "Create User Story button clicked.";

    QString title = QInputDialog::getText(nullptr, "Enter Title", "Title:");
    QString description = QInputDialog::getText(nullptr, "Enter Description", "Description:");
    QStringList statuses = {"To Do", "In Progress", "Done"};
    QString status = QInputDialog::getItem(nullptr, "Select Status", "Status:", statuses, 0, false);
    int priority = QInputDialog::getInt(nullptr, "Enter Priority", "Priority:", 1, 1, 5, 1);
    QString assignee = QInputDialog::getText(nullptr, "Enter Assignee", "Assignee:");

    addUserStoryToBacklog(title, description, status, priority, assignee);
}

void pb_productbacklog_implementation::addUserStoryToBacklog(const QString& title, const QString& description, const QString& status, int priority, const QString& assignee) {
    QTableWidget* table = parentBoard->getUserStoriesTableView();
    if (!table) {
        qDebug() << "Table view not found or accessible.";
        return;
    }

    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"ID", "Title", "Description", "Status", "Priority", "Assignee"});

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
}























void pb_productbacklog_implementation::SendTasksToSprints() {
    qDebug() << "SendTasksToSprints called";


    QComboBox* SprintBox = parentBoard->getSprintComboBox();
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();


    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);
        query.prepare("SELECT Title FROM scrummy.Sprint");

        if (query.exec()) {
            qDebug() << "Sprint Data Retrieved Successfully!";

            while (query.next()) {
                QString sprintTitle = query.value(0).toString(); // Assuming Title is in the first column of the result
                SprintBox->addItem(sprintTitle);
            }

        } else {
            qDebug() << "Failed to retrieve Sprint Data : " << query.lastError().text();
        }
        dbobj.close();
    } else {
        qDebug() << "Connection Not Established - pb_productbacklog_implementation class - ! - Sprint Data";
    }

}
