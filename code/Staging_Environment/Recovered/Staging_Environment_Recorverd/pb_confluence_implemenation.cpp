#include "pb_confluence_implemenation.h"
#include "parentboard.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QDebug> // Include qDebug for debugging
#include "parentboard.h"
#include <QTableView>
#include "qmessagebox.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>


PB_Confluence_Implemenation::PB_Confluence_Implemenation(QWidget *parent)
    : QObject(parent), parentBoard(qobject_cast<parentboard*>(parent)){
    parentBoard->getCreationBox_Confluence()->setVisible(false);
    parentBoard->get_InputTitle_2()->setVisible(false);
    parentBoard->get_InputGroup_Confluence_Heading()->setVisible(false);
    parentBoard->get_Confluence_NameText()->setVisible(false);
    parentBoard->get_Confluence_HeadingText()->setVisible(false);
    parentBoard->get_Confluence_GroupText()->setVisible(false);
    parentBoard->get_Button_Create_Confluence()->setVisible(false);
    parentBoard->getuserTaskTreeWidget_2()->setVisible(true);
}

parentboard* PB_Confluence_Implemenation::getParentBoard() {
    return parentBoard;
}

void PB_Confluence_Implemenation::HideCreationSection()
{
    int PassedProjectID = parentBoard->getProjectId();
    QTreeWidget* ConfluenceTable = parentBoard->getuserTaskTreeWidget_2();
    ConfluenceTable->clear();
    ConfluenceTable->setGeometry(10, 75, 1250, 550);
    parentBoard->getCreationBox_Confluence()->setVisible(false);
    parentBoard->get_InputTitle_2()->setVisible(false);
    parentBoard->get_InputGroup_Confluence_Heading()->setVisible(false);
    parentBoard->get_Confluence_NameText()->setVisible(false);
    parentBoard->get_Confluence_HeadingText()->setVisible(false);
    parentBoard->get_Confluence_GroupText()->setVisible(false);
    parentBoard->get_Button_Create_Confluence()->setVisible(false);
    parentBoard->getuserTaskTreeWidget_2()->setVisible(true);

    // Retrieve Confluence Current Data
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();
    QSqlQuery query(dbobj);
    // Prepare the query to get the GroupNames, idGroup, and corresponding PageNames
    query.prepare("SELECT cg.GroupName, cg.idGroup, p.PageName, p.idPage "
                  "FROM ConfluenceGroup AS cg "
                  "LEFT JOIN Page AS p ON cg.idGroup = p.ConfluenceGroup_idGroup "
                  "WHERE cg.Project_idProject = :ProjectID "
                  "ORDER BY cg.GroupName, p.PageName");
    query.bindValue(":ProjectID", PassedProjectID);

    if (query.exec()) {
        QMap<QString, QTreeWidgetItem*> groupItems;
        while (query.next()) {
            QString groupName = query.value(0).toString();
            int idGroup = query.value(1).toInt(); // Retrieve idGroup
            QString pageName = query.value(2).toString();
            // idPage is retrieved but not used directly here
            // int idPage = query.value(3).toInt(); // Retrieve idPage if needed

            // Check if the groupName item already exists
            QTreeWidgetItem* groupItem;
            if (!groupItems.contains(groupName)) {
                // Create a new item for the groupName if it doesn't exist
                groupItem = new QTreeWidgetItem(ConfluenceTable);
                groupItem->setText(0, groupName);
                // Store idGroup as user data in the tree item
                groupItem->setData(0, Qt::UserRole, idGroup);
                groupItems.insert(groupName, groupItem);
            } else {
                // Get the existing item for the groupName
                groupItem = groupItems.value(groupName);
            }

            // If there is a pageName, create a child item for it under the groupName
            if (!pageName.isEmpty()) {
                QTreeWidgetItem* pageItem = new QTreeWidgetItem();
                pageItem->setText(0, pageName);
                // Optionally store idPage as user data in the tree item
                // pageItem->setData(0, Qt::UserRole, idPage);
                groupItem->addChild(pageItem);
            }
        }
    }

    // Ensure that the ConfluenceTable is updated to display the new items
    ConfluenceTable->update();
}


