#include "pb_sprint_implemenation.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QDebug> // Include qDebug for debugging
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "qboxlayout.h"
#include "qheaderview.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "databasemanager.h"
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>


pb_sprint_implemenation::pb_sprint_implemenation(parentboard* parentBoardInstance)
{
    parentBoard = parentBoardInstance;
    OBJECTS_VISIBLE_BOXES();
    parentBoard->getSprintTableView()->setColumnCount(5);
    parentBoard->getSprintTableView()->setHorizontalHeaderLabels({"ID", "StartDate", "EndDate","Title", "Additional Details"}); // Set column headers
    TaskSBretrieval();
}


void pb_sprint_implemenation::OBJECTS_VISIBLE_BOXES(){

    // Initialize the sprintGroupBox member variable
    sprintGroupBox = parentBoard->getSprintGroupBox();  // Assuming you have a function to get the QGroupBox pointer
    sprintGroupBox->setVisible(false);  // Set the initial visibility to false
    parentBoard->get_Line_EditSprintEndDate()->setVisible(false);
    parentBoard->get_Label_SprintTitle()->setVisible(false);
    parentBoard->get_Label_TaskName()->setVisible(false);
    parentBoard->get_Table_SprintDetails()->setVisible(false);
    parentBoard->get_GroupBox_SprintGroup()->setVisible(false);
    parentBoard->get_Button_CreateSprintTab()->setVisible(false);
    parentBoard->get_DateEdit_InputEndDate()->setVisible(false);
    parentBoard->get_TextEdit_InputSprintGoal()->setVisible(false);
    parentBoard->get_LineEdit_InputSprintName()->setVisible(false);
    parentBoard->get_DateEdit_InputStartDate()->setVisible(false);
    parentBoard->get_DateEdit_Text_EndDate()->setVisible(false);
    parentBoard->get_Label_TextName()->setVisible(false);
    parentBoard->get_Label_TextSprintGoal()->setVisible(false);
    parentBoard->get_Label_TextStartDate()->setVisible(false);
    parentBoard->get_Sprint_View_Box()->setVisible(false);
    parentBoard->get_LineEdit_SprintName()->setVisible(false);
    parentBoard->get_LineEdit_SprintStartDate()->setVisible(false);
    parentBoard->get_Label_Sprint_EndDate()->setVisible(false);
    parentBoard->get_Label_SprintStartDate()->setVisible(false);
}
void pb_sprint_implemenation::OBJECTS_VISIBLE_on_create_sprint_clicked(){

    parentBoard->get_GroupBox_SprintGroup()->setVisible(true);
    parentBoard->get_Button_CreateSprintTab()->setVisible(true);
    parentBoard->get_DateEdit_InputEndDate()->setVisible(true);
    parentBoard->get_TextEdit_InputSprintGoal()->setVisible(true);
    parentBoard->get_LineEdit_InputSprintName()->setVisible(true);
    parentBoard->get_DateEdit_InputStartDate()->setVisible(true);
    parentBoard->get_DateEdit_Text_EndDate()->setVisible(true);
    parentBoard->get_Label_TextName()->setVisible(true);
    parentBoard->get_Label_TextSprintGoal()->setVisible(true);
    parentBoard->get_Label_TextStartDate()->setVisible(true);
}
void pb_sprint_implemenation::OBJECTS_VISIBLE_Additional_Details(){
    parentBoard->get_Line_EditSprintEndDate()->setVisible(true);
    parentBoard->get_Label_SprintTitle()->setVisible(true);
    parentBoard->get_Table_SprintDetails()->setVisible(true);
    parentBoard->get_Label_TaskName()->setVisible(true);
    parentBoard->get_Sprint_View_Box()->setVisible(true);
    parentBoard->get_LineEdit_SprintName()->setVisible(true);
    parentBoard->get_LineEdit_SprintStartDate()->setVisible(true);
    parentBoard->get_Label_Sprint_EndDate()->setVisible(true);
    parentBoard->get_Label_SprintStartDate()->setVisible(true);
}







