#ifndef PARENTBOARD_H
#define PARENTBOARD_H

#include "qcombobox.h"
#include "qpushbutton.h"
#include "qtextbrowser.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QTableWidget> // Include QTableWidget header

namespace Ui {
class parentboard;
}

class parentboard : public QWidget
{
    Q_OBJECT

public:
    explicit parentboard(QWidget *parent = nullptr);
    ~parentboard();
    QWidget* getSomeWidget(); // Declare the function
    QTableWidget* getUserStoriesTableView();
    QTableWidget* getIssuesTableView();
    QComboBox* getSprint_Top_Down();
    QPushButton* on_create_sprint_clicked();
    QTableWidget* getSprintTableView();
    QTextBrowser *getSprintDate();
private slots:
    void on_taskboardButton_clicked();
    void on_backlogButton_clicked();
    void on_sprintsButton_clicked();
    void on_confluenceButton_clicked();
    void goBackToMainWindow();
    void confluence_class();
private:
    Ui::parentboard *ui;
    QStandardItemModel *taskModel;
};

#endif // PARENTBOARD_H
