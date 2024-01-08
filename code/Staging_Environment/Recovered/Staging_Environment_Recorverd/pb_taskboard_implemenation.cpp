// pb_taskboard_implemenation.cpp
#include "pb_taskboard_implemenation.h"
#include "QtWidgets/qheaderview.h"
#include "parentboard.h"  // Include parentboard header
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QComboBox>
#include <QDebug>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDateTime>

pb_taskboard_implemenation::pb_taskboard_implemenation(parentboard* parentBoardInstance)
    : parentBoard(parentBoardInstance),
    scrollAreaLayout(parentBoardInstance->getScrollAreaLayout())  // Initialize the layout
{
    model = new QStandardItemModel(parentBoard);

    fetchSprintData();
    fetchSprintDates();
    generateUserTaskTree();

    // Resize QWidgetTree sections/columns
    QHeaderView* header = parentBoard->getTaskTreeWidget()->header();
    // header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    // header->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    // Fixed
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->resizeSection(1, 320); // Task section wider

    connect(parentBoard->getSprintDropdown(), QOverload<int>::of(&QComboBox::currentIndexChanged), this, &pb_taskboard_implemenation::updateLabels);
}

pb_taskboard_implemenation::~pb_taskboard_implemenation()
{
    delete model;
}

void pb_taskboard_implemenation::fetchSprintData()
{
    parentBoard->getSprintDropdown()->clear();

    parentBoard->getSprintDropdown()->addItem("All Sprints", -1);

    DatabaseManager database;
    QSqlDatabase db = database.getDatabase();

    // Passing project ID
    int PassedProjectID = parentBoard->getProjectId();
    qDebug() << "Project ID in Taskboard fetchSprint: " << PassedProjectID;

    if (db.isOpen()) {
        // qDebug() << "Connection Established - Taskboard class!";

        QSqlQuery query(db);
        query.prepare("SELECT "
                      "   idSprint, Title "
                      "FROM"
                      "   Sprint "
                      "WHERE Project_idProject = 1"); // value to replace with :projectId
        query.bindValue(":projectId", PassedProjectID);  // Use the stored project ID

        if (query.exec()) {
            while (query.next()) {
                int idSprint = query.value(0).toInt();
                QString title = query.value(1).toString();

                parentBoard->getSprintDropdown()->addItem(title, idSprint);

                // qDebug() << "Sprint data fetched successfully!";
            }
        } else {
            // qDebug() << "Failed to fetch Sprint data:" << query.lastError().text();
        }

        db.close();
    } else {
        // qDebug() << "Connection Not Established - Taskboard class!";
    }
}