void pb_sprint_implemenation::RetrieveAndDisplayTask() {
    clearTaskTable(); // Clears the table before adding new entries
    TaskSBretrieval(); // Retrieves tasks and adds them to the table

}

void pb_sprint_implemenation::clearTaskTable() {
    QTableWidget* sprint_table = parentBoard->getSprintTableView();
    sprint_table->clearContents();
    sprint_table->setRowCount(0);
}

void pb_sprint_implemenation::TaskSBretrieval() {

    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();
    int PassedProjectID = parentBoard->getProjectId();

    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);
        query.prepare("SELECT idSprint, StartDate, EndDate, Title FROM Sprint WHERE Project_idProject = :projectID");
        query.bindValue(":projectID", PassedProjectID);

        if (query.exec()) {
            qDebug() << "Sprint Retrieved Successfully!";

            while (query.next()) {
                int idSprint = query.value(0).toInt();
                // Extracting only the date part before the 'T'
                QString StartDate = query.value(1).toString().split("T").first();
                QString EndDate = query.value(2).toString().split("T").first();
                QString Title = query.value(3).toString();
                addSprint(idSprint, StartDate, EndDate, Title);
            }
        } else {
            qDebug() << "Failed to retrieve data: " << query.lastError().text();
        }
        dbobj.close();
    } else {
        qDebug() << "Connection Not Established - pb_productbacklog_implementation class! - Task";
    }
}


