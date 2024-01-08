#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QDebug> // Include qDebug for debugging
#include <QTableView>
#include "pb_productbacklog_implementation.h"


pb_productbacklog_implementation::pb_productbacklog_implementation(parentboard* parentBoardInstance) {
    // Initialize any necessary variables or connections
    parentBoard = parentBoardInstance;
}


//------------------------------------------------------------------------------------------------------------------------------

void pb_productbacklog_implementation::on_createuserstories_backlog_clicked() {
    qDebug() << "Create user story button clicked in the pb class.";

    QString taskName = QInputDialog::getText(nullptr, "Enter Userstory", "Userstory:");
    QString taskDescription = QInputDialog::getText(nullptr, "Enter Userstory", "Userstory Description:");
    int priority = QInputDialog::getInt(nullptr, "Enter Priority", "Priority:", 1, 1, 3, 1);

    addBacklog(taskName, taskDescription, priority); // Use this-> to refer to member function
    qDebug() << "Userstory: " << taskName;
    qDebug() << "Userstory Description: " << taskDescription;
    qDebug() << "Priority: " << priority;

}

void pb_productbacklog_implementation::addBacklog(const QString& taskName, const QString& description, int priority) {
    QTableWidget* userStoriesTable = parentBoard->getUserStoriesTableView();

    if (userStoriesTable) {
        int rowCount = userStoriesTable->rowCount(); // Get current row count
        userStoriesTable->insertRow(rowCount); // Insert a new row at the end

        QTableWidgetItem* itemTaskName = new QTableWidgetItem(taskName);
        QTableWidgetItem* itemDescription = new QTableWidgetItem(description);
        QTableWidgetItem* itemPriority = new QTableWidgetItem(QString::number(priority));

        userStoriesTable->setItem(rowCount, 0, itemTaskName); // Insert taskName in column 0
        userStoriesTable->setItem(rowCount, 1, itemDescription); // Insert description in column 1
        userStoriesTable->setItem(rowCount, 2, itemPriority); // Insert priority in column 2
    } else {
        qDebug() << "Table view not found or accessible.";
    }
}


//------------------------------------------------------------------------------------------------------------------------------


void pb_productbacklog_implementation::on_createissues_clicked(){
    qDebug() << "Create issue button clicked.";

    QString taskName = QInputDialog::getText(nullptr, "Enter Issue", "Issue:");
    QString taskDescription = QInputDialog::getText(nullptr, "Enter Issue Description", "Issue Description:");
    bool ok;
    int priority = QInputDialog::getInt(nullptr, "Enter Priority", "Priority:", 1, 1, 3, 1);


    addIssues(taskName, taskDescription, priority);
    qDebug() << "Issue: " << taskName;
    qDebug() << "Issue Description: " << taskDescription;
    qDebug() << "Priority: " << priority;

}

void pb_productbacklog_implementation::addIssues(const QString& taskName, const QString& description, int priority) {

    QTableWidget* IssuesTable = parentBoard->getIssuesTableView();
    if (IssuesTable) {
        int IssuerowCount = IssuesTable->rowCount(); // Get current row count
        IssuesTable->insertRow(IssuerowCount); // Insert a new row at the end

        QTableWidgetItem* issueTaskName = new QTableWidgetItem(taskName);
        QTableWidgetItem* issueDescription = new QTableWidgetItem(description);
        QTableWidgetItem* issuePriority = new QTableWidgetItem(QString::number(priority));

        IssuesTable->setItem(IssuerowCount, 0, issueTaskName); // Insert taskName in column 0
        IssuesTable->setItem(IssuerowCount, 1, issueDescription); // Insert description in column 1
        IssuesTable->setItem(IssuerowCount, 2, issuePriority); // Insert priority in column 2
    } else {
        qDebug() << "Table view not found or accessible.";
    }
}

//------------------------------------------------------------------------------------------------------------------------------