void pb_taskboard_implemenation::generateUserTaskTree()
{
    // Clear existing items from the QTreeWidget
    parentBoard->getTaskTreeWidget()->clear();

    // Fetch User and Task data and generate the tree
    DatabaseManager database;
    QSqlDatabase db = database.getDatabase();

    // Passing Project ID and Sprint ID
    int PassedProjectID = parentBoard->getProjectId();
    int selectedSprintId = parentBoard->getSprintDropdown()->currentData().toInt();
    qDebug() << "Project ID in Taskboard generateUserTasks: " << PassedProjectID;
    // qDebug() << "Selected Sprint ID: " << selectedSprintId;

    {
        DatabaseManager database;
        QSqlDatabase db = database.getDatabase();

        if (db.isOpen()) {
            // qDebug() << "Connection Established - Taskboard class!";

            QSqlQuery query(db);

            // Check if "All Sprints" is selected
            if (selectedSprintId == -1)
            {
                // If "All Sprints" is selected, don't filter by sprint ID
                query.prepare("SELECT "
                              "   User.FirstName, "
                              "   User.LastName, "
                              "   TaskSB.Title, "
                              "   TaskSB.Priority, "
                              "   TaskSB.Status, "
                              "   UserStorySB.Title, "
                              "   UserStorySB.Priority, "
                              "   UserStorySB.Status "
                              "FROM "
                              "   User "
                              "LEFT JOIN "
                              "   TaskSB ON User.idUser = TaskSB.Assignee "
                              "   AND TaskSB.SprintBacklog_Sprint_Project_idProject = 1 "
                              "LEFT JOIN "
                              "   UserStorySB ON User.idUser = UserStorySB.Assignee "
                              "   AND UserStorySB.SprintBacklog_Sprint_Project_idProject = 1 "
                              "ORDER BY User.Username; ");
            }
            else
            {
                query.prepare("SELECT "
                              "   User.FirstName, "
                              "   User.LastName, "
                              "   TaskSB.Title, "
                              "   TaskSB.Priority, "
                              "   TaskSB.Status, "
                              "   UserStorySB.Title, "
                              "   UserStorySB.Priority, "
                              "   UserStorySB.Status "
                              "FROM "
                              "   User "
                              "LEFT JOIN "
                              "   TaskSB ON User.idUser = TaskSB.Assignee "
                              "   AND TaskSB.SprintBacklog_Sprint_Project_idProject = 1 "
                              "   AND TaskSB.SprintBacklog_Sprint_idSprint = :sprintId "
                              "LEFT JOIN "
                              "   UserStorySB ON User.idUser = UserStorySB.Assignee "
                              "   AND UserStorySB.SprintBacklog_Sprint_Project_idProject = 1 " // value to replace with :projectId
                              "   AND UserStorySB.SprintBacklog_Sprint_idSprint = :sprintId "
                              "ORDER BY User.Username; ");

                query.bindValue(":sprintId", selectedSprintId);

            }

            query.bindValue(":projectId", PassedProjectID);

            QMap<QString, QTreeWidgetItem*> userItemMap;

            if (query.exec()) {
                while (query.next()) {
                    QString firstName = query.value(0).toString();
                    QString lastName = query.value(1).toString();
                    QString taskTitle = query.value(2).toString();
                    QString taskPriority = query.value(3).toString();
                    QString taskStatus = query.value(4).toString();
                    QString storyTitle = query.value(5).toString();
                    QString storyPriority = query.value(6).toString();
                    QString storyStatus = query.value(7).toString();
                    // qDebug() << "firstName: " << firstName;
                    // qDebug() << "lastName: " << lastName;
                    // qDebug() << "taskTitle: " << taskTitle;
                    // qDebug() << "taskPriority: " << taskPriority;
                    // qDebug() << "taskStatus: " << taskStatus;

                    QString name = firstName + " " + lastName;
                    // qDebug() << "HERE name: " << name;


                    // Find or create treeUserItem in the QTreeWidget
                    QTreeWidgetItem* treeUserItem = userItemMap.value(name);

                    if (!treeUserItem) {
                        treeUserItem = new QTreeWidgetItem(parentBoard->getTaskTreeWidget());
                        treeUserItem->setText(0, name);
                        // treeUserItem->setData(0, Qt::UserRole, "username");
                        userItemMap.insert(name, treeUserItem);
                    }

                    // Add task for the current user
                    if (!taskTitle.isEmpty()) {
                        QTreeWidgetItem* treeTaskItem = new QTreeWidgetItem(treeUserItem);
                        treeTaskItem->setText(1, taskTitle);
                        treeTaskItem->setText(2, taskPriority);
                        treeTaskItem->setText(3, "Task");
                        // treeTaskItem->setData(0, Qt::UserRole, "task");
                        treeTaskItem->setText(4, taskStatus);
                    }

                    // Add user story for the current user
                    if (!storyTitle.isEmpty()) {
                        QTreeWidgetItem* treeStoryItem = new QTreeWidgetItem(treeUserItem);
                        treeStoryItem->setText(1, storyTitle);
                        treeStoryItem->setText(2, storyPriority);
                        treeStoryItem->setText(3, "User Story");
                        // treeStoryItem->setData(0, Qt::UserRole, "userstory");
                        treeStoryItem->setText(4, storyStatus);
                    }

                    // qDebug() << "User and task/user story data fetched successfully!";
                }
                generateUnassigned();
            } else {
                // qDebug() << "Failed to fetch user and task/user story data:" << query.lastError().text();
            }

            db.close();
        } else {
            // qDebug() << "Connection Not Established - Taskboard class!";
        }
    }
}