void pb_sprint_implemenation::addSprint(int idSprint, const QString& StartDate, const QString& EndDate, const QString& Title) {
    QTableWidget* sprint_table = parentBoard->getSprintTableView();
    QHeaderView* header = sprint_table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    sprint_table->setColumnHidden(0, true);

    if (sprint_table) {
        int IssuerowCount = sprint_table->rowCount(); // Get current row count
        sprint_table->insertRow(IssuerowCount); // Insert a new row at the end

        // Convert int to QString
        QTableWidgetItem* ID = new QTableWidgetItem(QString::number(idSprint));
        QTableWidgetItem* SprintstartdateItem = new QTableWidgetItem(StartDate);
        QTableWidgetItem* SprintenddateItem = new QTableWidgetItem(EndDate);
        QTableWidgetItem* SprinttitleItem = new QTableWidgetItem(Title);

        sprint_table->setItem(IssuerowCount, 0, ID);
        sprint_table->setItem(IssuerowCount, 1, SprintstartdateItem);
        sprint_table->setItem(IssuerowCount, 2, SprintenddateItem);
        sprint_table->setItem(IssuerowCount, 3, SprinttitleItem);
        QPushButton *viewButton = new QPushButton("Click Here");
        sprint_table->setCellWidget(IssuerowCount, 4, viewButton);
        connect(viewButton, &QPushButton::clicked, [this, IssuerowCount, idSprint, StartDate, EndDate, Title](){
            AdditionalDetails(IssuerowCount, idSprint, StartDate, EndDate, Title);
        });
    } else {
        qDebug() << "Table view not found or accessible.";
    }
}
void pb_sprint_implemenation::AdditionalDetails(int row, int idSprint, const QString& StartDate, const QString& EndDate, const QString& Title) {
    OBJECTS_VISIBLE_Additional_Details();
    qDebug() << "Button clicked on row" << row;
    qDebug() << "Sprint ID:" << idSprint;
    qDebug() << "Start Date:" << StartDate;
    qDebug() << "End Date:" << EndDate;
    qDebug() << "Title:" << Title;
    QLineEdit* SprintName = parentBoard->get_LineEdit_SprintName();
    QLineEdit* SprintStartDate = parentBoard->get_LineEdit_SprintStartDate();
    QLineEdit* SprintEndDate = parentBoard->get_Line_EditSprintEndDate();
    if (SprintName) {
        SprintName->setText(Title);
    }
    if (SprintStartDate) {
        SprintStartDate->setText(StartDate);
    }
    if (SprintEndDate) {
        SprintEndDate->setText(EndDate);
    }


    QTableWidget* Table_SprintDetails = parentBoard->get_Table_SprintDetails();
    Table_SprintDetails->clear();
    Table_SprintDetails->clearContents();
    Table_SprintDetails->setRowCount(0);
    parentBoard->get_Table_SprintDetails()->setColumnCount(4);
    parentBoard->get_Table_SprintDetails()->setHorizontalHeaderLabels({"ID", "Title", "Priority", "Status"}); // Set column headers
    Table_SprintDetails->setColumnHidden(0, true);


    QHeaderView* header = Table_SprintDetails->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();
    int PassedProjectID = parentBoard->getProjectId();
    if (dbobj.isOpen()) {
        QSqlQuery query(dbobj);
        query.prepare("SELECT idSprint, StartDate, EndDate, Title FROM Sprint WHERE Project_idProject = :projectID");
        query.bindValue(":projectID", PassedProjectID);
        if (query.exec()) {
            qDebug() << "Sprint Retrieved Successfully!";
            while (query.next()) {
                int idSprint = query.value(0).toInt();
                QString StartDate = query.value(1).toString();
                QString EndDate = query.value(2).toString();
                QString Title = query.value(3).toString();
            }
        }
    }
    DatabaseManager database1;
    QSqlDatabase dbobj1 = database1.getDatabase();
    if (dbobj1.isOpen()) {
        QSqlQuery query1(dbobj1);
        query1.prepare(
            "SELECT TSB.idTask, TSB.Title, TSB.Priority, TSB.Status "
            "FROM TaskSB AS TSB "
            "INNER JOIN SprintBacklog AS SB ON TSB.SprintBacklog_idSprintBacklog = SB.idSprintBacklog "
            "INNER JOIN Sprint AS S ON SB.Sprint_idSprint = S.idSprint "
            "WHERE S.Project_idProject = :projectID AND S.idSprint = :sprintID"
            );
        query1.bindValue(":projectID", PassedProjectID);
        query1.bindValue(":sprintID", idSprint);

        if (query1.exec()) {
            qDebug() << "Tasks Retrieved Successfully!";
            while (query1.next()) {
                int idTask = query1.value("idTask").toInt();
                QString title = query1.value("Title").toString();
                int priority = query1.value("Priority").toInt();
                QString status = query1.value("Status").toString();
                int currentRow = Table_SprintDetails->rowCount();
                Table_SprintDetails->insertRow(currentRow);
                Table_SprintDetails->setItem(currentRow, 0, new QTableWidgetItem(QString::number(idTask)));
                Table_SprintDetails->setItem(currentRow, 1, new QTableWidgetItem(title));
                Table_SprintDetails->setItem(currentRow, 2, new QTableWidgetItem(QString::number(priority)));
                Table_SprintDetails->setItem(currentRow, 3, new QTableWidgetItem(status));
            }
        } else {
            qDebug() << "Error retrieving tasks:" << query1.lastError().text();
        }
    }
}