/**
 * @brief Slot function called when an item in the tree widget is clicked.
 *
 * This function handles the click event of an item in the tree widget and allows
 * the user to update the content associated with the selected page.
 *
 * @param item The clicked tree widget item.
 * @param column The column index of the clicked item.
 */
void PB_Confluence_Implemenation::onTreeItemClicked(QTreeWidgetItem* item, int column) {
    if (item && item->parent()) {  // Ensure it's a sub-item
        QString pageName = item->text(0);
        QVariant idGroupVariant = item->parent()->data(0, Qt::UserRole);

        // Validate idGroup
        if (!idGroupVariant.isValid() || idGroupVariant.toInt() <= 0) {
            qDebug() << "Invalid idGroup for pageName:" << pageName;
            return;  // Exit if idGroup is not valid
        }
        int idGroup = idGroupVariant.toInt();

        qDebug() << "Updating content for pageName:" << pageName << ", idGroup:" << idGroup;

        // Create and show the dialog
        QDialog *editorDialog = new QDialog(parentBoard);
        editorDialog->resize(800, 600);
        QVBoxLayout *layout = new QVBoxLayout(editorDialog);
        QTextEdit *textEditor = new QTextEdit(editorDialog);
        QPushButton *saveButton = new QPushButton("Save", editorDialog);
        QPushButton *closeButton = new QPushButton("Close", editorDialog);

        // Add widgets to layout
        layout->addWidget(textEditor);
        layout->addWidget(saveButton);
        layout->addWidget(closeButton);
        editorDialog->setLayout(layout);

        // Load the content from the database
        DatabaseManager database;
        QSqlDatabase dbobj = database.getDatabase();
        QSqlQuery query(dbobj);
        query.prepare("SELECT Content FROM Page WHERE PageName = :PageName AND ConfluenceGroup_idGroup = :idGroup");
        query.bindValue(":PageName", pageName);
        query.bindValue(":idGroup", idGroup);
        if(query.exec() && query.next()) {
            QString content = query.value(0).toString();
            textEditor->setPlainText(content);
        }

        // Connect the Save button
        connect(saveButton, &QPushButton::clicked, [this, textEditor, pageName, idGroup](){
            DatabaseManager database;
            QSqlDatabase dbobj = database.getDatabase();
            if (dbobj.isOpen() && dbobj.isValid()) {
                QSqlQuery query(dbobj);
                qDebug() << "Attempting to update content for pageName:" << pageName << ", idGroup:" << idGroup;
                qDebug() << "New content to save:" << textEditor->toPlainText();
                query.prepare("UPDATE Page SET Content = :Content WHERE PageName = :PageName AND ConfluenceGroup_idGroup = :idGroup");
                query.bindValue(":Content", textEditor->toPlainText());
                query.bindValue(":PageName", pageName);
                query.bindValue(":idGroup", idGroup);

                dbobj.transaction();
                // Inside the save button clicked lambda function
                if (!query.exec()) {
                    qDebug() << "Error updating content:" << query.lastError().text();
                    dbobj.rollback();
                } else {
                    int rowsAffected = query.numRowsAffected();
                    qDebug() << "Rows affected by the update:" << rowsAffected;
                    if (rowsAffected > 0) {
                        if (!dbobj.commit()) {
                            qDebug() << "Error committing changes:" << dbobj.lastError().text();
                        } else {
                            qDebug() << "Update and commit were successful, rows affected:" << rowsAffected;
                        }
                    } else {
                        qDebug() << "No rows affected by the update, rolling back.";
                        dbobj.rollback();
                    }
                }


            } else {
                qDebug() << "Database is not open or is not valid";
            }
        });



        // Connect the Close button
        connect(closeButton, &QPushButton::clicked, editorDialog, &QDialog::reject);

        // Show the dialog
        editorDialog->exec();
    }
}




/**
 * @brief Displays the create window for a new Confluence page.
 *
 * This function makes various UI elements visible to allow the user to create a new Confluence page.
 * It also populates a combo box with available Confluence groups for selection.
 */