void pb_taskboard_implemenation::generateUnassigned()
{
    QTreeWidgetItem* unassignedItem = new QTreeWidgetItem(parentBoard->getTaskTreeWidget());
    unassignedItem->setText(0, "Unassigned");

    DatabaseManager database;
    QSqlDatabase db = database.getDatabase();

    // Passing Project ID and Sprint ID
    int PassedProjectID = parentBoard->getProjectId();
    int selectedSprintId = parentBoard->getSprintDropdown()->currentData().toInt();
    qDebug() << "Project ID in Taskboard generateUnassigned: " << PassedProjectID;
    // qDebug() << "Selected Sprint ID generateUnassigned: " << selectedSprintId;

    if (db.isOpen()) {
        QSqlQuery query(db);

        // Check if "All Sprints" is selected
        if (selectedSprintId == -1)
        {
            query.prepare("SELECT "
                          "   TaskSB.Title, "
                          "   TaskSB.Priority, "
                          "   TaskSB.Status, "
                          "   UserStorySB.Title, "
                          "   UserStorySB.Priority, "
                          "   UserStorySB.Status "
                          "FROM "
                          "   TaskSB, UserStorySB "
                          "WHERE "
                          "   TaskSB.Assignee IS NULL AND UserStorySB.Assignee IS NULL "
                          "   AND TaskSB.SprintBacklog_Sprint_Project_idProject = 1 "
                          "ORDER BY TaskSB.Title");
        }
        else
        {
            query.prepare("SELECT "
                          "   TaskSB.Title, "
                          "   TaskSB.Priority, "
                          "   TaskSB.Status, "
                          "   UserStorySB.Title, "
                          "   UserStorySB.Priority, "
                          "   UserStorySB.Status "
                          "FROM "
                          "   TaskSB, UserStorySB "
                          "WHERE "
                          "   TaskSB.Assignee IS NULL AND UserStorySB.Assignee IS NULL "
                          "   AND TaskSB.SprintBacklog_Sprint_Project_idProject = 1 "
                          "   AND TaskSB.SprintBacklog_Sprint_idSprint = :sprintId "
                          "ORDER BY TaskSB.Title");

            query.bindValue(":sprintId", selectedSprintId);
        }

        query.bindValue(":projectId", PassedProjectID);

        if (query.exec()) {
            while (query.next()) {
                QString taskTitle = query.value(0).toString();
                QString taskPriority = query.value(1).toString();
                QString taskStatus = query.value(2).toString();
                QString storyTitle = query.value(3).toString();
                QString storyPriority = query.value(4).toString();
                QString storyStatus = query.value(5).toString();

                // Add task for unassigned
                if (!taskTitle.isEmpty()) {
                    QTreeWidgetItem* treeTaskItem = new QTreeWidgetItem(unassignedItem);
                    treeTaskItem->setText(1, taskTitle);
                    treeTaskItem->setText(2, taskPriority);
                    treeTaskItem->setText(3, "Task");
                    treeTaskItem->setText(4, taskStatus);
                }

                // Add user story for unassigned
                if (!storyTitle.isEmpty()) {
                    QTreeWidgetItem* treeStoryItem = new QTreeWidgetItem(unassignedItem);
                    treeStoryItem->setText(1, storyTitle);
                    treeStoryItem->setText(2, storyPriority);
                    treeStoryItem->setText(3, "User Story");
                    treeStoryItem->setText(4, storyStatus);
                }
            }
        } else {
            // qDebug() << "Failed to fetch unassigned tasks and user stories:" << query.lastError().text();
        }

        db.close();
    } else {
        // qDebug() << "Connection Not Established - Taskboard class!";
    }
}

void pb_taskboard_implemenation::fetchSprintDates()
{
    DatabaseManager database;
    QSqlDatabase db = database.getDatabase();

    // Passing Project ID and Sprint ID
    int PassedProjectID = parentBoard->getProjectId();
    int selectedSprintId = parentBoard->getSprintDropdown()->currentData().toInt();
    qDebug() << "Project ID in Taskboard fetchDates: " << PassedProjectID;
    // qDebug() << "Selected Sprint ID in fetchDates: " << selectedSprintId;

    if (db.isOpen()) {
        QSqlQuery query(db);

        if (selectedSprintId == -1)
        {
            parentBoard->updateSprintDateLabel("-");
        }
        else
        {
            query.prepare("SELECT StartDate, EndDate FROM Sprint "
                          "WHERE  Project_idProject = 1 "
                          "AND idSprint = :sprintId");
            query.bindValue(":projectId", PassedProjectID);
            query.bindValue(":sprintId", selectedSprintId);

            if (query.exec() && query.next()) {
                QString startDate = query.value(0).toString();
                QString endDate = query.value(1).toString();

                if (startDate.isEmpty() || endDate.isEmpty())
                {
                    parentBoard->updateSprintDateLabel("Missing Date");
                }
                else
                {
                // Format the StartDate and EndDate
                startDate = startDate.mid(5, 5);
                endDate = endDate.mid(5, 5);
                startDate.replace("-", ".");
                endDate.replace("-", ".");

                // Update the label with the new text
                parentBoard->updateSprintDateLabel(startDate + " - " + endDate);
                // qDebug() << "Dates: " << startDate << " - " << endDate;
                }
            } else {
                // qDebug() << "Failed to fetch Sprint dates:" << query.lastError().text();
            }
        }

        db.close();
    } else {
        // qDebug() << "Connection Not Established - Taskboard class!";
    }
}

void pb_taskboard_implemenation::updateLabels(int index)
{
    // int selectedIdSprint = parentBoard->getSprintDropdown()->currentData().toInt();

    QComboBox* sprintDropdown = parentBoard->getSprintDropdown();

    // Fetch Sprint ID for User and Task generation
    QVariant selectedData = sprintDropdown->currentData();
    if (selectedData.isValid()) {
        selectedSprintId = selectedData.toInt();
        qDebug() << "Selected Dropdown Sprint ID: " << selectedSprintId;

        // Fetch Sprint dates and update the label
        fetchSprintDates();

        // Update the tree widget
        generateUserTaskTree();

    } else {
        // qDebug() << "Invalid selected data in the sprint dropdown.";
        selectedSprintId = -2;
    }
}
int pb_taskboard_implemenation::getSelectedSprintId() const
{
    return selectedSprintId;
}