//Create-Sprint-Task-------------------------------------------------------------------------------
void pb_sprint_implemenation::on_createtask_sprint_clicked() {
    qDebug() << "Create task sprint button clicked.";
    QLineEdit* Input_SprintName = parentBoard->get_Input_SprintName();
    QDateEdit* Input_StartDate = parentBoard->get_Input_StartDate();
    QDateEdit* Input_EndDate = parentBoard->get_Input_EndDate();


    //Correct the format of the date


    int PassedProjectID = parentBoard->getProjectId();
    if(Input_SprintName->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Missing Values", "Please fill in all fields.");
        return; // Return early if the input is empty
    }else{
        DatabaseManager database;
        QSqlDatabase dbobj = database.getDatabase();

        if (!dbobj.isOpen()) {
            qDebug() << "Connection Not Established - pb_productbacklog_implementation!";
            return;
        }

        // Start a transaction
        dbobj.transaction();

        // Insert into Sprint table
        QSqlQuery query(dbobj);
        query.prepare("INSERT INTO Sprint(Title, StartDate, EndDate, Project_idProject) "
                      "VALUES (:title, :StartDate, :EndDate, :Project_idProject)");
        query.bindValue(":title", Input_SprintName->text());
        query.bindValue(":StartDate", Input_StartDate->date());
        query.bindValue(":EndDate", Input_EndDate->date());
        query.bindValue(":Project_idProject", PassedProjectID);

        if (!query.exec()) {
            qDebug() << "Error executing query: " << query.lastError().text();
            dbobj.rollback(); // Rollback transaction on failure
            return;
        }

        // Get the last inserted id for Sprint
        int lastSprintId = query.lastInsertId().toInt();

        // Insert into SprintBacklog table
        query.prepare("INSERT INTO SprintBacklog (idSprintBacklog, Priority, Status, Sprint_idSprint, Sprint_Project_idProject) "
                      "VALUES (:idSprintBacklog, :Priority, :Status, :Sprint_idSprint, :Sprint_Project_idProject)");
        query.bindValue(":idSprintBacklog", lastSprintId);
        query.bindValue(":Priority", "N/A");
        query.bindValue(":Status", Input_SprintName->text());
        query.bindValue(":Sprint_idSprint", lastSprintId);
        query.bindValue(":Sprint_Project_idProject", PassedProjectID);

        if (!query.exec()) {
            qDebug() << "Failed to insert data into SprintBacklog table:" << query.lastError().text();
            dbobj.rollback(); // Rollback transaction on failure
            return;
        }
        // Commit the transaction
        dbobj.commit();
        qDebug() << "Data inserted into Sprint and SprintBacklog tables successfully!";
    }

}


void pb_sprint_implemenation::onDeleteButtonClicked() {
    deleteProject();
}


void pb_sprint_implemenation::deleteProject() {
    QTableWidget* sprint_table = parentBoard->getSprintTableView();
    QItemSelectionModel* selectionModel = sprint_table->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    if (selectedRows.isEmpty()) {
        qDebug() << "No rows selected for deletion.";
        return;
    }

    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();
    if (!dbobj.isOpen()) {
        qDebug() << "Connection Not Established - pb_productbacklog_implmentation!";
        return;
    }

    QSqlQuery query(dbobj);
    dbobj.transaction();  // Start a transaction to ensure atomicity

    for (const QModelIndex& selectedRow : selectedRows) {
        int row = selectedRow.row();
        int idSprint = sprint_table->item(row, 0)->text().toInt();  // Assuming the first column contains Sprint ID

        // SQL Statement to delete the sprint from the database
        query.prepare("DELETE FROM Sprint WHERE idSprint = :idSprint");
        query.bindValue(":idSprint", idSprint);

        if (!query.exec()) {
            qDebug() << "Error executing delete query: " << query.lastError().text();
            dbobj.rollback();  // Rollback the transaction on error
            return;
        }

        sprint_table->removeRow(row);
    }

    dbobj.commit();  // Commit the transaction if everything is successful
}