void PB_Confluence_Implemenation::ShowCreateWindow_Page(){

    parentBoard->getCreationBox_Confluence()->setVisible(true);
    parentBoard->get_InputTitle_2()->setVisible(true);
    parentBoard->get_InputGroup_Confluence_Heading()->setVisible(true);
    parentBoard->get_Confluence_NameText()->setVisible(true);
    parentBoard->get_Confluence_HeadingText()->setVisible(true);
    parentBoard->get_Confluence_GroupText()->setVisible(true);
    //LowerButtons
    parentBoard->get_Button_CreatePage_Confluence()->setVisible(true);
    parentBoard->get_Button_Create_Confluence()->setVisible(false);

    QComboBox* GroupComboBox= parentBoard->get_InputGroup_Confluence_Heading();
    GroupComboBox->clear();

    // Retrieve the GroupName values from the database
    int PassedProjectID = parentBoard->getProjectId();  // Assuming you have the project ID available here

    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (dbobj.isOpen() && dbobj.isValid()) {
        QSqlQuery query(dbobj);
        query.prepare("SELECT GroupName FROM ConfluenceGroup WHERE Project_idProject = :Project_idProject");
        query.bindValue(":Project_idProject", PassedProjectID);

        if (!query.exec()) {
            QMessageBox::critical(nullptr, "Database Error", query.lastError().text());
        } else {
            while (query.next()) {
                QString groupName = query.value(0).toString();
                GroupComboBox->addItem(groupName);
            }
            if (GroupComboBox->count() == 0) {
                QMessageBox::information(nullptr, "No Groups Found", "No groups found for this project.");
            }
        }
    } else {
        QMessageBox::critical(nullptr, "Database Error", "Cannot connect to the database.");
    }
}

/**
 * @brief Creates a new Confluence page with the provided title and selected group.
 *
 * This function retrieves the selected group name from the combo box, fetches the corresponding group ID from the database,
 * and inserts a new page into the database with the provided title and group ID.
 */


void PB_Confluence_Implemenation::CreatePageFunction(){
    QTextEdit* Title = parentBoard->get_InputTitle_2();
    QComboBox* GroupComboBox = parentBoard->get_InputGroup_Confluence_Heading();
    int PassedProjectID = parentBoard->getProjectId();  // Assuming you have the project ID available here

    // Get the current selected value from GroupComboBox
    QString selectedGroupName = GroupComboBox->currentText();

    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (dbobj.isOpen() && dbobj.isValid()) {
        // Begin a transaction
        dbobj.transaction();

        QSqlQuery query(dbobj);
        // Prepare the query to get the idGroup based on the GroupName
        query.prepare("SELECT idGroup FROM ConfluenceGroup WHERE GroupName = :GroupName AND Project_idProject = :ProjectID");
        query.bindValue(":GroupName", selectedGroupName);
        query.bindValue(":ProjectID", PassedProjectID);

        if (query.exec() && query.next()) {
            int idGroup = query.value(0).toInt();

            // Prepare the query to insert a new page
            QSqlQuery insertQuery(dbobj);
            insertQuery.prepare("INSERT INTO Page (PageName, Content, ConfluenceGroup_idGroup, ConfluenceGroup_Project_idProject) VALUES (:PageName, :Content, :idGroup, :ProjectID)");
            insertQuery.bindValue(":PageName", Title->toPlainText());
            insertQuery.bindValue(":Content", "");
            insertQuery.bindValue(":idGroup", idGroup);
            insertQuery.bindValue(":ProjectID", PassedProjectID);

            // Execute the insert query
            if (insertQuery.exec()) {
                // Commit the transaction if the insert was successful
                dbobj.commit();
            } else {
                // Rollback the transaction in case of an error
                dbobj.rollback();
            }
        } else {
            // Handle the error if the SELECT query didn't work
            qDebug() << "Error getting idGroup from ConfluenceGroup: " << query.lastError().text();
            // Optionally rollback or handle the transaction
            dbobj.rollback();
        }
    } else {
        qDebug() << "Database not open or not valid";
    }
}


/**
 * @brief Displays the create window for a new Confluence group.
 *
 * This function sets up the UI elements for creating a new Confluence group.
 */