//Create-Sprint--------------------------------------------------------------------------------------
void pb_sprint_implemenation::on_create_sprint_clicked() {
    OBJECTS_VISIBLE_on_create_sprint_clicked();
    qDebug() << "Create sprint button clicked.";
    sprintGroupBox->setVisible(true);
    QLineEdit* sprintTitleLineEdit = parentBoard->get_Input_SprintName();
    QDateEdit* startDateEdit = parentBoard->get_Input_StartDate();
    QDateEdit* endDateEdit = parentBoard->get_Input_EndDate();
    int PassedProjectID = parentBoard->getProjectId();


    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();
    QString formattedStartDate = startDate.toString("MM/dd/yyyy");
    QString formattedEndDate = endDate.toString("MM/dd/yyyy");
    formattedStartDate = formattedStartDate.split("T").first();
    formattedEndDate = formattedEndDate.split("T").first();


    qDebug() << "formattedEndDate"<<formattedEndDate;
    qDebug() << "formattedStartDate"<<formattedStartDate;


    DatabaseManager database;
    QSqlDatabase dbobj = database.getDatabase();

    if (dbobj.isOpen()) {
        // Start a transaction
        dbobj.transaction();

        // Insert into Sprint table
        QSqlQuery query(dbobj);
        query.prepare("INSERT INTO Sprint (StartDate, EndDate, Title, Project_idProject) "
                      "VALUES (:StartDate, :EndDate, :Title, :Project_idProject)");
        // Fetch the actual date values and title text from the widgets
        query.bindValue(":StartDate", formattedStartDate);
        query.bindValue(":EndDate", formattedEndDate);
        query.bindValue(":Title", sprintTitleLineEdit->text());
        query.bindValue(":Project_idProject", PassedProjectID);

        if (!query.exec()) {
            qDebug() << "Failed to insert data into Sprint table:" << query.lastError().text();
            dbobj.rollback(); // Rollback transaction on failure
            return;
        }

        // Get the last inserted id for Sprint
        QVariant lastId = query.lastInsertId();
        if (!lastId.isValid()) {
            qDebug() << "Failed to retrieve last insert id from Sprint table.";
            dbobj.rollback();
            return;
        }

        // Insert into SprintBacklog table
        query.prepare("INSERT INTO SprintBacklog (Sprint_idSprint, Sprint_Project_idProject) "
                      "VALUES (:Sprint_idSprint, :Sprint_Project_idProject)");
        query.bindValue(":Sprint_idSprint", lastId.toInt());
        query.bindValue(":Sprint_Project_idProject", PassedProjectID);

        if (!query.exec()) {
            qDebug() << "Failed to insert data into SprintBacklog table:" << query.lastError().text();
            dbobj.rollback(); // Rollback transaction on failure
            return;
        }

        // Commit the transaction
        dbobj.commit();
        qDebug() << "Data inserted into Sprint and SprintBacklog tables successfully!";
    } else {
        qDebug() << "Connection Not Established - ProjectAdmin class!";
    }
}




///CHECK FROM @RUTH IF WE NEED THIS
void pb_sprint_implemenation::addSprintName (const QString& sprintName) {
    QComboBox* sprint_table_combobox = parentBoard->getSprint_Top_Down();
    sprint_table_combobox->addItem(sprintName);
}

//edit-Sprint--------------------------------------------------------------------------------------
void pb_sprint_implemenation::on_editsprint_sprint_clicked() {
    qDebug() << "Edit sprint button clicked.";

    QComboBox* sprint_top_down = parentBoard->getSprint_Top_Down();
    QTextBrowser* date = parentBoard->getSprintDate();

    // Get the selected sprint name from the dropdown
    QString selectedSprintName = sprint_top_down->currentText();
    QTextBrowser *selectedSprintDate = date;

    // Get the new sprint name
    QString newSprintName = QInputDialog::getText(nullptr, "Enter Sprint Name", "Sprint Name:", QLineEdit::Normal, selectedSprintName);

    // Check if the user canceled the input
    if (newSprintName.isEmpty()) {
        qDebug() << "Edit canceled. Sprint Name remains unchanged.";
        return;
    }

    // Get the new date
    QString newDate = QInputDialog::getText(nullptr, "Enter Sprint Date", "Sprint Date:", QLineEdit::Normal, selectedSprintDate->toPlainText());

    // Check if the user canceled the input for the date
    if (newDate.isEmpty()) {
        qDebug() << "Edit canceled. Sprint Date remains unchanged.";
        return;
    }

    // Update the existing sprint's name and date
    editSprint(selectedSprintName, newSprintName);
    selectedSprintDate->setText(newDate);

    qDebug() << "Sprint Name: " << newSprintName;
    qDebug() << "Sprint Date: " << newDate;
}

void pb_sprint_implemenation::editSprint(const QString& oldSprintName, const QString& newSprintName) {
    QComboBox* sprint_top_down = parentBoard->getSprint_Top_Down();
    // Update the dropdown menu with the new sprint name
    int index = sprint_top_down->findText(oldSprintName);
    if (index != -1) {
        sprint_top_down->setItemText(index, newSprintName);
    }
}



void pb_sprint_implemenation::addTaskSprint() {

}