void PB_Confluence_Implemenation::ShowCreateWindow(){
    QTreeWidget* ConfluenceTable = parentBoard->getuserTaskTreeWidget_2();
    ConfluenceTable->setGeometry(10, 75, 800, 550);
    parentBoard->getCreationBox_Confluence()->setVisible(true);
    parentBoard->get_InputTitle_2()->setVisible(true);
    parentBoard->get_InputGroup_Confluence_Heading()->setVisible(false);
    parentBoard->get_Confluence_NameText()->setVisible(true);
    parentBoard->get_Confluence_HeadingText()->setVisible(true);
    parentBoard->get_Confluence_GroupText()->setVisible(false);
    parentBoard->get_Button_Create_Confluence()->setVisible(true);
    QComboBox* GroupInput = parentBoard->get_InputGroup_Confluence_Heading();
    //LowerButtons
    parentBoard->get_Button_CreatePage_Confluence()->setVisible(false);
    parentBoard->get_Button_Create_Confluence()->setVisible(true);

}


/**
 * @brief Creates a new Confluence group with the provided group name.
 *
 * This function inserts a new Confluence group into the database with the provided group name.
 */


void PB_Confluence_Implemenation::CreateGroupFunction(){
    GroupName = parentBoard->get_InputTitle_2()->toPlainText();
    int PassedProjectID = parentBoard->getProjectId();

    if (GroupName.isEmpty()) {
        QMessageBox::warning(nullptr, "Required Fields", "Please fill up all the required fields.");
    } else {
        DatabaseManager database;
        QSqlDatabase dbobj = database.getDatabase();

        if (dbobj.isOpen() && dbobj.isValid()) {
            // Start a transaction
            dbobj.transaction();

            // Insert into ConfluenceGroup table
            QSqlQuery query(dbobj);
            query.prepare("INSERT INTO ConfluenceGroup (GroupName, Project_idProject) VALUES (:GroupName, :Project_idProject)");
            query.bindValue(":GroupName", GroupName);
            query.bindValue(":Project_idProject", PassedProjectID);

            if (!query.exec()) {
                QMessageBox::critical(nullptr, "Database Error", query.lastError().text());
                dbobj.rollback(); // Rollback transaction on failure
                return;
            }
            /*
            // Get the last inserted idGroup
            QVariant lastGroupId = query.lastInsertId();

            // Insert into Page table
            query.prepare("INSERT INTO Page (PageName, Content, ConfluenceGroup_idGroup, ConfluenceGroup_Project_idProject) "
                          "VALUES (:PageName, :Content, :ConfluenceGroup_idGroup, :ConfluenceGroup_Project_idProject)");
            query.bindValue(":PageName", GroupName);
            query.bindValue(":Content", "");
            query.bindValue(":ConfluenceGroup_idGroup", lastGroupId);
            query.bindValue(":ConfluenceGroup_Project_idProject", PassedProjectID);

            if (!query.exec()) {
                QMessageBox::critical(nullptr, "Failed to insert data into Page table", query.lastError().text());
                dbobj.rollback(); // Rollback transaction on failure
                return;
            }
        */

            // Commit the transaction
            dbobj.commit();
            QMessageBox::information(nullptr, "Success", "The group and page have been added successfully.");
        } else {
            QMessageBox::critical(nullptr, "Database Error", "Cannot connect to the database.");
        }
    }

    // Hide UI elements after insertion
    QTreeWidget* ConfluenceTable = parentBoard->getuserTaskTreeWidget_2();
    ConfluenceTable->setGeometry(10, 75, 1250, 550);
    parentBoard->get_InputGroup_Confluence_Heading()->setVisible(false);
    parentBoard->getCreationBox_Confluence()->setVisible(false);
    parentBoard->get_InputTitle_2()->setVisible(false);
    parentBoard->get_InputGroup_Confluence_Heading()->setVisible(false);
    parentBoard->get_Confluence_NameText()->setVisible(false);
    parentBoard->get_Confluence_HeadingText()->setVisible(false);
    parentBoard->get_Confluence_GroupText()->setVisible(false);
    parentBoard->get_Button_Create_Confluence()->setVisible(false);
}
